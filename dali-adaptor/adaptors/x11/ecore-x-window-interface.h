#ifndef __DALI_INTERNAL_ECORE_X_RENDER_SURFACE_H__
#define __DALI_INTERNAL_ECORE_X_RENDER_SURFACE_H__

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
#include <string>

#include <Ecore_X.h>
#include <X11/Xlib.h>

// INTERNAL INCLUDES

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

namespace ECore
{

namespace WindowInterface
{
/**
 * X11 Window interface, to separate X11 calls within adaptor.
 */

/**
 * Gets the Ecore X Window
 * @return window
 */
 Ecore_X_Window GetWindow();

 /**
  * Gets a specified X window property
  * @param[in] property the required property id
  * @param[in] xDataType the type
  * @param[in] num the offset / index of the property
  * @return string the property value
  */
 std::string GetWindowProperty( Ecore_X_Atom property, Ecore_X_Atom *xDataType, unsigned int num  );

 /**
  * Send an X Event
  * @param[in] display target display
  * @param[in] window target window
  * @param[in] propagate to propagate to other windows
  * @parma[in] eventMask event mask
  * @param[in] messageType Ecore_X_Atom message type
  * @param[in] messageFormat format of message
  * @param[in] msg message to send
  */
 void SendXEvent(Ecore_X_Display* display, Ecore_X_Window window, bool propagate,
                 long int eventMask, Ecore_X_Atom messageType, int messageFormat, const char *msg );

} // namespace WindowInterface


} // namespace ECore

} // namespace Adaptor

} // namespace internal

} // namespace Dali

#endif // __DALI_INTERNAL_ECORE_X_RENDER_SURFACE_H__
