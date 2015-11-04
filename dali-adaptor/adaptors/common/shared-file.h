#ifndef __DALI_INTERNAL_ADAPTOR_SHARED_FILE_H__
#define __DALI_INTERNAL_ADAPTOR_SHARED_FILE_H__

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

// EXTERNAL INCLUDES
#include <string>

namespace Dali
{
namespace Internal
{
namespace Adaptor
{

class SharedFile
{
public:
  /**
   * Open an existing shared file for read/write
   * @return The shared file, or NULL if a file could not be opened and mapped.
   */
  static SharedFile* New( const char* filename, int size, bool isSystem );

  /**
   * Constructor
   */
  SharedFile();

  /**
   * Destructor
   */
  virtual ~SharedFile();

  /**
   * Opens a file for read/write
   * @return true if opened, false on error.
   */
  bool OpenFile( const char* filename, int size, bool isSystem );

  /**
   * Close the shared file
   */
  void Close();

  /**
   * Get the memory address of the shared file
   * @return the memory address
   */
  unsigned char* GetAddress();

private:
  int         mFileDescriptor;
  int         mSize;
  void*       mAddress;
  std::string mFilename;
};

} // Adaptor
} // Internal
} // Dali

#endif // __DALI_INTERNAL_ADAPTOR_SHARED_FILE_H__
