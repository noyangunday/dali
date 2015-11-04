// CLASS HEADER
#include "x-event-manager.h"

// EXTERNAL INCLUDES
#include <stdio.h>
#include <cstring>
#include <dali/integration-api/debug.h>
#include <dali/public-api/signals/callback.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

namespace
{

#if defined(DEBUG_ENABLED)
//Dali::Integration::Log::Filter* gInputDeviceLogFilter  = Integration::Log::Filter::New(Debug::NoLogging, false, "LOG_X_EVENT");
#endif

}

XEventManager::XEventManager( XID window, Display* display, WindowEventInterface* eventInterface )
: mXInput2( window, display, eventInterface ),
  mFileDescriptorMonitor( NULL ),
  mDisplay( display ),
  mWindow( window ),
  mInitialized( false )
{

}
XEventManager::~XEventManager()
{
  delete mFileDescriptorMonitor;
}

void XEventManager::Initialize()
{
  if( mInitialized )
  {
    return;
  }

  mXInput2.Initialize();

  // Start monitoring for X events on a file descriptor return via ConnectionNumber.
  int fileDescriptor = ConnectionNumber( mDisplay );

  CallbackBase* callback =  MakeCallback( this, &XEventManager::XEventReceived);

  mFileDescriptorMonitor = new FileDescriptorMonitor( fileDescriptor, callback );

  mInitialized = true;
}


void XEventManager::XEventReceived()
{
  while( XPending( mDisplay) )
  {
    XEvent xEvent;
    XNextEvent( mDisplay, &xEvent );

    // cookie data pointer is undefined until XGetEventData is called.
    XGenericEventCookie* cookie = &xEvent.xcookie;

    if (XGetEventData( mDisplay, cookie))
    {
      if( cookie->extension == mXInput2.GetExtensionId() )
      {
        mXInput2.ProcessEvent( cookie );
      }
      XFreeEventData( mDisplay, cookie );
    }
  }
}


} // namespace internal
} // namespace adaptor
} // namespace dali
