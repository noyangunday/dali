#!/bin/bash
# Log resource analyser tool for Dali
# Monitors resource usage of last run Dali app
# Shows memory uploaded to GPU or normal RAM
# Texture atlas usage usually reflects used Font atlases

set -u

#global variables
TERM_W=0       # latest width of terminal window
TERM_H=0       # latest height of terminal window
SEPARATOR_H=5  # 5 lines in info bar
CURPAGE=0      # current page number
MAXFILENO=0    # maximum lines to display from resourcelist
MAXP=0         # number of pages

DLOGTEMPFILE=/tmp/dalidlog.txt
DLOGUTIL=/usr/bin/dlogutil
USING_DLOG=
INPUTFILE=""



#possible states
# ID| Desc                             | Color     | Tags
#---+----------------------------------+-----------+-------
# 0.| loaded in CPU memory             |  [CPU]    | [LOAD]
# 1.| present in both memories         |  [CPUGPU] | [LOAD] [UPLOAD]
# 2.| GPU memory only, buffer discarded|  [GPU]    | [UPLOAD] [DELBUF]
# 3.| loaded but discarded later on    |  [DISC]   | [LOAD] [DELBUF] or [DELBUF] [DELTEXTURE]

#colors for marking resource state
COLOR_CPU=5
COLOR_CPUGPU=1
COLOR_GPU=2
COLOR_DISC=6

declare -a COLORS=( $COLOR_CPU $COLOR_CPUGPU $COLOR_GPU $COLOR_DISC )

declare -a FILENAMES_G=( )
declare -a BITMAPS_G=( )
declare -a TEXTURES_G=( )
declare -a STATES_G=( )
declare -a SIZES_G=( )
declare -a SIZE_DETAILS_G=( )

ATLASMEM=0
ATLAS_NO=0

CPUMEM=0
GPUMEM=0

#process ID of last running Dali app
PID_G=0

#distinguish texture atlases from framebuffer textures
BITMAP_ID_ATLAS=0
BITMAP_ID_FB_TEXTURE="-"

###################################string formatting, command line and error handling
function error
{
  echo "Error: $1"
  cleanup
  exit 1
}

function usage
{
    echo "usage: ./dalireslog.sh [FILE]"
    echo "if FILE isn't specified script will try to use dlogutil"
}

