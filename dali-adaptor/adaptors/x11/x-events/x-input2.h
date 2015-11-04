#ifndef __DALI_INTERNAL_X_INPUT_2_MANAGER_H__
#define __DALI_INTERNAL_X_INPUT_2_MANAGER_H__

/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd.
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
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/extensions/XInput2.h>
#include <X11/extensions/XI2.h>
#include <dali/public-api/common/dali-vector.h>

// INTERNAL INCLUDES
#include <base/interfaces/window-event-interface.h>
#include "x-input2-device.h"

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

/**
 *
 * @brief Used to setup and process XInput2 events.
 *
 * For help with debugging, build DALi in debug mode then set the environment variables
 * export LOG_X_INPUT_EVENTS=2
 * export LOG_X_INPUT_DEVICES=2
 */
class XInput2
{

public:

  /**
   * @brief Constructor
   */
  XInput2( XID window, Display* display, WindowEventInterface* eventInterface );

  /**
   * @brief destructor
   */
  ~XInput2();

  /**
   * @brief enumerates input devices using XIQueryDevice then sets up event filtering using XISelectEvents
   */
  void Initialize();

  /**
   * @brief get X the extension id
   * @return the Id
   */
  int GetExtensionId() const;

  /**
   * @brief process an XInput2 event
   * @param cookie X cookie
   */
  void ProcessEvent( XGenericEventCookie* cookie );


private:

  /**
   * @brief query x input devices
   */
  void QueryDevices();

  /**
   * @brief query multi-touch support
   */
  void QueryMultiTouchSupport();

  /**
   * Uses XISelectEvents to select the events we want to recieve from each input device
   */
  void SelectInputEvents();

  /**
   * @brief checks if we are filtering events from a specific device
   * @param[in] deviceId device id
   * @return true if the device is being filtered
   */
  bool FilteredDevice( int deviceId ) const;

  /**
   * @brief Select specific events to be filtered on a device
   * @param[in] device id
   * @param[in] filter vector of X input events like XI_ButtonPress
   */
  void SelectEvents( int deviceId, const Dali::Vector< unsigned int >& filter );

  /**
   * @brief checks if the event should be processed
   * @param[in] deviceEvent device event
   * @return true if should be processed
   */
  bool PreProcessEvent( XIDeviceEvent *deviceEvent ) const;

  /**
   * @brief creates a DALi key event given a XIDeviceEvent for a key press
   * @param[in] deviceEvent device event
   * @param[out] keyEvent DALi key event
   */
  void CreateKeyEvent( const XIDeviceEvent* deviceEvent, KeyEvent& keyEvent ) const;

private:

  Dali::Vector< XInput2Device > mInputDeviceInfo;   ///< list of input devices
  WindowEventInterface* mEventInterface;            ///< window event interface
  Display* mDisplay;                                ///< X display
  XID mWindow;                                      ///< X window
  int mXI2ExtensionId;                              ///< XI2 extension id
  bool mMultiTouchSupport:1;                        ///< whether multi-touch is supported
};

} // namespace Adaptor
} // namespace Internal
} // namespace Dali

#endif
