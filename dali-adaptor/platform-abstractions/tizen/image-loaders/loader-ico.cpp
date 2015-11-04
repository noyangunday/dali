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
 */

/*
 * Derived from Enlightenment file evas_image_load_ico.c[1]  which is licensed
 * under the BSD 2-clause license[2] reproduced below.
 *
 * [1][http://web.archive.org/web/20141201151111/http://git.enlightenment.org/core/efl.git/tree/src/modules/evas/loaders/ico/evas_image_load_ico.c]
 * [2][http://web.archive.org/web/20140717012400/https://git.enlightenment.org/core/efl.git/about/]
 *
 * Copyright (C) 2002-2012 Carsten Haitzler, Dan Sinclair, Mike Blumenkrantz,
 * Samsung Electronics and various contributors (see AUTHORS)
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *    1. Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *    2. Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// HEADER
#include "loader-ico.h"

// EXTERNAL INCLUDES
#include <cstring>
#include <dali/public-api/common/dali-vector.h>

// INTERNAL INCLUDES
#include <dali/integration-api/debug.h>
#include <dali/integration-api/bitmap.h>

namespace Dali
{
using Integration::Bitmap;
using Dali::Integration::PixelBuffer;

namespace TizenPlatform
{

namespace
{
//reserved 2 bytes + type 2 bytes + count 2 bytes + count * 16 bytes
const unsigned char ICO_FILE_HEADER = 22;
// info header 40 bytes = size 4 bytes + width 4 bytes + height 4 bytes + planes 2 bytes + bitcount 2 bytes
// + compression 4 bytes + imagesize 4 bytes + xpixelsPerM 4 bytes + ypixelsPerM 4 bytes + colorsUsed 4 bytes + colorImportant 4 bytes
// besides, there are rgba color data = numberOfColors * 4 bytes
const unsigned char ICO_IMAGE_INFO_HEADER = 40;

typedef unsigned char  DATA8;
#define A_VAL(p) (((DATA8 *)(p))[3])

#define RGB_JOIN(r,g,b) \
                (((r) << 16) + ((g) << 8) + (b))

#define ARGB_JOIN(a,r,g,b) \
                (((a) << 24) + ((r) << 16) + ((g) << 8) + (b))
#define IMG_TOO_BIG(w, h) \
       ((((unsigned long long)w) * ((unsigned long long)h)) >= \
           ((1ULL << (29 * (sizeof(void *) / 4))) - 2048))

bool read_ushort(unsigned char *map, size_t length, size_t *position, unsigned short *ret)
{
  unsigned char b[2];

  if (*position + 2 > length)
  {
    return false;
  }
  b[0] = map[(*position)++];
  b[1] = map[(*position)++];
  *ret = (b[1] << 8) | b[0];
  return true;
}

bool read_uint(unsigned char *map, size_t length, size_t *position, unsigned int *ret)
{
  unsigned char b[4];
  unsigned int i;

  if (*position + 4 > length)
  {
    return false;
  }
  for (i = 0; i < 4; i++)
  {
    b[i] = map[(*position)++];
  }
  *ret = ARGB_JOIN(b[3], b[2], b[1], b[0]);
  return true;
}

bool read_uchar(unsigned char *map, size_t length, size_t *position, unsigned char *ret)
{
  if (*position + 1 > length)
  {
    return false;
  }
  *ret = map[(*position)++];
  return true;
}

bool read_mem(unsigned char *map, size_t length, size_t *position, void *buffer, int size)
{
  if (*position + size > length)
  {
    return false;
  }
  memcpy(buffer, map + *position, size);
  *position += size;
  return true;
}

enum
{
  SMALLEST,
  BIGGEST,
  SMALLER,
  BIGGER
};

enum
{
  ICON = 1,
  CURSOR = 2
};

struct IcoData
{
  int pdelta;
  int w, h;
  int cols;
  int bpp, planes;
  int hot_x, hot_y;
  unsigned int bmoffset, bmsize;
};

bool LoadIcoHeaderHelper( FILE* fp,
                          IcoData& chosen,
                          Dali::Vector<unsigned char>& map,
                          unsigned int& fsize )
{
  memset( &chosen, 0, sizeof(chosen) );

  if(fp == NULL)
  {
    DALI_LOG_ERROR("Error loading bitmap\n");
    return false;
  }
  size_t position = 0;
  unsigned short word;
  unsigned char byte;

  if( fseek(fp,0,SEEK_END) )
  {
    DALI_LOG_ERROR("Error seeking ICO data\n");
    return false;
  }

  long positionIndicator = ftell(fp);
  fsize = 0u;

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
    DALI_LOG_ERROR("Error seeking ICO data\n");
    return false;
  }

  if (fsize < (ICO_FILE_HEADER + ICO_IMAGE_INFO_HEADER)) //6 + 16 + 40
  {
    return false;
  }
  map.Resize(fsize);

  if(fread(&map[0], 1, fsize, fp) != fsize)
  {
    DALI_LOG_WARNING("image file read opeation error!");
    return false;
  }

  int search = BIGGEST;
  unsigned short reserved, type, count;
  if (!read_ushort(&map[0], fsize, &position, &reserved))
  {
    return false;
  }
  if (!read_ushort(&map[0], fsize, &position, &type))
  {
    return false;
  }
  if (!read_ushort(&map[0], fsize, &position, &count))
  {
    return false;
  }
  if (!((reserved == 0) &&
       ((type == ICON) || (type == CURSOR)) && (count != 0)))
  {
    return false;
  }
  search = BIGGEST;
  chosen.pdelta = 0;
  bool have_choice = false;

  for (unsigned short i = 0; i < count; i++)
  {
    unsigned char tw = 0, th = 0, tcols = 0;
    if (!read_uchar(&map[0], fsize, &position, &tw))
    {
      return false;
    }
    int w = tw;
    if (w <= 0)
    {
      w = 256;
    }
    if (!read_uchar(&map[0], fsize, &position, &th))
    {
      return false;

    }
    int h = th;
    if (h <= 0)
    {
      h = 256;
    }
    if (!read_uchar(&map[0], fsize, &position, &tcols))
    {
      return false;
    }
    int cols = tcols;
    if (cols <= 0)
    {
      cols = 256;
    }
    if (!read_uchar(&map[0], fsize, &position, &byte))
    {
      return false;
    }
    if (!read_ushort(&map[0], fsize, &position, &word))
    {
      return false;
    }
    int planes=0;
    if (type == 1)
    {
      planes = word;
    }
    //else hot_x = word;
    if (!read_ushort(&map[0], fsize, &position, &word))
    {
      return false;
    }
    int bpp=0;
    if (type == 1)
    {
      bpp = word;
    }
    //else hot_y = word;
    unsigned int bmoffset, bmsize;
    if (!read_uint(&map[0], fsize, &position, &bmsize))
    {
      return false;
    }
    if (!read_uint(&map[0], fsize, &position, &bmoffset))
    {
      return false;
    }
    if ((bmsize <= 0) || (bmoffset <= 0) || (bmoffset >= fsize))
    {
      return false;
    }
    if (search == BIGGEST)
    {
      int pdelta = w * h;
      if ((!have_choice) ||
       ((pdelta >= chosen.pdelta) &&
           (((bpp >= 3) && (bpp >= chosen.bpp)) ||
               ((bpp < 3) && (cols >= chosen.cols)))))
      {
        have_choice = true;
        chosen.pdelta = pdelta;
        chosen.w = w;
        chosen.h = h;
        chosen.cols = cols;
        chosen.bpp = bpp;
        chosen.planes = planes;
        chosen.bmsize = bmsize;
        chosen.bmoffset = bmoffset;
      }
    }
  }

  if (chosen.bmoffset == 0)
  {
    return false;
  }

  return true;
}

}//unnamed namespace

bool LoadIcoHeader( const ImageLoader::Input& input, unsigned int& width, unsigned int& height )
{
  IcoData chosen;
  Dali::Vector<unsigned char> map;
  unsigned int fsize;
  FILE* const fp = input.file;

  if ( false == LoadIcoHeaderHelper(fp, chosen, map, fsize) )
  {
    return false;
  }

  width = chosen.w;
  height = chosen.h;

  return true;
}

bool LoadBitmapFromIco( const ResourceLoadingClient& client, const ImageLoader::Input& input, Integration::Bitmap& bitmap )
{
  IcoData chosen;
  Dali::Vector<unsigned char> map;
  unsigned int fsize;
  FILE* const fp = input.file;

  if ( false == LoadIcoHeaderHelper(fp, chosen, map, fsize) )
  {
    return false;
  }

  Dali::Vector<unsigned int> pal;
  Dali::Vector<unsigned int> surface;
  Dali::Vector<unsigned char> maskbuf;
  Dali::Vector<unsigned char> pixbuf;
  pal.Resize(256 * 4);

  unsigned int dword;
  unsigned short word;

  int diff_size = 0;
  unsigned int* pix;
  PixelBuffer* pixels = NULL;

  size_t position = chosen.bmoffset;//22 == position

  int w = chosen.w;
  int h = chosen.h;
  int cols = chosen.cols;

  // read bmp header time... let's do some checking
  if (!read_uint(&map[0], fsize, &position, &dword))
  {
    return false; // headersize - dont care
  }
  if (!read_uint(&map[0], fsize, &position, &dword))
  {
    return false; // width
  }
  if (dword > 0)
  {
    if ((int)dword != w)
    {
      w = dword;
      diff_size = 1;
    }
  }
  if (!read_uint(&map[0], fsize, &position, &dword))
  {
    return false; // height
  }
  if (dword > 0)
  {
    if ((int)dword != (h * 2))
    {
      h = dword / 2;
      diff_size = 1;
    }
  }
  if (diff_size)
  {
    DALI_LOG_WARNING("Broken ICO file!");
  }
  if (!read_ushort(&map[0], fsize, &position, &word))
  {
    return false; // planes
  }
  //planes2 = word;
  if (!read_ushort(&map[0], fsize, &position, &word))
  {
    return false; // bitcount
  }
  unsigned int bitcount = word;
  if (!read_uint(&map[0], fsize, &position, &dword))
  {
    return false; // compression
  }
  //compression = dword;
  if (!read_uint(&map[0], fsize, &position, &dword))
  {
    return false; // imagesize
  }
  //imagesize = dword;
  if (!read_uint(&map[0], fsize, &position, &dword))
  {
    return false; // z pixels per m
  }
  if (!read_uint(&map[0], fsize, &position, &dword))
  {
    return false; // y pizels per m
  }
  if (!read_uint(&map[0], fsize, &position, &dword))
  {
    return false; // colors used
  }
  //colorsused = dword;
  if (!read_uint(&map[0], fsize, &position, &dword))
  {
    return false; // colors important
  }
  //colorsimportant = dword;
  surface.Resize(w * h * 4);

  memset(&surface[0], 0, w * h * 4);

  for(int i = 0; i < cols ; i ++)
  {
    unsigned char a, r, g, b;

    if (!read_uchar(&map[0], fsize, &position, &b))
    {
      return false;
    }
    if (!read_uchar(&map[0], fsize, &position, &g))
    {
      return false;
    }
    if (!read_uchar(&map[0], fsize, &position, &r))
    {
      return false;
    }
    if (!read_uchar(&map[0], fsize, &position, &a))
    {
      return false;
    }
    a = 0xff;
    pal[i] = ARGB_JOIN(a, r, g, b);
  }

  if (!((bitcount == 1) || (bitcount == 4) || (bitcount == 8) ||
       (bitcount == 24) || (bitcount == 32)))
  {
    return false;
  }
  int stride = ((w + 31) / 32);

  maskbuf.Resize(stride * h);
  pixbuf.Resize(stride * 32 * 4); // more than enough

  unsigned int none_zero_alpha = 0;
  if (bitcount == 1)
  {
    int pstride = stride * 4;
    for (int i = 0; i < h; i++)
    {
      pix = &surface[0] + ((h - 1 - i) * w);

      if (!read_mem(&map[0], fsize, &position, &pixbuf[0], pstride))
      {
        return false;
      }
      unsigned char* p = &pixbuf[0];
      if (i >= (int)h)
      {
        continue;
      }
      for (int j = 0; j < w; j++)
      {
        if (j >= (int)w)
        {
          break;
        }
        if ((j & 0x7) == 0x0)
        {
          *pix = pal[*p >> 7];
        }
        else if ((j & 0x7) == 0x1)
        {
          *pix = pal[(*p >> 6) & 0x1];
        }
        else if ((j & 0x7) == 0x2)
        {
          *pix = pal[(*p >> 5) & 0x1];
        }
        else if ((j & 0x7) == 0x3)
        {
          *pix = pal[(*p >> 4) & 0x1];
        }
        else if ((j & 0x7) == 0x4)
        {
          *pix = pal[(*p >> 3) & 0x1];
        }
        else if ((j & 0x7) == 0x5)
        {
          *pix = pal[(*p >> 2) & 0x1];
        }
        else if ((j & 0x7) == 0x6)
        {
          *pix = pal[(*p >> 1) & 0x1];
        }
        else
        {
          *pix = pal[*p & 0x1];
          p++;
        }
        pix++;
      }
    }
  }
  else if (bitcount == 4)
  {
    int pstride = ((w + 7) / 8) * 4;
    for (int i = 0; i < h; i++)
    {
      pix = &surface[0] + ((h - 1 - i) * w);

      if (!read_mem(&map[0], fsize, &position, &pixbuf[0], pstride))
      {
        return false;
      }
      unsigned char* p = &pixbuf[0];
      if (i >= (int)h)
      {
        continue;
      }
      for (int j = 0; j < w; j++)
      {
        if (j >= (int)w)
        {
          break;
        }
        if ((j & 0x1) == 0x1)
        {
          *pix = pal[*p & 0x0f];
          p++;
        }
        else
        {
          *pix = pal[*p >> 4];
        }
        pix++;
      }
    }
  }
  else if (bitcount == 8)
  {
    int pstride = ((w + 3) / 4) * 4;
    for (int i = 0; i < h; i++)
    {
      pix = &surface[0] + ((h - 1 - i) * w);

      if (!read_mem(&map[0], fsize, &position, &pixbuf[0], pstride))
      {
        return false;
      }
      unsigned char* p = &pixbuf[0];
      if (i >= (int)h)
      {
        continue;
      }
      for (int j = 0; j < w; j++)
      {
        if (j >= (int)w)
        {
          break;
        }
        *pix = pal[*p];
        p++;
        pix++;
      }
    }
  }
  else if (bitcount == 24)
  {
    int pstride = w * 3;
    for (int i = 0; i < h; i++)
    {
      pix = &surface[0] + ((h - 1 - i) * w);

      if (!read_mem(&map[0], fsize, &position, &pixbuf[0], pstride))
      {
        return false;
      }
      unsigned char* p = &pixbuf[0];
      if (i >= (int)h)
      {
        continue;
      }
      for (int j = 0; j < w; j++)
      {
        unsigned char a, r, g, b;

        if (j >= (int)w)
        {
          break;
        }
        b = p[0];
        g = p[1];
        r = p[2];
        p += 3;
        a = 0xff;
        *pix = ARGB_JOIN(a, r, g, b);
        pix++;
      }
    }
  }
  else if (bitcount == 32)
  {
    int pstride = w * 4;
    for (int i = 0; i < h; i++)
    {
      pix = &surface[0] + ((h - 1 - i) * w);

      if (!read_mem(&map[0], fsize, &position, &pixbuf[0], pstride))
      {
        return false;
      }
      unsigned char* p = &pixbuf[0];
      if (i >= (int)h)
      {
        continue;
      }
      for (int j = 0; j < w; j++)
      {
        unsigned char a, r, g, b;
        if (j >= (int)w)
        {
          break;
        }
        b = p[0];
        g = p[1];
        r = p[2];
        a = p[3];
        p += 4;
        if (a)
        {
          none_zero_alpha = 1;
        }
        *pix = ARGB_JOIN(a, r, g, b);
        pix++;
      }
    }
  }
  if (!none_zero_alpha)
  {
    if (!read_mem(&map[0], fsize, &position, &maskbuf[0], stride * 4 * h))
    {
      return false;
    }
    // apply mask
    for (int i = 0; i < h; i++)
    {
      unsigned char *m;

      pix = &surface[0] + ((h - 1 - i) * w);

      m = &maskbuf[0] + (stride * i * 4);
      if (i >= (int)h)
      {
        continue;
      }
      for (int j = 0; j < w; j++)
      {
        if (j >= (int)w)
        {
          break;
        }
        if (*m & (1 << (7 - (j & 0x7))))
        {
          A_VAL(pix) = 0x00;
        }
        else
        {
          A_VAL(pix) = 0xff;
        }
        if ((j & 0x7) == 0x7)
        {
          m++;
        }
        pix++;
      }
    }
  }
  pixels = bitmap.GetPackedPixelsProfile()->ReserveBuffer(Pixel::RGBA8888, w, h);
  memset(pixels, 0, w * h * 4);
  memcpy(pixels, (unsigned char*)&surface[0], w * h * 4);

  return true;
}

}

}
