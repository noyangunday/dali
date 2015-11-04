/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

// CLASS HEADER
#include "shared-file.h"

// EXTERNAL INCLUDES
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/mman.h>

#include <cstring>


namespace Dali
{
namespace Internal
{
namespace Adaptor
{

SharedFile* SharedFile::New(const char* filename, int size, bool isSystem)
{
  SharedFile *sf = NULL;

  sf = new SharedFile;

  bool opened = sf->OpenFile( filename, size, isSystem );
  if( !opened )
  {
    delete sf;
    sf = NULL;
  }
  return sf;
}

SharedFile::SharedFile()
: mFileDescriptor(-1),
  mSize(0),
  mAddress(NULL),
  mFilename()
{
}

SharedFile::~SharedFile()
{
  Close();
}

void SharedFile::Close()
{
  if( mAddress != NULL )
  {
    munmap( mAddress, mSize );
    mAddress = NULL;
  }

  if( mFileDescriptor >= 0 )
  {
    close( mFileDescriptor );
    mFileDescriptor = -1;
  }
}

unsigned char* SharedFile::GetAddress()
{
  return static_cast<unsigned char *>( mAddress );
}

bool SharedFile::OpenFile(const char* filename, int size, bool isSystem)
{
  bool opened = false;
  mode_t mode;

  mode = S_IRUSR | S_IWUSR;
  if( isSystem )
  {
    mode |= S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  }

  mFileDescriptor = shm_open( filename, O_RDONLY, mode );

  if( mFileDescriptor >= 0 )
  {
    mFilename = filename;

    mSize = size;
    mAddress = mmap( NULL, mSize, PROT_READ, MAP_SHARED, mFileDescriptor, 0 );

    if( mAddress != MAP_FAILED )
    {
      opened = true;
    }
  }
  return opened;
}

} // Adaptor
} // Internal
} // Dali