function getTermSize
{
    TERM_W=$(tput cols)
    TERM_H=$(tput lines)

    let MAXFILENO=$(($TERM_H-$SEPARATOR_H-2)) #leave space for keyboard shortcuts and separator itself
    let MAXP=${#FILENAMES_G[@]}/$MAXFILENO

    # don't show empty page if list just fits on screen
    local rmd=0
    let rmd=${#FILENAMES_G[@]}%$MAXFILENO
    if [ $rmd -eq 0 ]
    then
        let MAXP-=1;
    fi
}

# print string, notifying user if it doesn't fit in one line. takes one parameter
function printString
{
    echo -n ${1:0:$TERM_W}
    if [[ $TERM_W -lt ${#1} ]]
    then
        tput cub 1;
        tput setab 1; tput smso; echo -n '>'; tput el; tput rmso
        return 1
    else
        tput el
        return 0
    fi
}

# print string, clear until end of line, print newline. takes one parameter
function printLine
{
    printString "$1"
    local RET=$?
    printf '\n'
    return $RET
}

function parseCmdLine
{
    if [[ $# -lt 1 ]]
    then
        # try using dlogutil
        if [[ ! -e "$DLOGUTIL" ]]
        then
            echo "dlogutil not installed"
            usage
            exit 1
        fi

        INPUTFILE="$DLOGTEMPFILE"
        USING_DLOG=true
    else
        # check if help is requested
        if [[ $1 == '-h' || $1 == '--help' ]]
        then
            usage
            exit 0
        # try reading from file
        else
            INPUTFILE=$1
            if [[ ! -e "$INPUTFILE" ]]
            then
                echo cannot read file "$INPUTFILE"
                usage
                exit 1
            fi
        fi
    fi
}

# print filename or basename or "..." depending on terminal size, takes one parameter
function printPath
{
    if [ -z "$1" ]
    then
        echo "ERROR in printPath";
        cleanup; exit 1
    fi

    FILENAME="$1"
    FBASENAME=$(basename $FILENAME)
    if [[ ${#FILENAME} -lt $TERM_W ]]
    then
        printLine "$FILENAME"
    else
        if [[ ${#FBASENAME} -lt $TERM_W ]]
        then
            printLine "$FBASENAME"
        else
            printLine ...
        fi
    fi
}

###################################memory query functions
function getGpuMemUsage
{
  GPUMEM=0
  local i=0
  for state in ${STATES_G[@]}
  do
    if [[ $state == 1 || $state == 2 ]]
    then
      let GPUMEM+=${SIZES_G[$i]}
    fi
    let i+=1
  done
  return $GPUMEM
}

function getCpuMemUsage
{
  CPUMEM=0
  local i=0
  for state in ${STATES_G[@]}
  do
    if [[ $state == 0 || $state == 1 ]]
    then
      let CPUMEM+=${SIZES_G[$i]}
    fi
    let i+=1
  done
  return $CPUMEM
}

function getAtlasNumber
{
  ATLAS_NO=0
  local i=0
  for bitmap in ${BITMAPS_G[@]}
  do
    if [[ $bitmap == 0 && ${STATES_G[$i]} == 2 ]]
    then
      let ATLAS_NO+=1
    fi
    let i+=1
  done
  return $ATLAS_NO
}

function getAtlasMemUsage
{
  ATLASMEM=0
  local i=0
  for bitmap in ${BITMAPS_G[@]}
  do
    if [[ $bitmap == 0 && ${STATES_G[$i]} == 2 ]]
    then
      let ATLASMEM+=${SIZES_G[$i]}
    fi
    let i+=1
  done
  return $ATLASMEM
}

##################################global arrays manipulation
#adds record to resource list
#params: filename, bitmap, texture, state, size, size detail
function addRecord
{
  if [ $# -ne 6 ]
  then
    error "addRecord - number of arguments is $#"
  fi
  FILENAMES_G+=("$1")
  BITMAPS_G+=("$2")
  TEXTURES_G+=("$3")
  STATES_G+=("$4")
  SIZES_G+=("$5")
  SIZE_DETAILS_G+=("$6")
}

#adds image resource to list
#params: filename, bitmap, size, size detail
function fileLoaded
{
  if [ $# -ne 4 ]
  then
    error "fileLoaded"
  fi
  FILENAMES_G+=("$1")
  BITMAPS_G+=("$2")
  SIZES_G+=("$3")
  SIZE_DETAILS_G+=("$4")
  TEXTURES_G+=(0)
  STATES_G+=(0)
}

#params: texture, size, size detail
function atlasUploaded
{
  FILENAMES_G+=("-")
  BITMAPS_G+=("$BITMAP_ID_ATLAS")
  TEXTURES_G+=("$1")
  STATES_G+=(2)
  SIZES_G+=("$2")
  SIZE_DETAILS_G+=("$3")
}

#params: size, size detail
function frameBufUploaded
{
  FILENAMES_G+=("$1")
  BITMAPS_G+=("$BITMAP_ID_FB_TEXTURE")
  TEXTURES_G+=("$2")
  STATES_G+=(2)
  SIZES_G+=("$3")
  SIZE_DETAILS_G+=("$4")
}


##################################log parsing functions
function checkLoaded
{
  if [[ "$1" =~ .*DALI.*[LOAD].*file\ (.*)\ to\ Bitmap\ (.*)\ -\ size\ ([[:digit:]]*)\ bytes\ (.*) ]]
  then
    local FILENAME="${BASH_REMATCH[1]}"
    local BITMAP="${BASH_REMATCH[2]}"
    local SIZE="${BASH_REMATCH[3]}"
    local SIZE_DETAILS="${BASH_REMATCH[4]}"

    local found=0

    #check if file was loaded before with same size
    local i=0
    if [ ${#FILENAMES_G[@]} -ne 0 ]
    then

    for filenameIter in ${FILENAMES_G[@]}
    do
      if [[ "$filenameIter" == "$FILENAME" ]]
      then
        if [[ ${SIZES_G[$i]} == "$SIZE" && ${SIZE_DETAILS_G[$i]} == "$SIZE_DETAILS" ]]
        then
          found=1
          case ${STATES_G[$i]} in
            0) #CPU
              BITMAPS_G[$i]="$BITMAP"
              ;;
            1) #CPUGPU
              BITMAPS_G[$i]="$BITMAP"
              ;;
            2) #GPU
              STATES_G[$i]=1 #GPU->CPUGPU  loaded into memory again
              BITMAPS_G[$i]="$BITMAP"
              ;;
            3) #DISC
              #previously discarded, load again
              STATES_G[$i]=0
              BITMAPS_G[$i]="$BITMAP"
              ;;
            *)
              error "checkLoaded - unknown state"
              ;;
          esac
        else
          #filename is same, but its loaded in different size
          :
        fi
      fi
      let i+=1
    done
    fi

    if [ $found -ne 1 ]
    then
      fileLoaded "$FILENAME" "$BITMAP" "$SIZE" "$SIZE_DETAILS"
    fi

    return 0
  else
    error "checkLoaded"
  fi
}

function checkUploaded
{
  if [[ "$1" =~ .*DALI.*[UPLOAD].*Bitmap\ (.*)\ to\ Texture\ (.*)\ -\ size\ ([[:digit:]]*)\ bytes\ (.*) ]]
  then
    local BITMAP="${BASH_REMATCH[1]}"
    local TEXTURE="${BASH_REMATCH[2]}"
    local SIZE="${BASH_REMATCH[3]}"
    local SIZE_DETAILS="${BASH_REMATCH[4]}"

    local i=0
    local lastIdx=-1

    if [[ "$BITMAP" =~ \(nil\) ]]
    then
      atlasUploaded $TEXTURE $SIZE "$SIZE_DETAILS"
      return 0
    else
      #not a texture atlas
      if [ ${#BITMAPS_G[@]} -ne $BITMAP_ID_ATLAS ]
      then
        for bitmap in ${BITMAPS_G[@]}
        do
          if [ $bitmap == $BITMAP ]
          then
            lastIdx=$i
          fi
        let i+=1
        done
      fi
    fi

    if [ $lastIdx != -1 ]
    then
      #Bitmap found
      if [[ ${TEXTURES_G[$lastIdx]} == 0 && ${STATES_G[$lastIdx]} == 0 ]]
      then
        #File loaded in memory -> upload to GPU
        TEXTURES_G[$lastIdx]="$TEXTURE"
        STATES_G[$lastIdx]=1
      elif [[ ${FILENAMES_G[$lastIdx]} == "-" && ${STATES_G[$lastIdx]} == 1 ]]
      then
        #BufferImage already in memory and GPU mem. -> updated
        SIZES_G[$lastIdx]=$SIZE
        SIZE_DETAILS_G[$lastIdx]="$SIZE_DETAILS"
      else
        #bitmap uploaded to new texture
        addRecord ${FILENAMES_G[$lastIdx]} $BITMAP $TEXTURE 1 $SIZE "$SIZE_DETAILS"
      fi
    else
      #bitmapImage - not loaded from file
      #newly added
      addRecord "-" $BITMAP $TEXTURE 1 $SIZE "$SIZE_DETAILS"
    fi
    return 0
  elif [[ "$1" =~ .*DALI.*[UPLOAD].*FrameBufferTexture\ (.*)\ GL\ Texture\ (.*)\ -\ size\ ([[:digit:]]*)\ bytes\ (.*) ]]
  then
    local FBTEXTURE="${BASH_REMATCH[1]}"
    local TEXTURE="${BASH_REMATCH[2]}"
    local SIZE="${BASH_REMATCH[3]}"
    local SIZE_DETAILS="${BASH_REMATCH[4]}"
    frameBufUploaded "$FBTEXTURE" "$TEXTURE" "$SIZE" "$SIZE_DETAILS"
    return 0
  else
    echo "$1"
    error "checkUploaded"
  fi
}

function checkDeletedBuf
{
  if [[ "$1" =~ .*DALI.*[DELBUF].*Bitmap\ (.*)\ -\ .*size\ (.*) ]]
  then
    local BITMAP=${BASH_REMATCH[1]}
    local i=0

    for bitmap in ${BITMAPS_G[@]}
    do
      if [ $bitmap == "$BITMAP" ]
      then
        case ${STATES_G[$i]} in
        0)
            STATES_G[$i]=3 #CPU->DISC
            ;;
        1)
            STATES_G[$i]=2 #CPUGPU->GPU
            ;;
        2)
            #GPU->?
            #probably previously freed bitmap buffer but memory is reused since
            ;;
        3)
            #DISC->?
            #probably previously freed but memory is reused since
            ;;
        *)
            error "checkDeletedBuf - unkown state"
            ;;
        esac
      fi
    let i+=1
    done

    return 0
  else
    echo "$1"
    error "checkDeletedBuf"
  fi
}

function checkDeletedTexture
{
  if [[ "$1" =~ .*DALI.*[DELTEXTURE].*Texture\ (.*)\ -\ size\ (.*) ]]
  then
    local TEXTURE="${BASH_REMATCH[1]}"
    local i=0
    local lastIdx=-1

    for texture in ${TEXTURES_G[@]}
    do
      if [ $texture == $TEXTURE ]
      then
        lastIdx=$i
      fi
    let i+=1
    done

    if [ $lastIdx != -1 ]
    then
      case ${STATES_G[$lastIdx]} in
      0)
          #CPU->?
          echo "$1"
          error "checkDeletedTexture - state CPU"
          ;;
      1)
          STATES_G[$lastIdx]=0 #CPUGPU->CPU
          ;;
      2)
          STATES_G[$lastIdx]=3 #GPU->DISC
          ;;
      3)
          #DISC->?
          echo "$1"
          error "checkDeletedTexture - state DISC"
          ;;
      *)
          error "checkDeletedTexture - unkown state"
          ;;
      esac
    else
      echo "$1"
      error "checkDeletedTexture - texture not uploaded"
    fi
    return 0
  else
    echo "$1"
    error "checkDeletedTexture"
  fi
}

function processLine
{
  if [[ "$1" =~ .*DALI.*\ \[(.*)\].* ]]
  then
    RESCMD=${BASH_REMATCH[1]}
    case "$RESCMD" in
    LOAD)
        checkLoaded "$1"
        ;;
    UPLOAD)
        checkUploaded "$1"
        ;;
    DELBUF)
        checkDeletedBuf "$1"
        ;;
    DELTEXTURE)
        checkDeletedTexture "$1"
        ;;
    INIT)
        ;;
    FIN)
        return 1 #end of last log session
        ;;
    *)
        error "Unkown command $RESCMD"
        ;;
    esac
  fi
  return 0
}

