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

// INTERNAL INCLUDES
#include <dali/integration-api/debug.h>
#include <memory.h>

namespace Dali
{
namespace TizenPlatform
{
namespace DataCompression
{

std::size_t GetMaximumRleCompressedSize(const std::size_t inputLength)
{
  // RLE has worst case scenerio of double the input data
  // e.g. if data is 1,2,3,4  = 4 bytes
  // it will be encoded as 1,1, 1,2 1,3 1,4 = 8 bytes

  // we also encode the original size into the stream to check
  // the decode buffers are big enough and for corruption
  return (inputLength * 2) + 4;  // 4 bytes is space for size

}

// Run length encode a byte stream, consisting of byte values.
// Format is one byte for run-length, one byte value.
// e.g. 10, 15, 20, 20, 20, 5, 5
// is represented as :
// 1,10
// 1,15
// 3,20
// 2, 5
// First 4 bytes are the size of the decoded data
//
void EncodeRle( const unsigned char* input,
                const std::size_t inputLength,
                unsigned char* output,
                const std::size_t outputLength,
                std::size_t& encodedSize)
{
  DALI_ASSERT_DEBUG( outputLength >= GetMaximumRleCompressedSize( inputLength ));

  unsigned int index(0);
  unsigned int runLength(0);
  encodedSize = 0;

  // encode the input length in the first 4 bytes.
  output[ encodedSize++ ] =  inputLength & 0xFF;
  output[ encodedSize++ ] = (inputLength >> 8) & 0xFF;
  output[ encodedSize++ ] = (inputLength >> 16) & 0xFF;
  output[ encodedSize++ ] = (inputLength >> 24) & 0xFF;

  while( index < inputLength  )
  {
    unsigned char curChar = input[ index ];
    runLength = 1;

    if( ( (index + 1) == inputLength )         // is more data available
        || input[index + 1] != curChar  )      // character doesn't match
    {
      // we out of data, or the next character doesn't match (run of zero)
      index++;
    }
    else
    {
      while( ( (index+1) < inputLength ) &&
               ( input[index + 1] == curChar ) &&
               ( runLength < 0xFF ) )
      {
        runLength++;
        index++;
      }
      index++;
    }
    output[ encodedSize++ ] = runLength;
    output[ encodedSize++ ] = curChar;

  }
}

bool DecodeRle( const unsigned char* input,
                const std::size_t inputLength,
                unsigned char* output,
                const std::size_t outputLength,
                std::size_t& decodedSize)
{
  unsigned int index(0);
  unsigned int outputIndex(0);

  // there should be at least 4 bytes for the size field
  if( inputLength < 4)
  {
    DALI_LOG_ERROR("input buffer too small\n");
    return false;
  }

  decodedSize = input[ index++ ] ;
  decodedSize|= input[ index++ ]<<8 ;
  decodedSize|= input[ index++ ]<<16 ;
  decodedSize|= input[ index++ ]<<24 ;

  // check the decoded data will fit in to
  if( outputLength < decodedSize )
  {
    DALI_LOG_ERROR("buffer too small, buffer size =%d, data size = %d \n",outputLength, decodedSize);
    return false;
  }

  while( (index+1)< inputLength )
  {
    // read the value and the run length
    unsigned char runLength =  input[ index++ ];
    unsigned char value = input[ index++ ];

    if( (runLength + outputIndex) > decodedSize)
    {
      DALI_LOG_ERROR( "corrupted RLE data" );
      // corrupted
      return false;
    }
    // set the value run Length times
    memset( &output[ outputIndex ], value, runLength * sizeof( unsigned char) );
    outputIndex+= runLength;
  }
  if( outputIndex != decodedSize)
  {
    DALI_LOG_ERROR(" RLE data size missmatch");
    return false;
  }

  return true;
}

} // DataCompression

} // namespace TizenPlatform

} // namespace Dali
