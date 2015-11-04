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
#include <gl/egl-sync-implementation.h>

// EXTERNAL INCLUDES

#ifdef _ARCH_ARM_

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/eglext.h>

#endif

#include <dali/integration-api/debug.h>

// INTERNAL INCLUDES
#include <gl/egl-implementation.h>

#ifdef _ARCH_ARM_

// function pointers
static PFNEGLCREATESYNCKHRPROC     eglCreateSyncKHR = NULL;
static PFNEGLCLIENTWAITSYNCKHRPROC eglClientWaitSyncKHR = NULL;
static PFNEGLDESTROYSYNCKHRPROC    eglDestroySyncKHR = NULL;

#endif

namespace Dali
{
namespace Internal
{
namespace Adaptor
{

#ifdef _ARCH_ARM_

EglSyncObject::EglSyncObject( EglImplementation& eglSyncImpl )
: mEglSync(NULL),
  mEglImplementation(eglSyncImpl)
{
  EGLDisplay display = mEglImplementation.GetDisplay();
  mEglSync = eglCreateSyncKHR( display, EGL_SYNC_FENCE_KHR, NULL );
  if (mEglSync == EGL_NO_SYNC_KHR)
  {
    DALI_LOG_ERROR("eglCreateSyncKHR failed %#0.4x\n", eglGetError());
    mEglSync = NULL;
  }
}

EglSyncObject::~EglSyncObject()
{
  if( mEglSync != NULL )
  {
    eglDestroySyncKHR( mEglImplementation.GetDisplay(), mEglSync );
    EGLint error = eglGetError();
    if( EGL_SUCCESS != error )
    {
      DALI_LOG_ERROR("eglDestroySyncKHR failed %#0.4x\n", error);
    }
  }
}

bool EglSyncObject::IsSynced()
{
  bool synced = false;

  if( mEglSync != NULL )
  {
    EGLint result = eglClientWaitSyncKHR( mEglImplementation.GetDisplay(), mEglSync, 0, 0ull );
    EGLint error = eglGetError();
    if( EGL_SUCCESS != error )
    {
      DALI_LOG_ERROR("eglClientWaitSyncKHR failed %#0.4x\n", error);
    }
    else if( result == EGL_CONDITION_SATISFIED_KHR )
    {
      synced = true;
    }
  }

  return synced;
}

EglSyncImplementation::EglSyncImplementation()
: mEglImplementation( NULL ),
  mSyncInitialized( false ),
  mSyncInitializeFailed( false )
{
}

EglSyncImplementation::~EglSyncImplementation()
{
}

void EglSyncImplementation::Initialize( EglImplementation* eglImpl )
{
  mEglImplementation = eglImpl;
}

Integration::GlSyncAbstraction::SyncObject* EglSyncImplementation::CreateSyncObject()
{
  DALI_ASSERT_ALWAYS( mEglImplementation && "Sync Implementation not initialized" );
  if( mSyncInitialized == false )
  {
    InitializeEglSync();
  }

  EglSyncObject* syncObject = new EglSyncObject(*mEglImplementation);
  mSyncObjects.PushBack( syncObject );
  return syncObject;
}

void EglSyncImplementation::DestroySyncObject( Integration::GlSyncAbstraction::SyncObject* syncObject )
{
  DALI_ASSERT_ALWAYS( mEglImplementation && "Sync Implementation not initialized" );

  if( mSyncInitialized == false )
  {
    InitializeEglSync();
  }

  for( SyncIter iter=mSyncObjects.Begin(), end=mSyncObjects.End(); iter != end; ++iter )
  {
    if( *iter == syncObject )
    {
      mSyncObjects.Erase(iter);
      break;
    }
  }
  EglSyncObject* eglSyncObject = static_cast<EglSyncObject*>(syncObject);
  delete eglSyncObject;
}

void EglSyncImplementation::InitializeEglSync()
{
  if( ! mSyncInitializeFailed )
  {
    eglCreateSyncKHR = (PFNEGLCREATESYNCKHRPROC)eglGetProcAddress("eglCreateSyncKHR");
    eglClientWaitSyncKHR = (PFNEGLCLIENTWAITSYNCKHRPROC)eglGetProcAddress("eglClientWaitSyncKHR");
    eglDestroySyncKHR = (PFNEGLDESTROYSYNCKHRPROC)eglGetProcAddress("eglDestroySyncKHR");
  }

  if( eglCreateSyncKHR && eglClientWaitSyncKHR && eglDestroySyncKHR )
  {
    mSyncInitialized = true;
  }
  else
  {
    mSyncInitializeFailed = true;
  }
}

#else

EglSyncObject::EglSyncObject( EglImplementation& eglImpl )
: mPollCounter(3),
  mEglImplementation(eglImpl)
{
}

EglSyncObject::~EglSyncObject()
{
}

bool EglSyncObject::IsSynced()
{
  if(mPollCounter <= 0)
  {
    return true;
  }
  --mPollCounter;
  return false;
}

EglSyncImplementation::EglSyncImplementation()
: mEglImplementation( NULL ),
  mSyncInitialized( false ),
  mSyncInitializeFailed( false )
{
}

EglSyncImplementation::~EglSyncImplementation()
{
}

void EglSyncImplementation::Initialize( EglImplementation* eglImpl )
{
  mEglImplementation = eglImpl;
}

Integration::GlSyncAbstraction::SyncObject* EglSyncImplementation::CreateSyncObject()
{
  DALI_ASSERT_ALWAYS( mEglImplementation && "Sync Implementation not initialized" );
  return new EglSyncObject(*mEglImplementation);
}

void EglSyncImplementation::DestroySyncObject(Integration::GlSyncAbstraction::SyncObject* syncObject)
{
  DALI_ASSERT_ALWAYS( mEglImplementation && "Sync Implementation not initialized" );

  // The abstraction's virtual destructor is protected, so that Core can't delete the sync objects
  // directly (This object also needs removing from the mSyncObject container in the ARM
  // implementation above). We therefore need to cast to the actual implementation object first.
  EglSyncObject* eglSyncObject = static_cast<EglSyncObject*>(syncObject);
  delete eglSyncObject;
}

void EglSyncImplementation::InitializeEglSync()
{
}

#endif

} // namespace Dali
} // namespace Internal
} // namespace Adaptor
