#ifndef __DALI_INTERNAL_ADAPTOR_EGL_FACTORY_IMPL_H__
#define __DALI_INTERNAL_ADAPTOR_EGL_FACTORY_IMPL_H__

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

// INTERNAL INCLUDES
#include <base/interfaces/egl-factory-interface.h>

namespace Dali
{
namespace Internal
{
namespace Adaptor
{
class EglImplementation;
class EglImageExtensions;
class EglSyncImplementation;

class EglFactory : public EglFactoryInterface
{
public:
  /**
   * Constructor
   */
  EglFactory();

  /**
   * Destructor
   */
  virtual ~EglFactory();

  /**
   * Create an EGL Implementation
   * @return[in] An implementation
   */
  EglInterface* Create();

  /**
   * Destroy the EGL Implementation
   */
  void Destroy();

  /**
   * Get an implementation if one has been created.
   * @return An implementation, or NULL if one has not yet been created.
   */
  EglInterface* GetImplementation();

  /**
   * Get the image extension
   */
  EglImageExtensions* GetImageExtensions();

  /**
   * Get the fence sync implementation
   * @return An implementation of fence sync
   */
  EglSyncImplementation* GetSyncImplementation();

private:
  /** Undefined */
  EglFactory(const EglFactory& rhs);
  EglFactory& operator=(const EglFactory& rhs);

private:
  EglImplementation* mEglImplementation;
  EglImageExtensions* mEglImageExtensions;
  EglSyncImplementation* mEglSync;
};

}
}
}

#endif //__DALI_INTERNAL_ADAPTOR_EGL_FACTORY_IMPL_H__
