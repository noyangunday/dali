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

#define LOG_TAG "DALI_FEEDBACK"

// CLASS HEADER
#include "dali-feedback.h"

// EXTERNAL INCLUDES
#include <iostream>
#include <fstream>
#include <feedback.h>
#include <mm_sound.h>
#include <mm_sound_private.h>

#include <dlog.h>

#define DEBUG_PRINTF(fmt, arg...)  LOGD(" "fmt, ##arg)

using std::string;
using namespace Dali;

// The plugin factories
extern "C" DALI_EXPORT_API Dali::FeedbackPlugin* CreateFeedbackPlugin(void)
{
  return new Dali::Plugin::DaliFeedback;
}

namespace Dali
{

namespace Plugin
{

DaliFeedback::DaliFeedback()
: mHapticInitialized( false )
{
  feedback_initialize();

  if( 0 == haptic_open( HAPTIC_DEVICE_0, &mDeviceHandle ) )
  {
    mHapticInitialized = true;
  }
  else
  {
    DEBUG_PRINTF( "Haptic feedback controller failed to initialize\n" );
  }
}

DaliFeedback::~DaliFeedback()
{
  if( mHapticInitialized )
  {
    int errorCode = haptic_close( mDeviceHandle );
    if( errorCode < 0 )
    {
      DEBUG_PRINTF( "device_haptic_close() failed with error code: %d\n", errorCode );
    }
  }

  feedback_deinitialize();
}

void DaliFeedback::PlayHaptic( const std::string& filePath )
{
  if( mHapticInitialized )
  {
    if ( filePath.size() == 0 )
    {
      DEBUG_PRINTF( "File Path can't be NULL!\n" );
      return;
    }

    int errorCode = haptic_vibrate_file_with_detail( mDeviceHandle, filePath.c_str(), HAPTIC_ITERATION_ONCE, HAPTIC_FEEDBACK_AUTO, HAPTIC_PRIORITY_MIN, NULL );
    if( errorCode != 0 )
    {
      DEBUG_PRINTF( "PlayHaptic() failed with error code: %d\n", errorCode );
    }
  }
  else
  {
    DEBUG_PRINTF( "HapticPlayer is not Initialized\n" );
  }
}

void DaliFeedback::PlayHapticMonotone( unsigned int duration )
{
  if( mHapticInitialized )
  {
    int errorCode = haptic_vibrate_monotone_with_detail( mDeviceHandle, duration, HAPTIC_FEEDBACK_AUTO, HAPTIC_PRIORITY_MIN, NULL );
    if( errorCode != 0 )
    {
      DEBUG_PRINTF( "PlayHapticMonotone() failed with error code: %d\n", errorCode );
    }
  }
  else
  {
    DEBUG_PRINTF( "HapticPlayer is not Initialized\n" );
  }
}

void DaliFeedback::StopHaptic()
{
  if( mHapticInitialized )
  {
    int errorCode = haptic_stop_all_effects( mDeviceHandle );
    if( errorCode != 0 )
    {
      DEBUG_PRINTF( "StopHaptic() failed with error code: %d\n", errorCode );
    }
  }
  else
  {
    DEBUG_PRINTF( "HapticPlayer is not Initialized\n" );
  }
}

int DaliFeedback::PlaySound( const std::string& fileName )
{
  int handle = -1;
  int errorCode = mm_sound_play_keysound( fileName.c_str(), VOLUME_TYPE_SYSTEM & VOLUME_GAIN_TOUCH );
  if( errorCode < 0 )
  {
    DEBUG_PRINTF( "PlaySound() %s failed with error code = %d\n", fileName.c_str(), errorCode );
  }

  return handle;
}

void DaliFeedback::StopSound( int handle )
{
  int errorCode = mm_sound_stop_sound( handle );
  if( errorCode < 0 )
  {
    DEBUG_PRINTF( "StopSound() handle = %d failed with error code = %d\n", handle, errorCode);
  }
  else
  {
    DEBUG_PRINTF( "stop handle %d success\n", handle );
  }
}

void DaliFeedback::PlayFeedbackPattern( int type, int pattern )
{
  int errorCode = feedback_play_type( static_cast<feedback_type_e>(type), static_cast<feedback_pattern_e>(pattern) );
  if( errorCode != 0 )
  {
    DEBUG_PRINTF( "DaliFeedback::PlayFeedbackPattern() with type = %d, pattern = %d returned with error = %d\n", (int)type, (int)pattern, errorCode );
  }
}

} // namespace Plugin

} // namespace Dali