function parseFile
{
  if [ -z "$1" ]
  then
    echo "ERROR in parseFile";
    cleanup; exit 1
  fi

  #return if file does not contain dali resource log
  if ! grep -q -m 1 -E "DALI.*\[INIT\]" $1
  then
    return 1
  fi

  #find last resource log session
  local LOGBUFFER=$(sed -n 'H; /DALI.*\[INIT\]/h; ${g;p;}' $1)

  while read -r line
  do
    #show PID of last process
    PID_G=$(echo "$line" | sed 's/[^0-9]*\([0-9]*\).*/\1/')
    if [ ! -z "$PID_G" ]
    then
      break
    fi
  done <<< "$LOGBUFFER"

  while read -r line
  do
    if ! processLine "$line" #stop parsing at the end of last session
    then
      break
    fi
  done <<< "$LOGBUFFER"
}

##################################draw and main functions
function redraw
{
  tput cup 0 0 #move cursor to top left

  # print info (4 lines)
  tput bold
  printLine "PID: $PID_G"
  printLine "MEM 3D: $GPUMEM"
  printLine "MEM Atlas: $ATLASMEM";
  printLine "MEM CPU: $CPUMEM"
  printLine "Number of atlases: $ATLAS_NO";
  tput sgr0

  # separator bar (colored bar with (actual/number of files) count)
  tput cup $SEPARATOR_H 0
  local PAGEIND="$(expr $CURPAGE + 1)/$(expr $MAXP + 1)"
  local FILL_W=0
  let FILL_W=$TERM_W-${#PAGEIND}
  tput setab 4; printf $PAGEIND%"$FILL_W"s; printf '\n'; tput sgr0

  # print filenames
  local count=0
  local index=0
  let index=$CURPAGE*$MAXFILENO

  filecount=${#FILENAMES_G[@]}

  tput setaf 252

  while [[ $count -lt $MAXFILENO ]]
  do
    if [[ $index -lt $filecount ]]
    then
      tput setab ${COLORS[${STATES_G[$index]}]}
#     printPath "${FILENAMES_G[$count]}"
      printLine "${FILENAMES_G[$index]} ${SIZES_G[$index]} ${SIZE_DETAILS_G[$index]}"
    else
      tput sgr0
      printLine "" #clear remaining lines to fill screen
    fi
    let count+=1
    let index+=1
  done

  # print keyboard shortcuts
  tput setab 4; tput bold
  IFS= printString "  |  n: next page  |  p: previous page  |  ^C: exit  |  Resource state: "
  # print color codes
  if [[ $TERM_W -gt 100 ]]
  then
    tput setab ${COLORS[0]}
    echo -n " CPU "
    tput setab ${COLORS[1]}
    echo -n " CPUGPU "
    tput setab ${COLORS[2]}
    echo -n " GPU "
    tput setab ${COLORS[3]}
    echo -n " DISCARDED "
  fi

  tput sgr0
}

function readInput
{
    local key
    read -n1 -t 0.3 key

    case "$key" in
        'p')
            if [[ $CURPAGE -ne 0 ]]
            then
              let CURPAGE-=1
            fi
            ;;
        'n')
            if [[ $CURPAGE -lt $MAXP ]]
            then
              let CURPAGE+=1
            fi
            ;;
    esac
}

