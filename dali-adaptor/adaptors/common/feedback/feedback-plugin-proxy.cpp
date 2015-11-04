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
#include <feedback/feedback-plugin-proxy.h>

// EXTERNAL INCLUDES
#include <dlfcn.h>
#include <dali/integration-api/debug.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

const char * const FeedbackPluginProxy::DEFAULT_OBJECT_NAME( "libdali-feedback-plugin.so" );

FeedbackPluginProxy::FeedbackPluginProxy( const std::string& sharedObjectName )
: mInitializeAttempted( false ),
  mLibHandle( NULL ),
  mSharedObjectName( sharedObjectName ),
  mCreatePluginFunctionPtr( NULL ),
  mFeedbackPlugin( NULL )
{
  // Lazily initialize when sound/haptic is first played
}

FeedbackPluginProxy::~FeedbackPluginProxy()
{
  if( mFeedbackPlugin )
  {
    delete mFeedbackPlugin;
    mFeedbackPlugin = NULL;

    if( mLibHandle && dlclose( mLibHandle ) )
    {
      DALI_LOG_ERROR( "Error closing dali feedback plugin library: %s\n", dlerror() );
    }
  }
}

void FeedbackPluginProxy::PlayHaptic( const std::string& filePath )
{
  // Lazy initialization
  Initialize();

  if( mFeedbackPlugin )
  {
    mFeedbackPlugin->PlayHaptic( filePath );
  }
}

void FeedbackPluginProxy::PlayHapticMonotone( unsigned int duration )
{
  // Lazy initialization
  Initialize();

  if( mFeedbackPlugin )
  {
    mFeedbackPlugin->PlayHapticMonotone( duration );
  }
}

void FeedbackPluginProxy::StopHaptic()
{
  // Must already have been initialized to play haptic
  if( mFeedbackPlugin )
  {
    mFeedbackPlugin->StopHaptic();
  }
}

int FeedbackPluginProxy::PlaySound( const std::string& fileName )
{
  // Lazy initialization
  Initialize();

  if( mFeedbackPlugin )
  {
    return mFeedbackPlugin->PlaySound( fileName );
  }

  return 0;
}

void FeedbackPluginProxy::StopSound( int handle )
{
  // Must already have been initialized to play sound
  if ( mFeedbackPlugin )
  {
    mFeedbackPlugin->StopSound( handle );
  }
}

void FeedbackPluginProxy::PlayFeedbackPattern( int type, int pattern )
{
  // Lazy initialization
  Initialize();

  if( mFeedbackPlugin )
  {
    mFeedbackPlugin->PlayFeedbackPattern( type, pattern );
  }
}

void FeedbackPluginProxy::Initialize()
{
  // Only attempt to load dll once
  if ( !mInitializeAttempted )
  {
    mInitializeAttempted = true;

    mLibHandle = dlopen( mSharedObjectName.c_str(), RTLD_NOW | RTLD_GLOBAL );
    if( !mLibHandle )
    {
      DALI_LOG_ERROR( "Cannot load dali feedback plugin library error: %s\n", dlerror() );
      return;
    }

    // reset errors
    dlerror();

    // load plugin
    mCreatePluginFunctionPtr = reinterpret_cast<CreateFeedbackPlugin*>(dlsym(mLibHandle, "CreateFeedbackPlugin"));
    if(!mCreatePluginFunctionPtr)
    {
      DALI_LOG_ERROR("Cannot load symbol CreateFeedbackPlugin(): %s\n", dlerror());
      return;
    }

    // reset errors
    dlerror();

    mFeedbackPlugin = mCreatePluginFunctionPtr();

    if(!mFeedbackPlugin)
    {
      DALI_LOG_ERROR("Call to function CreateFeedbackPlugin() failed\n");
    }
  }
}

} // namespace Adaptor

} // namespace Internal

} // namespace Dali
