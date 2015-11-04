#ifndef _DALI_INTERNAL_PLATFORM_FILECLOSER_H__
#define _DALI_INTERNAL_PLATFORM_FILECLOSER_H__
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

// EXTERNAL INCLUDES
#include <cstdio>

namespace Dali
{
namespace Internal
{
namespace Platform
{

/**
 * Opens files and closes them later even if an exception is thrown.
 */
class FileCloser
{
public:

  /**
   * @brief Construct a FileCloser guarding a new FILE* for accessing the path passed in.
   */
  FileCloser( const char * const filename, const char * const mode ) :
    mFile(fopen(filename, mode))
  {
    DALI_ASSERT_DEBUG( filename != 0 && "Cant open a null filename." );
    DALI_ASSERT_DEBUG( mode != 0 && "Null mode is undefined behaviour in spec." );

    if( mFile == 0 )
    {
      DALI_LOG_WARNING( "File open failed for: \"%s\" in mode: \"%s\".\n", filename, mode );
    }
  }

  /**
   * @brief Construct a FileCloser guarding a FILE* for reading out of the memory buffer passed in.
   */
  FileCloser( void * const buffer, const size_t bufferSize, const char * const mode ) :
    mFile( fmemopen( buffer, bufferSize, mode ) )
  {
    DALI_ASSERT_DEBUG( buffer != 0 && "Cant open file on null buffer." );
    DALI_ASSERT_DEBUG( bufferSize > 0 && "Pointless to open file on empty buffer." );
    DALI_ASSERT_DEBUG( mode != 0 && "Null mode is undefined behaviour in spec." );

    if( mFile == 0 )
    {
      DALI_LOG_WARNING( "File open failed for memory buffer at location: \"%p\", of size: \"%u\", in mode: \"%s\".\n", static_cast<void*>(buffer), static_cast<unsigned>(bufferSize), mode );
    }
  }

   /**
    * @brief Destroy the FileCloser and clean up its FILE*.
    */
  ~FileCloser()
  {
    if( mFile != 0 )
    {
      const int closeFailed = fclose( mFile );

      if ( closeFailed )
      {
        DALI_LOG_WARNING( "File close failed for FILE: \"%p\".\n", static_cast<void*>(mFile) );
      }
      mFile = 0;
    }
  }

  /**
   * @return The FILE* guarded by this object.
   */
  FILE* GetFile()
  {
    return mFile;
  }

private:

  // Undefined
  FileCloser( const FileCloser& fileCloser );

  // Undefined
  FileCloser& operator=( const FileCloser& fileCloser );

private:
  FILE* mFile;
};

} /* namespace Platform */
} /* namespace Internal */
} /* namespace Dali */

#endif /* _DALI_INTERNAL_PLATFORM_FILECLOSER_H__ */
