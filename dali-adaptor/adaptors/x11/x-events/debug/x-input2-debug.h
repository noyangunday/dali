#ifndef __DALI_INTERNAL_X_INPUT_2_DEBUG_H__
#define __DALI_INTERNAL_X_INPUT_2_DEBUG_H__

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



namespace Dali
{

namespace Internal
{

namespace Adaptor
{

namespace X11Debug
{

/**
 * To log input devices found on the system buid DALi in debug mode.
 * Then on the command line:
 *
 * export LOG_X_INPUT_DEVICES=2
 * dali-demo
 *
 *
 * To log XInput events
 *
 * export LOG_X_INPUT_EVENTS=2
 * dali-demo
 *
 * 2 = LogLevel::General
 */


/**
 * @brief Debug log input device information.
 * Similar output to command line tool 'xinput -list' except it includes class + source information
 * Useful if the device doesn't have xinput tool installed
 * @param devices array of XIDeviceInfo
 * @param numberOfDevices number of devices
 */
void LogInputDeviceInfo( const XIDeviceInfo* devices, unsigned int numberOfDevices );

/**
 * @brief Debug log input event information.
 * @param cookie input event cookie
 */
void LogXI2Event( XGenericEventCookie* cookie );

} // X11 Debug
} // namespace Adaptor
} // namespace Internal
} // namespace Dali

#endif
