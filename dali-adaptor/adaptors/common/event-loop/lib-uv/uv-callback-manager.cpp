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
#include "uv-callback-manager.h"

// EXTERNAL INCLUDES
#include <uv.h>
#include <dali/integration-api/debug.h>

// INTERNAL INCLUDES


namespace Dali
{

namespace Internal
{

namespace Adaptor
{

namespace
{

static void FreeHandleCallback(uv_handle_t* handle )
{
  delete handle;
}

}
/**
 * Structure contains the callback function and control options
 */
struct CallbackData
{
  typedef void (*CallbackFunction)(uv_idle_t*);

  /**
   * Constructor
   */
  CallbackData( CallbackBase* callback )
  :  mCallback(callback),
     mRemoveFromContainerFunction(NULL),
     mIdleHandle( NULL),
     mExecute(true)
  {
  }

  /**
   * Add the idle callback
   */
  void AddIdle( CallbackFunction callback)
  {
    // heap allocate a handle as it will be alive after the CallbackData object is deleted.
    mIdleHandle = new uv_idle_t;

    // Node.JS uses uv_default_loop
    uv_idle_init( uv_default_loop() , mIdleHandle );

    mIdleHandle->data = this;

    uv_idle_start( mIdleHandle, callback);
  }

  /**
   * Destructor
   */
  ~CallbackData()
  {
    // the handle will still be alive for a short period after calling uv_close
    // set the data to NULL to avoid a dangling pointer
    mIdleHandle->data = NULL;

    uv_idle_stop( mIdleHandle );

    uv_close( reinterpret_cast< uv_handle_t*>( mIdleHandle ) , FreeHandleCallback );

    delete mCallback;
    delete mRemoveFromContainerFunction;
  }

  // Data
  CallbackBase*                   mCallback;      ///< call back
  CallbackBase*                   mRemoveFromContainerFunction; ///< Called to remove the callbackdata from the callback container
  uv_idle_t*                      mIdleHandle;   ///< idle handle
  bool                            mExecute;      ///< whether to run the callback

};

namespace
{
void IdleCallback( uv_idle_t* handle )
{
  CallbackData *callbackData = static_cast<CallbackData *>(handle->data);

  // remove callback data from the container first in case our callback tries to modify the container
  CallbackBase::Execute( *callbackData->mRemoveFromContainerFunction, callbackData );

  // run the function
  CallbackBase::Execute( *callbackData->mCallback );

  // will clear up the handle
  delete callbackData;

}
}

UvCallbackManager::UvCallbackManager()
:mRunning(false)
{
}

void UvCallbackManager::Start()
{
  DALI_ASSERT_DEBUG( mRunning == false );
  mRunning = true;
}

void UvCallbackManager::Stop()
{
  // make sure we're not called twice
  DALI_ASSERT_DEBUG( mRunning == true );

  mRunning = false;

  for( CallbackList::iterator  iter =  mCallbackContainer.begin(); iter != mCallbackContainer.end(); ++iter)
  {
    CallbackData* data = (*iter);

    delete data;
  }
  mCallbackContainer.clear();
}

bool UvCallbackManager::AddIdleCallback( CallbackBase* callback )
{
  if( !mRunning )
  {
    return false;
  }

  CallbackData *callbackData = new CallbackData(callback );

  // To inform the manager a callback has finished, we get it to call RemoveCallbackFromContainer
  callbackData->mRemoveFromContainerFunction =  MakeCallback( this, &UvCallbackManager::RemoveCallbackFromContainer );

  // add the call back to the container
  mCallbackContainer.push_front(callbackData);

  // init the callback
  callbackData->AddIdle( &IdleCallback );

  return true;
}

void UvCallbackManager::RemoveCallbackFromContainer(CallbackData *callbackData)
{
  mCallbackContainer.remove(callbackData);
}

// Creates a concrete interface for CallbackManager
CallbackManager* CallbackManager::New()
{
  return new UvCallbackManager;
}

} // namespace Adaptor

} // namespace Internal

} // namespace Dali
