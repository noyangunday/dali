#ifndef __DALI_INTERNAL_VSYNC_MONITOR_IMPL_H__
#define __DALI_INTERNAL_VSYNC_MONITOR_IMPL_H__

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
#include <xf86drm.h>

// INTERNAL INCLUDES
#include <base/interfaces/vsync-monitor-interface.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

/**
 * Tizen interface for monitoring VSync
 */
class VSyncMonitor : public VSyncMonitorInterface
{
public:
  /**
   * Default constructor
   */
  VSyncMonitor();

  /**
   * Destructor
   */
  virtual ~VSyncMonitor();

public:

  /**
   * Set the use hardware flag
   * @param[in] useHardware The new state for the use hardware flag.
   */
  void SetUseHardwareVSync( bool useHardware );

  /**
   * Set whether the vsync hardware is available.
   * (This is public to allow callback method to work...)
   */
  void SetHardwareVSyncAvailable(bool available);

private: // From Dali::Internal::Adaptor::VSyncMonitorInterface

  /**
   * copydoc Dali::Internal::Adaptor::VSyncMonitorInterface::Initialize
   */
  virtual void Initialize();

  /**
   * copydoc Dali::Internal::Adaptor::VSyncMonitorInterface::Terminate
   */
  virtual void Terminate();

  /**
   * copydoc Dali::Internal::Adaptor::VSyncMonitorInterface::UseHardware
   */
  virtual bool UseHardware();

  /**
   * copydoc Dali::Internal::Adaptor::VSyncMonitorInterface::DoSync
   */
  virtual bool DoSync( unsigned int& frameNumber, unsigned int& seconds, unsigned int& microseconds );

private:

  int       mFileDescriptor;  ///< DRM dev node file descriptor
  drmVBlank mVBlankInfo;
  // NOTE cannot use booleans as these are used from multiple threads, must use variable with machine word size for atomic read/write
  unsigned int mUseHardwareVSync; ///< Whether to use hardware vsync
  unsigned int mHardwareVSyncAvailable; ///< Whether hardware vsync is available
};

} // namespace Adaptor

} // namespace Internal

} // namespace Dali

#endif // __DALI_INTERNAL_VSYNC_MONITOR_IMPL_H__
