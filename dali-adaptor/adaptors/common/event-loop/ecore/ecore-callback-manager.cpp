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
#include "ecore-callback-manager.h"

// EXTERNAL INCLUDES
#include <Ecore.h>

#include <dali/integration-api/debug.h>

// INTERNAL INCLUDES


namespace Dali
{

namespace Internal
{

namespace Adaptor
{

/**
 * Structure contains the callback function and control options
 */
struct CallbackData
{

  /**
   * Constructor
   */
  CallbackData( CallbackBase* callback )
  :  mCallback(callback),
     mRemoveFromContainerFunction(NULL),
     mIdler(NULL)
  {
  }
  /**
   * Destructor
   */
  ~CallbackData()
  {
    delete mCallback;
    delete mRemoveFromContainerFunction;
  }

  CallbackBase*                   mCallback;      ///< call back
  CallbackBase*                   mRemoveFromContainerFunction; ///< Called to remove the callbackdata from the callback container
  Ecore_Idler*                    mIdler;         ///< ecore idler
 };

namespace
{

/**
 * Called from the main thread while idle.
 */
Eina_Bool IdleCallback(void *data)
{
  CallbackData *callbackData = static_cast< CallbackData * >( data );

  // remove callback data from the container
  CallbackBase::Execute( *callbackData->mRemoveFromContainerFunction, callbackData );

  // run the function
  CallbackBase::Execute( *callbackData->mCallback );

  // delete our data
  delete callbackData;

  // CALLBACK Cancel will delete the idler so we don't need to call ecore_idler_del
  return ECORE_CALLBACK_CANCEL;
}

} // unnamed namespace

EcoreCallbackManager::EcoreCallbackManager()
:mRunning(false)
{
}


void EcoreCallbackManager::Start()
{
  DALI_ASSERT_DEBUG( mRunning == false );

  mRunning = true;
}

void EcoreCallbackManager::Stop()
{
  // make sure we're not called twice
  DALI_ASSERT_DEBUG( mRunning == true );

  RemoveAllCallbacks();

  mRunning = false;

}

bool EcoreCallbackManager::AddIdleCallback( CallbackBase* callback )
{
  if( !mRunning )
  {
    return false;
  }

  CallbackData *callbackData = new CallbackData( callback );

  callbackData->mRemoveFromContainerFunction =  MakeCallback( this, &EcoreCallbackManager::RemoveCallbackFromContainer );

  // add the call back to the container
  mCallbackContainer.push_front(callbackData);

  // add the idler
  callbackData->mIdler = ecore_idler_add( IdleCallback, callbackData);

  DALI_ASSERT_ALWAYS( ( callbackData->mIdler != NULL ) && "Idle method not created" );

  return true;
}

void EcoreCallbackManager::RemoveCallbackFromContainer(CallbackData *callbackData)
{
  mCallbackContainer.remove(callbackData);
}

void EcoreCallbackManager::RemoveAllCallbacks()
{
  // always called from main thread
  for( CallbackList::iterator  iter =  mCallbackContainer.begin(); iter != mCallbackContainer.end(); ++iter)
  {
    CallbackData* data = (*iter);

    ecore_idler_del( data->mIdler );

    delete data;

  }
  mCallbackContainer.clear();
}

// Creates a concrete interface for CallbackManager
CallbackManager* CallbackManager::New()
{
  return new EcoreCallbackManager;
}

} // namespace Adaptor

} // namespace Internal

} // namespace Dali
