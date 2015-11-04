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
#include "ecore-x-window-interface.h"

// EXTERNAL INCLUDES
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <dali/integration-api/debug.h>

// INTERNAL INCLUDES
#include <ecore-x-types.h>

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
// CONSTANTS
const char* const CBHM_WINDOW = "CBHM_XWIN";

Ecore_X_Window GetWindow()
{
  Ecore_X_Atom xAtomCbhm = ecore_x_atom_get( CBHM_WINDOW );

  Ecore_X_Window xCbhmWin = 0;
  unsigned char *buf = NULL;
  int num = 0;
  int ret = ecore_x_window_prop_property_get( 0, xAtomCbhm, XA_WINDOW, 0, &buf, &num );

  if ( ret && num )
  {
    memcpy( &xCbhmWin, buf, sizeof( Ecore_X_Window ) );
  }
  if ( buf )
  {
    free( buf );
  }

  return xCbhmWin;
}

std::string GetWindowProperty( Ecore_X_Atom property, Ecore_X_Atom *xDataType, unsigned int num  )
{
   std::string data("");

   if ( !property )
   {
      return data;
   }

   ecore_x_sync();

   long unsigned int numRet = 0, bytes = 0;
   int ret = 0, sizeRet;
   unsigned int i;
   unsigned char *propRet;
   Ecore_X_Atom typeRet;

   // X11 Function to get window property
   ret = XGetWindowProperty( static_cast<Display*>(ecore_x_display_get()), // Display* X Server Connection
                             GetWindow(),                                  // Window  window in question
                             property,                                     // Atom  name of property
                             num,                                          // long offset where required data is stored
                             LONG_MAX,                                     // long length of data to retrieve
                             False,                                        // Bool flag to delete data
                             ecore_x_window_prop_any_type(),               // Atom atom id associated to property type
                             (Atom *)&typeRet,                             // Atom actual_type_return, atom id property type
                             &sizeRet,                                     // int* format of property
                             &numRet,                                      // unsigned long*  number of items being returned in prop_return
                             &bytes,                                       // unsigned long* remaining bytes if partial retrieval
                             &propRet );                                   // unsigned char** return data
   if ( ret != Success )
   {
     return data;
   }

   if ( !numRet )
   {
      XFree( propRet );
      return data;
   }

   numRet--; // As propRet in XGetWindowProperty gets an extra 0 added for compatibility reasons.

   switch ( sizeRet ) // Format returned by XGetWindowProperty int, short, long
   {
   case 8:
   {
     for ( i = 0; i < numRet; i++ )
     {
       data += propRet[i];
     }
   }
   break;

   case 16:
   {
     for ( i = 0; i < numRet; i++ )
     {
       data +=  ( ( unsigned short * )propRet )[i];
     }
   }
   break;

   case 32:
   {
     for ( i = 0; i < numRet; i++ )
     {
       data += ( ( unsigned long * )propRet )[i];
     }
   }
   break;
   }

   XFree( propRet );

   if ( xDataType )
   {
     *xDataType = typeRet;
   }

   return data;
  }

void SendXEvent(Ecore_X_Display* display, Ecore_X_Window window, bool propagate,
    long int eventMask, Ecore_X_Atom messageType, int messageFormat, const char *msg )
{
  XClientMessageEvent message;
  memset(&message, 0, sizeof( message ) );
  message.type = ClientMessage;
  message.display = static_cast<Display*>( display );
  message.message_type = messageType;
  message.format = messageFormat;
  message.window = window;
  snprintf(message.data.b, 20, "%s", msg);

  XSendEvent( static_cast<Display*>( display ), window, propagate, eventMask,( XEvent* )&message );
}


} // namespace WindowInterface


} // namespace ECore

} // namespace Adaptor

} // namespace Internal

} // namespace Dali
