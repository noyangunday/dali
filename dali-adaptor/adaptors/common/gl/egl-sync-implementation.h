#ifndef __DALI_INTERNAL_ADAPTOR_EGL_SYNC_IMPLEMENTATION_H__
#define __DALI_INTERNAL_ADAPTOR_EGL_SYNC_IMPLEMENTATION_H__

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
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <dali/public-api/common/dali-common.h>
#include <dali/public-api/common/dali-vector.h>
#include <dali/integration-api/gl-sync-abstraction.h>

namespace Dali
{
namespace Internal
{
namespace Adaptor
{
class EglImplementation;

class EglSyncObject : public Integration::GlSyncAbstraction::SyncObject
{
public:
  /**
   * Constructor
   */
  EglSyncObject( EglImplementation& eglSyncImpl );

  /**
   * Destructor
   */
  virtual ~EglSyncObject();

  /**
   * @copydoc Dali::Integration::GlSyncAbstraction::SyncObject::IsSynced()
   */
  virtual bool IsSynced();

private:
#ifdef _ARCH_ARM_
  EGLSyncKHR mEglSync;
#else
  int mPollCounter; // Implementations without fence sync use a 3 frame counter
#endif
  EglImplementation& mEglImplementation;
};


/**
 * GlSyncImplementation is a concrete implementation for GlSyncAbstraction.
 * It provides fence syncing for resources such as FrameBuffers using EGL extensions
 *
 * Sync objects are created in the render thread after a render instruction
 * has been processed (i.e. GL draw calls have completed for a given FB), and
 * tested in the update
 */
class EglSyncImplementation : public Integration::GlSyncAbstraction
{
public:
  /**
   * Constructor
   */
  EglSyncImplementation();

  /**
   * Destructor
   */
  virtual ~EglSyncImplementation();

  /**
   * Initialize the sync object with the Egl implementation.
   * @param[in] impl The EGL implementation (to access display)
   */
  void Initialize( EglImplementation* impl );

  /**
   * @copydoc Dali::Integration::GlSyncAbstraction::CreateSyncObject()
   */
  virtual SyncObject* CreateSyncObject();

  /**
   * @copydoc Dali::Integration::GlSyncAbstraction::DestroySyncObject()
   */
  virtual void DestroySyncObject(SyncObject* syncObject);

private:
  /**
   * Set up the function pointers
   */
  void InitializeEglSync();

private:
  typedef Vector<EglSyncObject*>   SyncContainer;
  typedef SyncContainer::Iterator  SyncIter;

  EglImplementation* mEglImplementation; ///< Egl implementation (to get display)
  bool mSyncInitialized;        ///< Flag to perform initialization on first use
  bool mSyncInitializeFailed;   ///< Flag to avoid reloading functions if failed once

  SyncContainer mSyncObjects;
};

} // namespace Adaptor
} // namespace Internal
} // namespace Dali

#endif // __DALI_INTERNAL_EGL_ADAPTOR_SYNC_IMPLEMENTATION_H__
