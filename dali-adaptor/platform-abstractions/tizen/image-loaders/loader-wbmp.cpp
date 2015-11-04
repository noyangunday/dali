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

// HEADER
#include "loader-wbmp.h"

// EXTERNAL INCLUDES
#include <cstdio>
#include <cstdlib>
#include <cstring>

// INTERNAL INCLUDES
#include <dali/integration-api/debug.h>
#include <dali/integration-api/bitmap.h>
#include <dali/public-api/common/dali-vector.h>

namespace Dali
{
using Integration::Bitmap;
using Dali::Integration::PixelBuffer;
namespace TizenPlatform
{

namespace
{

#if defined(DEBUG_ENABLED)
Debug::Filter* gLogFilter = Debug::Filter::New(Debug::NoLogging, false, "LOG_LOADER_WBMP");
#endif

#define IMG_MAX_SIZE 65536

#define IMG_TOO_BIG(w, h) \
        ((((unsigned long long)w) * ((unsigned long long)h)) >= \
           ((1ULL << (29)) - 2048))


//extract multiple bytes integer , and saved in *data
int extractMultiByteInteger(unsigned int *data, void *map, size_t length, size_t *position)
{
  // the header field contains an image type indentifier of multi-byte length(TypeField), an octet of general header info(FixHeaderField)
  //,  a multi-byte width field(Width) and a multi-byte height field(Height) and so on.
  // The actual organisation of the image data depends on the image type
  // for Ext Headers flag (7th bit), 1 = More will follow, 0 = Last octet
  // so in the for loop, if(buf & 0x80 == 0), loop will be exited
  int targetMultiByteInteger = 0, readBufCount;
  unsigned char buf;

  for (readBufCount = 0;;)
  {
    // readBufCount means the count that fetched data from map
    // extractMultiByteInteger() is to fetch wbmp type , width, and height
    // for wbmp type, when readBufCount == 1, buf = 0x00, it will exit the loop
    // for width, it have 4 bytes, so when readBufCount == 4, it must exit the loop
    // for general width and height, if(buf & 0x80) == 0, then the next byte does not need to fetch again
    // first step, readBufCount = 1 , read int(4 bytes) to buf, if buf & 0x80 !=0, the buf need to continue to fetch
    // second step, readBufCount = 2, read next( 4 bytes) to buf, if buf & 0x80 == 0, then assigned the buf to target
    if ((readBufCount ++) == 4)
    {
      return -1;
    }
    if (*position > length)
    {
      return -1;
    }
    buf = ((unsigned char *) map)[(*position)++];
    targetMultiByteInteger = (targetMultiByteInteger << 7) | (buf & 0x7f);

    if ((buf & 0x80) == 0)
    {
      DALI_LOG_INFO(gLogFilter, Debug::Verbose, "position: %d, readBufCount: %d", *position, readBufCount);
      break;
    }
  }
  *data = targetMultiByteInteger;
  return 0;
}

}// end unnamed namespace

bool LoadBitmapFromWbmp( const ResourceLoadingClient& client, const ImageLoader::Input& input, Integration::Bitmap& bitmap )
{
  FILE* const fp = input.file;
  if(fp == NULL)
  {
    DALI_LOG_ERROR("Error loading bitmap\n");
    return false;
  }
  Dali::Vector<unsigned char> map;
  Dali::Vector<unsigned char> surface;//unsigned int
  PixelBuffer* pixels = NULL;
  size_t position = 0;

  unsigned int  w, h;
  unsigned int type;
  unsigned int line_length;
  unsigned char *line = NULL;
  int cur = 0, x, y;

  if( fseek(fp,0,SEEK_END) )
  {
    DALI_LOG_ERROR("Error seeking WBMP data\n");
    return false;
  }
  long positionIndicator = ftell(fp);

  unsigned int fsize( 0u );
  if( positionIndicator > -1L )
  {
    fsize = static_cast<unsigned int>(positionIndicator);
  }

  if( 0u == fsize )
  {
    DALI_LOG_ERROR("Error: filesize is 0!\n");
    return false;
  }

  if( fseek(fp, 0, SEEK_SET) )
  {
    DALI_LOG_ERROR("Error seeking WBMP data\n");
    return false;
  }
  if(fsize <= 4)
  {
    DALI_LOG_ERROR("Error: WBMP Raw Data Not Found!\n");
    return false;
  }
  if(fsize > 4096 * 4096 * 4)
  {
    DALI_LOG_ERROR("Error: WBMP size is too large!\n");
    return false;
  }
  map.Resize(fsize);

  if(fread(&map[0], 1, fsize, fp) != fsize)
  {
    DALI_LOG_WARNING("image file read opeation error!");
    return false;
  }

  if (extractMultiByteInteger(&type, &map[0], fsize, &position) < 0)
  {
    return false;
  }

  position++; /* skipping one byte */

  if (extractMultiByteInteger(&w, &map[0], fsize, &position) < 0)
  {
    return false;
  }
  if (extractMultiByteInteger(&h, &map[0], fsize, &position) < 0)
  {
    return false;
  }
  if(type != 0)
  {
    DALI_LOG_ERROR("Unknown Format!\n");
    return false;
  }

  if ((w < 1) || (h < 1) || (w > IMG_MAX_SIZE) || (h > IMG_MAX_SIZE))
  {
    return false;
  }

  surface.Resize(w* h );//(w * h * 4);
  memset(&surface[0], 0, w * h ); // w * h * 4

  line_length = (w + 7) >> 3;
  for (y = 0; y < (int)h; y ++)
  {
    if (position + line_length > fsize)
    {
      return false;
    }
    line = &map[0] + position;
    position += line_length;
    for (x = 0; x < (int)w; x++)
    {
      int idx = x >> 3;
      int offset = 1 << (0x07 - (x & 0x07));
      if (line[idx] & offset)
      {
        surface[cur] = 0xff;//0xffffffff;
      }
      else
      {
        surface[cur] = 0x00;//0xff000000;
      }
      cur++;
    }
  }
  pixels = bitmap.GetPackedPixelsProfile()->ReserveBuffer(Pixel::L8, w, h);//Pixel::RGBA8888

  memcpy(pixels, (unsigned char*)&surface[0], w * h );//w * h * 4

  return true;
}


bool LoadWbmpHeader( const ImageLoader::Input& input, unsigned int& width, unsigned int& height )
{
  FILE* const fp = input.file;
  if(fp == NULL)
  {
    DALI_LOG_ERROR("Error loading bitmap\n");
    return false;
  }
  Dali::Vector<unsigned char> map;
  size_t position = 0;

  unsigned int  w, h;
  unsigned int type;
  if( fseek(fp,0,SEEK_END) )
  {
    DALI_LOG_ERROR("Error seeking WBMP data\n");
    return false;
  }
  long positionIndicator = ftell(fp);

  unsigned int fsize( 0u );
  if( positionIndicator > -1L )
  {
    fsize = static_cast<unsigned int>(positionIndicator);
  }

  if( 0u == fsize )
  {
    return false;
  }

  if( fseek(fp, 0, SEEK_SET) )
  {
    DALI_LOG_ERROR("Error seeking WBMP data\n");
    return false;
  }
  if(fsize <= 4)
  {
    DALI_LOG_ERROR("Error: WBMP Raw Data Not Found!\n");
    return false;
  }

  // type(1 byte) + fixedheader(1 byte) + width(uint) + height(uint)
  unsigned int headerSize = 1 + 1 + 4 + 4;// 8 + 8 + 32 + 32;
  headerSize = std::min(headerSize, fsize);

  map.Resize(headerSize);
  if(fread(&map[0], 1, headerSize, fp) != headerSize)
  {
    DALI_LOG_WARNING("image file read opeation error!");
    return false;
  }

  if (extractMultiByteInteger(&type, &map[0], fsize, &position) < 0)
  {
    DALI_LOG_ERROR("Error: unable to read type!");
    return false;
  }
  position++; /* skipping one byte */
  if(type != 0)
  {
    DALI_LOG_ERROR("Error: unknown format!\n");
    return false;
  }
  if (extractMultiByteInteger(&w, &map[0], fsize, &position) < 0)
  {
    DALI_LOG_ERROR("Error: can not read width!\n");
    return false;
  }
  if (extractMultiByteInteger(&h, &map[0], fsize, &position) < 0)
  {
    DALI_LOG_ERROR("Error: can not read height!\n");
    return false;
  }

  if ((w < 1) || (h < 1) || (w > IMG_MAX_SIZE) || (h > IMG_MAX_SIZE) )
  {
    DALI_LOG_ERROR("Error: file size is not supported!\n");
    return false;
  }

  width = w;
  height = h;
  return true;
}

}
}
