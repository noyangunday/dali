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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <vconf.h>
#include <vconf-keys.h>

#include <dali/integration-api/debug.h>

// INTERNAL INCLUDES
#include "vsync-monitor.h"

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

namespace
{
// constants to keep code readability with unsigned int has to be used as boolean (due to multithreading)
const unsigned int TRUE = 1u;
const unsigned int FALSE = 0u;

#if defined(DEBUG_ENABLED)
Debug::Filter* gLogFilter = Debug::Filter::New(Debug::NoLogging, false, "LOG_VSYNC_MONITOR");
#endif

const char * const DRM_DEVICE( "/dev/dri/card0" );
const int FD_NONE( -1 );

void ScreenStatusChanged(keynode_t* node, void* data)
{
  VSyncMonitor* vsyncMonitor( static_cast< VSyncMonitor* >( data ) );

  int status = 0;
  vconf_get_int( VCONFKEY_PM_STATE, &status );

  // status values
  //  - VCONFKEY_PM_STATE_NORMAL : turn vsync on
  //  - VCONFKEY_PM_STATE_LCDDIM : turn vsync off
  //  - VCONFKEY_PM_STATE_LCDOFF : turn vsync off
  //  - VCONFKEY_PM_STATE_SLEEP : turn vsync off
  const unsigned int screenOn( VCONFKEY_PM_STATE_NORMAL == status );

  vsyncMonitor->SetHardwareVSyncAvailable( screenOn );

  DALI_LOG_INFO( gLogFilter, Debug::Concise, "%s, Screen %s.\n", __PRETTY_FUNCTION__, screenOn ? "On" : "Off" );
}

} // unnamed namespace

VSyncMonitor::VSyncMonitor()
: mFileDescriptor( FD_NONE ),
  mUseHardwareVSync( TRUE ),
  mHardwareVSyncAvailable( FALSE )
{
  vconf_notify_key_changed( VCONFKEY_PM_STATE, ScreenStatusChanged, this );
}

VSyncMonitor::~VSyncMonitor()
{
  Terminate();

  vconf_ignore_key_changed( VCONFKEY_PM_STATE, ScreenStatusChanged );
}

void VSyncMonitor::SetUseHardwareVSync( bool useHardware )
{
  mUseHardwareVSync = useHardware;
}

void VSyncMonitor::SetHardwareVSyncAvailable( bool hardwareVSyncAvailable )
{
  mHardwareVSyncAvailable = hardwareVSyncAvailable;
}

void VSyncMonitor::Initialize()
{
  DALI_ASSERT_DEBUG( mFileDescriptor == FD_NONE && "VSyncMonitor::Initialize() called twice" );

  // Read initial 'use hardware' status
  ScreenStatusChanged( NULL, this );

  // open /dev node
  mFileDescriptor = open( DRM_DEVICE, O_RDWR );

  // setup vblank request - block and wait for next vblank
  mVBlankInfo.request.type = DRM_VBLANK_NEXTONMISS;
  mVBlankInfo.request.sequence = 0;
  mVBlankInfo.request.signal = 0;

  // setup vblank reply - block and wait for next vblank
  mVBlankInfo.reply.type = DRM_VBLANK_NEXTONMISS;
  mVBlankInfo.reply.sequence = 0;
  mVBlankInfo.reply.tval_sec = 0;
  mVBlankInfo.reply.tval_usec = 0;
}

void VSyncMonitor::Terminate()
{
  if( mFileDescriptor != FD_NONE )
  {
    close( mFileDescriptor );
    mFileDescriptor = FD_NONE;
  }
}

bool VSyncMonitor::UseHardware()
{
  return mUseHardwareVSync && mHardwareVSyncAvailable && (FD_NONE != mFileDescriptor );
}

bool VSyncMonitor::DoSync( unsigned int& frameNumber, unsigned int& seconds, unsigned int& microseconds )
{
  DALI_ASSERT_DEBUG( mFileDescriptor != FD_NONE && "ECoreX::VSyncMonitor is not initialized" );

  if( 0 == drmWaitVBlank( mFileDescriptor, &mVBlankInfo ) )
  {
    frameNumber = mVBlankInfo.reply.sequence;
    seconds = mVBlankInfo.reply.tval_sec;
    microseconds = mVBlankInfo.reply.tval_usec;

    return true;
  }

  return false;
}

} // namespace Adaptor

} // namespace Internal

} // namespace Dali
