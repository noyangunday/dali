/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd.
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

// FILE HEADER
#include <dali/devel-api/text-abstraction/script.h>

namespace Dali
{

namespace TextAbstraction
{

namespace
{
const unsigned int WHITE_SPACE_THRESHOLD  = 0x21; ///< All characters below 0x21 are considered white spaces.
const unsigned int CHAR_LF   = 0x000A; ///< NL Line feed, new line.
const unsigned int CHAR_VT   = 0x000B; ///< Vertical tab.
const unsigned int CHAR_FF   = 0x000C; ///< NP Form feed, new page.
const unsigned int CHAR_CR   = 0x000D; ///< Carriage return, new line.
const unsigned int CHAR_NEL  = 0x0085; ///< Next line.
const unsigned int CHAR_LS   = 0x2028; ///< Line separator.
const unsigned int CHAR_PS   = 0x2029; ///< Paragraph separator

const unsigned int CHAR_ZWS  = 0x200B; ///< Zero width space.
const unsigned int CHAR_ZWNJ = 0x200C; ///< Zero width non joiner.
const unsigned int CHAR_ZWJ  = 0x200D; ///< Zero width joiner.
const unsigned int CHAR_LTRM = 0x200E; ///< Left to Right Mark.
const unsigned int CHAR_RTLM = 0x200F; ///< Right to Left Mark.
const unsigned int CHAR_TS   = 0x2009; ///< Thin Space.
} // namespace

bool IsRightToLeftScript( Script script )
{
  return ( ( ARABIC == script ) ||
           ( HEBREW == script ) );
}

Script GetCharacterScript( Character character )
{
  // Latin script:
  // 0x0000 - 0x007f C0 Controls and Basic Latin
  // 0x0080 - 0x00ff C1 Controls and Latin-1 Supplement
  // 0x0100 - 0x017f Latin Extended-A
  // 0x0180 - 0x024f Latin Extended-B
  // 0x0250 - 0x02af IPA Extensions
  // 0x02b0 - 0x02ff Spacing Modifier Letters
  // 0x1d00 - 0x1d7f Phonetic Extensions
  // 0x1d80 - 0x1dbf Phonetic Extensions Supplement
  // 0x1e00 - 0x1eff Latin Extended Additional
  // 0x2070 - 0x209f Superscripts and Subscripts
  // 0x2100 - 0x214f Letterlike symbols
  // 0x2150 - 0x218f Number Forms
  // 0x2c60 - 0x2c7f Latin Extended-C
  // 0xa720 - 0xa7ff Latin Extended-D
  // 0xab30 - 0xab6f Latin Extended-E
  // 0xfb00 - 0xfb06 Latin Alphabetic Presentation Forms
  // 0xff00 - 0xffef Halfwidth and Fullwidth Forms

  // Brahmic scripts:
  // 0x0900 - 0x097f Devanagari
  // 0x0980 - 0x09ff Bengali
  // 0x0a00 - 0x0a7f Gurmukhi
  // 0x0a80 - 0x0aff Gujarati
  // 0x0b00 - 0x0b7f Oriya
  // 0x0b80 - 0x0bff Tamil
  // 0x0c00 - 0x0c7f Telugu
  // 0x0c80 - 0x0cff Kannada
  // 0x0d00 - 0x0d7f Malayalam

  // Sinhala script.
  // 0x0d80 - 0x0dff Sinhala

  // Arabic script.
  // 0x0600 - 0x06ff Arabic
  // 0x0750 - 0x077f Arabic Supplement
  // 0x08A0 - 0x08ff Arabic Extended-A
  // 0xfb50 - 0xfdff Arabic Presentation Forms-A
  // 0xfe70 - 0xfeff Arabic Presentation Forms-B
  // 0x1ee00 - 0x1eeff Arabic Mathematical Alphabetic Symbols

  // CJK (Chinese, Japanese and Korean) and Vietnamese script.
  // 0x2e80 - 0x2eff CJK Radicals Supplement
  // 0x2f00 - 0x2fdf Kangxi Radicals
  // 0x3000 - 0x303f CJK Symbols and Punctuation
  // 0x3200 - 0x32ff Enclosed CJK Letters and Months
  // 0x3400 - 0x4dbf CJK Unified Ideographs Extension A
  // 0x4e00 - 0x62ff CJK Unified Ideographs
  // 0x6300 - 0x77ff CJK Unified Ideographs
  // 0x7800 - 0x8cff CJK Unified Ideographs
  // 0x8d00 - 0x9fff CJK Unified Ideographs
  // 0x20000 - 0x215ff CJK Unified Ideographs Extension B
  // 0x21600 - 0x230ff CJK Unified Ideographs Extension B
  // 0x23100 - 0x245ff CJK Unified Ideographs Extension B
  // 0x24600 - 0x260ff CJK Unified Ideographs Extension B
  // 0x26100 - 0x275ff CJK Unified Ideographs Extension B
  // 0x27600 - 0x290ff CJK Unified Ideographs Extension B
  // 0x29100 - 0x2a6df CJK Unified Ideographs Extension B
  // 0x2a700 - 0x2b73f CJK Unified Ideographs Extension C
  // 0x2b740 - 0x2b81f CJK Unified Ideographs Extension D

  // Japanese scripts.
  // 0x3040 - 0x309f Hiragana
  // 0x30a0 - 0x30ff Katakana

  // Hangul script
  // 0x1100 - 0x11ff Hangul jamo
  // 0x3130 - 0x318f Hangul Compatibility Jamo
  // 0xa960 - 0xa97f Hangul Jamo Extended-A
  // 0xac00 - 0xd7af Hangul Syllables
  // 0xd7b0 - 0xd7ff Hangul Jamo Extended-B

  // Bopomofo script
  // 0x3100 - 0x312f Bopomofo
  // 0x31a0 - 0x31bf Bopomofo Extended

  // Khmer script
  // 0x1780 - 0x17ff Khmer
  // 0x19e0 - 0x19ff Khmer Symbols

  // Lao script
  // 0x0e80 - 0x0eff Lao

  // Thai script
  // 0x0e00 - 0x0e7f Thai

  // Burmese script
  // 0x1000 - 0x109f Myanmar

  // Hebrew script
  // 0x0591 - 0x05f4 Hebrew
  // 0xfb1d - 0xfb4f Hebrew subset of Alphabetic Presentation Forms

  // Cyrillic script
  // 0x0400 - 0x04ff Cyrillic
  // 0x0500 - 0x052f Cyrillic suplement
  // 0x2de0 - 0x2dff Cyrillic Extended-A
  // 0xa640 - 0xa69f Cyrillic Extended-B

  // Georgian script
  // 0x10a0 - 0x10ff Georgian
  // 0x2d00 - 0x2d2f Georgian suplement

  // Greek script
  // 0x0370 - 0x03ff Greek & Coptic
  // 0x1f00 - 0x1fff Greek Extended

  // Armenian script
  // 0x0530 - 0x058f Armenian
  // 0xfb13 - 0xfb17 Armenian subset of Alphabetic prefentation forms

  // The Emoji which map to standardized Unicode characters
  // 1. Emoticons ( 1F601 - 1F64F )
  // 2. Dingbats ( 2702 - 27B0 )
  // 3. Transport and map symbols ( 1F680 - 1F6C0 )
  // 4. Enclosed characters ( 24C2 - 1F251 )
  // 5. Uncategorized :-S
  // 6. Additional Emoticons ( 1F600 - 1F636 )
  // 6b. Additional transport and map symbols ( 1F681 - 1F6C5 )
  // 6c. Other additional symbols ( 1F30D - 1F567 )

  if( IsCommonScript( character ) )
  {
    return COMMON;
  }

  if( character <= 0x0cff )
  {
    if( character <= 0x09ff )
    {
      if( character <= 0x077f )
      {
        if( character == 0x00A9 )
        {
          return EMOJI; // 5. Uncategorized: copyright sign
        }
        if( character == 0x00AE )
        {
          return EMOJI; // 5. Uncategorized: registered sign
        }
        if( character <= 0x02ff )
        {
          return LATIN;
        }
        if( ( 0x0370 <= character ) && ( character <= 0x03ff ) )
        {
          return GREEK;
        }
        if( ( 0x0400 <= character ) && ( character <= 0x04ff ) )
        {
          return CYRILLIC;
        }
        if( ( 0x0500 <= character ) && ( character <= 0x052f ) )
        {
          return CYRILLIC;
        }
        if( ( 0x0530 <= character ) && ( character <= 0x058f ) )
        {
          return ARMENIAN;
        }
        if( ( 0x0591 <= character ) && ( character <= 0x05f4 ) )
        {
          return HEBREW;
        }
        if( ( 0x0600 <= character ) && ( character <= 0x06ff ) )
        {
          return ARABIC;
        }
        if( ( 0x0750 <= character ) && ( character <= 0x077f ) )
        {
          return ARABIC;
        }
      }
      else // > 0x077f
      {
        if( ( 0x08A0 <= character ) && ( character <= 0x08ff ) )
        {
          return ARABIC;
        }
        if( ( 0x0900 <= character ) && ( character <= 0x097f ) )
        {
          return DEVANAGARI;
        }
        if( ( 0x0980 <= character ) && ( character <= 0x09ff ) )
        {
          return BENGALI;
        }
      }
    }
    else // > 0x09ff
    {
      if( character <= 0x0b7f )
      {
        if( ( 0x0a00 <= character ) && ( character <= 0x0a7f ) )
        {
          return GURMUKHI;
        }
        if( ( 0x0a80 <= character ) && ( character <= 0x0aff ) )
        {
          return GUJARATI;
        }
        if( ( 0x0b00 <= character ) && ( character <= 0x0b7f ) )
        {
          return ORIYA;
        }
      }
      else // > 0x0b7f
      {
        if( ( 0x0b80 <= character ) && ( character <= 0x0bff ) )
        {
          return TAMIL;
        }
        if( ( 0x0c00 <= character ) && ( character <= 0x0c7f ) )
        {
          return TELUGU;
        }
        if( ( 0x0c80 <= character ) && ( character <= 0x0cff ) )
        {
          return KANNADA;
        }
      }
    }
  }
  else // > 0x0cff
  {
    if( character <= 0x2c7f )
    {
      if( character <= 0x1eff )
      {
        if( ( 0x0d00 <= character ) && ( character <= 0x0d7f ) )
        {
          return MALAYALAM;
        }
        if( ( 0x0d80 <= character ) && ( character <= 0x0dff ) )
        {
          return SINHALA;
        }
        if( ( 0x0e00 <= character ) && ( character <= 0x0e7f ) )
        {
          return THAI;
        }
        if( ( 0x0e80 <= character ) && ( character <= 0x0eff ) )
        {
          return LAO;
        }
        if( ( 0x1000 <= character ) && ( character <= 0x109f ) )
        {
          return BURMESE;
        }
        if( ( 0x10a0 <= character ) && ( character <= 0x10ff ) )
        {
          return GEORGIAN;
        }
        if( ( 0x1100 <= character ) && ( character <= 0x11ff ) )
        {
          return HANGUL;
        }
        if( ( 0x1780 <= character ) && ( character <= 0x17ff ) )
        {
          return KHMER;
        }
        if( ( 0x19e0 <= character ) && ( character <= 0x19ff ) )
        {
          return KHMER;
        }
        if( ( 0x1d00 <= character ) && ( character <= 0x1eff ) )
        {
          return LATIN;
        }
      }
      else // > 0x1eff
      {
        if( ( 0x1f00 <= character ) && ( character <= 0x1fff ) )
        {
          return GREEK;
        }
        if( character == 0x203c )
        {
          return EMOJI; // 5. Uncategorized: double exclamation mark
        }
        if( character == 0x2049 )
        {
          return EMOJI; // 5. Uncategorized: exclamation question mark
        }
        if( ( 0x2070 <= character ) && ( character <= 0x209f ) )
        {
          return LATIN;
        }
        if( character == 0x20e3 )
        {
          return EMOJI; // 5. Uncategorized: combining enclosing keycap
        }
        if( character == 0x2122 )
        {
          return EMOJI; // 5. Uncategorized: trade mark sign
        }
        if( character == 0x2139 )
        {
          return EMOJI; // 5. Uncategorized: information source
        }
        if( ( 0x2100 <= character ) && ( character <= 0x218f ) )
        {
          return LATIN;
        }
        // U+2194 5. Uncategorized: left right arrow
        // U+2B55 5. Uncategorized: heavy large circle
        if( ( 0x2194 <= character ) && ( character <= 0x2B55 ) )
        {
          return EMOJI;
        }
        if( ( 0x2c60 <= character ) && ( character <= 0x2c7f ) )
        {
          return LATIN;
        }
      }
    }
    else // > 0x2c7f
    {
      if( character <= 0xfdff )
      {
        if( ( 0x2d00 <= character ) && ( character <= 0x2d2f ) )
        {
          return GEORGIAN;
        }
        if( ( 0x2de0 <= character ) && ( character <= 0x2dff ) )
        {
          return CYRILLIC;
        }
        if( ( 0x2e80 <= character ) && ( character <= 0x2eff ) )
        {
          return CJK;
        }
        if( ( 0x2f00 <= character ) && ( character <= 0x2fdf ) )
        {
          return CJK;
        }
        if( ( 0x3000 <= character ) && ( character <= 0x303f ) )
        {
          return CJK;
        }
        if( ( 0x3040 <= character ) && ( character <= 0x309f ) )
        {
          return HIRAGANA;
        }
        if( ( 0x30a0 <= character ) && ( character <= 0x30ff ) )
        {
          return KATAKANA;
        }
        if( ( 0x3100 <= character ) && ( character <= 0x312f ) )
        {
          return BOPOMOFO;
        }
        if( ( 0x3130 <= character ) && ( character <= 0x318f ) )
        {
          return HANGUL;
        }
        if( ( 0x31a0 <= character ) && ( character <= 0x31bf ) )
        {
          return BOPOMOFO;
        }
        if( ( 0x3200 <= character ) && ( character <= 0x32ff ) )
        {
          return CJK;
        }
        if( ( 0x3400 <= character ) && ( character <= 0x4dbf ) )
        {
          return CJK;
        }
        if( ( 0x4e00 <= character ) && ( character <= 0x62ff ) )
        {
          return CJK;
        }
        if( ( 0x6300 <= character ) && ( character <= 0x77ff ) )
        {
          return CJK;
        }
        if( ( 0x7800 <= character ) && ( character <= 0x8cff ) )
        {
          return CJK;
        }
        if( ( 0x8d00 <= character ) && ( character <= 0x9fff ) )
        {
          return CJK;
        }
        if( ( 0xa640 <= character ) && ( character <= 0xa69f ) )
        {
          return CYRILLIC;
        }
        if( ( 0xa720 <= character ) && ( character <= 0xa7ff ) )
        {
          return LATIN;
        }
        if( ( 0xa960 <= character ) && ( character <= 0xa97f ) )
        {
          return HANGUL;
        }
        if( ( 0xab30 <= character ) && ( character <= 0xab6f ) )
        {
          return LATIN;
        }
        if( ( 0xac00 <= character ) && ( character <= 0xd7af ) )
        {
          return HANGUL;
        }
        if( ( 0xd7b0 <= character ) && ( character <= 0xd7ff ) )
        {
          return HANGUL;
        }
        if( ( 0xfb00 <= character ) && ( character <= 0xfb06 ) )
        {
          return LATIN;
        }
        if( ( 0xfb13 <= character ) && ( character <= 0xfb17 ) )
        {
          return ARMENIAN;
        }
        if( ( 0xfb1d <= character ) && ( character <= 0xfb4f ) )
        {
          return HEBREW;
        }
        if( ( 0xfb50 <= character ) && ( character <= 0xfdff ) )
        {
          return ARABIC;
        }
      }
      else // > 0xfdff
      {
        if( ( 0xfe70 <= character ) && ( character <= 0xfeff ) )
        {
          return ARABIC;
        }
        if( ( 0xff00 <= character ) && ( character <= 0xffef ) )
        {
          return LATIN;
        }
        if( ( 0x1ee00 <= character ) && ( character <= 0x1eeff ) )
        {
          return ARABIC;
        }
        // U+1f170 4. Enclosed characters: negative squared latin capital letter A
        // U+1f6c5 6b. Additional transport and map symbols
        if( ( 0x1f170 <= character ) && ( character <= 0x1f6c5 ) )
        {
          return EMOJI;
        }
        if( ( 0x20000 <= character ) && ( character <= 0x215ff ) )
        {
          return CJK;
        }
        if( ( 0x21600 <= character ) && ( character <= 0x230ff ) )
        {
          return CJK;
        }
        if( ( 0x23100 <= character ) && ( character <= 0x245ff ) )
        {
          return CJK;
        }
        if( ( 0x24600 <= character ) && ( character <= 0x260ff ) )
        {
          return CJK;
        }
        if( ( 0x26100 <= character ) && ( character <= 0x275ff ) )
        {
          return CJK;
        }
        if( ( 0x27600 <= character ) && ( character <= 0x290ff ) )
        {
          return CJK;
        }
        if( ( 0x29100 <= character ) && ( character <= 0x2a6df ) )
        {
          return CJK;
        }
        if( ( 0x2a700 <= character ) && ( character <= 0x2b73f ) )
        {
          return CJK;
        }
        if( ( 0x2b740 <= character ) && ( character <= 0x2b81f ) )
        {
          return CJK;
        }
      }
    }
  }

  return UNKNOWN;
}

bool IsWhiteSpace( Character character )
{
  return character < WHITE_SPACE_THRESHOLD;
}

bool IsNewParagraph( Character character )
{
  return ( ( CHAR_LF == character )  ||
           ( CHAR_VT == character )  ||
           ( CHAR_FF == character )  ||
           ( CHAR_CR == character )  ||
           ( CHAR_NEL == character ) ||
           ( CHAR_LS == character )  ||
           ( CHAR_PS == character ) );
}

bool IsZeroWidthNonJoiner( Character character )
{
  return CHAR_ZWNJ == character;
}

bool IsZeroWidthJoiner( Character character )
{
  return CHAR_ZWJ == character;
}

bool IsZeroWidthSpace( Character character )
{
  return CHAR_ZWS == character;
}

bool IsLeftToRightMark( Character character )
{
  return CHAR_LTRM == character;
}

bool IsRightToLeftMark( Character character )
{
  return CHAR_RTLM == character;
}

bool IsThinSpace( Character character )
{
  return CHAR_TS == character;
}

bool IsCommonScript( Character character )
{
  return ( IsWhiteSpace( character )         ||
           IsZeroWidthNonJoiner( character ) ||
           IsZeroWidthJoiner( character )    ||
           IsZeroWidthSpace( character )     ||
           IsLeftToRightMark( character )    ||
           IsRightToLeftMark( character )    ||
           IsThinSpace( character )          ||
           IsNewParagraph( character ) );
}

bool HasLigatureMustBreak( Script script )
{
  return ( ( LATIN == script ) ||
           ( ARABIC == script ) );
}

} // namespace TextAbstraction

} // namespace Dali
