#ifndef __DALI_INTERNAL_BASE_VSYNC_MONITOR_INTERFACE_H__
#define __DALI_INTERNAL_BASE_VSYNC_MONITOR_INTERFACE_H__

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

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

/**
 * Interface for monitoring VSync
 * Implementations must provide a DoSync method
 */
class VSyncMonitorInterface
{
public:

  /**
   * Initialize the vsync monitor.
   */
  virtual void Initialize() = 0;

  /**
   * Terminate the vsync monitor
   */
  virtual void Terminate() = 0;

  /**
   * Checks if hardware sync is available and enabled
   * @return true if hardware sync is available and enabled
   */
  virtual bool UseHardware() = 0;

  /**
   * Wait for vertical blank sync.
   * @param[out] frameNumber  The current sequence number for this vsync (increments by one for each vsync)
   * @param[out] seconds      The timestamp (seconds) when the vsync occured
   * @param[out] microseconds The timestamp (microseconds) when the vsync occured
   * @return true if a valid sync was detected, false on error
   */
  virtual bool DoSync( unsigned int& frameNumber, unsigned int& seconds, unsigned int& microseconds ) = 0;

protected:

  /**
   * Virtual protected destructor - no deletion through this interface
   */
  virtual ~VSyncMonitorInterface() {}

}; // class VSyncMonitorInterface

} // namespace Adaptor

} // namespace Internal

} // namespace Dali

#endif // __DALI_INTERNAL_BASE_VSYNC_MONITOR_INTERFACE_H__