function initVars
{
  FILENAMES_G=( )
  BITMAPS_G=( )
  TEXTURES_G=( )
  SIZES_G=( )
  SIZE_DETAILS_G=( )
  STATES_G=( )
}

function cleanup
{
  tput cup 9999 0 #go to bottom of screen
  tput cnorm #show cursor
  tput sgr0
  if [ -f "$DLOGTEMPFILE" ]
  then
    rm "$DLOGTEMPFILE"
  fi
}

function update
{
  initVars
  if [ -n "$USING_DLOG" ]
  then
    if [ -f "$DLOGTEMPFILE" ]
    then
      rm "$DLOGTEMPFILE"
    fi
    "$DLOGUTIL" DALI:I -d -f "$DLOGTEMPFILE" 2>/dev/null
  fi

  if [ ! -e "$INPUTFILE" ]
  then
    return 1
  fi

  parseFile "$INPUTFILE"

  if [[ $? -gt 0 || ${#STATES_G[@]} -lt 1 ]]
  then
    return 1
  fi

  getCpuMemUsage
  getGpuMemUsage
  getAtlasMemUsage
  getAtlasNumber

  getTermSize
  readInput
  redraw
}

function main
{
  parseCmdLine $@

  if [ -z "$INPUTFILE" ]
  then
    echo No input file specified;
    cleanup
    exit 1
  fi

  tput civis #hide cursor
  tput clear #clear screen

  echo "waiting for log..."

  while [ 1 ]
  do
    update
#    sleep 0.3  # we are now reading input for 0.3
  done

  cleanup
}

trap "cleanup; exit 0" SIGINT SIGTERM  #reset terminal when ^C is pressed
# trap "getTermSize" SIGWINCH            #handle window resize

main $@
