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
#include "window-render-surface.h"

// EXTERNAL INCLUDES
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <X11/extensions/Xfixes.h> // for damage notify
#include <X11/extensions/Xdamage.h> // for damage notify

#include <dali/integration-api/gl-abstraction.h>
#include <dali/integration-api/debug.h>

// INTERNAL INCLUDES

#include <ecore-x-types.h>
#include <trigger-event.h>
#include <gl/egl-implementation.h>
#include <base/display-connection.h>

namespace Dali
{

#if defined(DEBUG_ENABLED)
extern Debug::Filter* gRenderSurfaceLogFilter;
#endif

namespace ECore
{

namespace
{

const int MINIMUM_DIMENSION_CHANGE( 1 ); ///< Minimum change for window to be considered to have moved

} // unnamed namespace

WindowRenderSurface::WindowRenderSurface( Dali::PositionSize positionSize,
                                          Any surface,
                                          const std::string& name,
                                          const std::string& className,
                                          bool isTransparent)
: EcoreXRenderSurface( positionSize, surface, name, isTransparent ),
  mNeedToApproveDeiconify(false),
  mClassName(className)
{
  DALI_LOG_INFO( gRenderSurfaceLogFilter, Debug::Verbose, "Creating Window\n" );
  Init( surface );
}

WindowRenderSurface::~WindowRenderSurface()
{
  if( mOwnSurface )
  {
    ecore_x_window_free( mX11Window );
  }
}

Ecore_X_Drawable WindowRenderSurface::GetDrawable()
{
  // already an e-core type
  return (Ecore_X_Drawable)mX11Window;
}

Any WindowRenderSurface::GetSurface()
{
  // already an e-core type
  return Any( mX11Window );
}

Ecore_X_Window WindowRenderSurface::GetXWindow()
{
  return mX11Window;
}

void WindowRenderSurface::RequestToApproveDeiconify()
{
  mNeedToApproveDeiconify = true;
}

void WindowRenderSurface::InitializeEgl( EglInterface& eglIf )
{
  DALI_LOG_TRACE_METHOD( gRenderSurfaceLogFilter );

  Internal::Adaptor::EglImplementation& eglImpl = static_cast<Internal::Adaptor::EglImplementation&>( eglIf );

  eglImpl.ChooseConfig(true, mColorDepth);
}

void WindowRenderSurface::CreateEglSurface( EglInterface& eglIf )
{
  DALI_LOG_TRACE_METHOD( gRenderSurfaceLogFilter );

  Internal::Adaptor::EglImplementation& eglImpl = static_cast<Internal::Adaptor::EglImplementation&>( eglIf );

  // create the EGL surface
  // need to cast to X handle as in 64bit system ECore handle is 32 bit whereas EGLnative and XWindow are 64 bit
  XWindow window = static_cast< XWindow>( mX11Window );
  eglImpl.CreateSurfaceWindow( (EGLNativeWindowType)window, mColorDepth ); // reinterpret_cast does not compile
}

void WindowRenderSurface::DestroyEglSurface( EglInterface& eglIf )
{
  DALI_LOG_TRACE_METHOD( gRenderSurfaceLogFilter );

  Internal::Adaptor::EglImplementation& eglImpl = static_cast<Internal::Adaptor::EglImplementation&>( eglIf );
  eglImpl.DestroySurface();
}

bool WindowRenderSurface::ReplaceEGLSurface( EglInterface& egl )
{
  DALI_LOG_TRACE_METHOD( gRenderSurfaceLogFilter );

  // need to cast to X handle as in 64bit system ECore handle is 32 bit whereas EGLnative and XWindow are 64 bit
  XWindow window = static_cast< XWindow >( mX11Window );
  Internal::Adaptor::EglImplementation& eglImpl = static_cast<Internal::Adaptor::EglImplementation&>( egl );

  return eglImpl.ReplaceSurfaceWindow( (EGLNativeWindowType)window ); // reinterpret_cast does not compile
}

void WindowRenderSurface::MoveResize( Dali::PositionSize positionSize )
{
  bool needToMove = false;
  bool needToResize = false;

  // check moving
  if( (fabs(positionSize.x - mPosition.x) > MINIMUM_DIMENSION_CHANGE) ||
      (fabs(positionSize.y - mPosition.y) > MINIMUM_DIMENSION_CHANGE) )
  {
    needToMove = true;
  }

  // check resizing
  if( (fabs(positionSize.width - mPosition.width) > MINIMUM_DIMENSION_CHANGE) ||
      (fabs(positionSize.height - mPosition.height) > MINIMUM_DIMENSION_CHANGE) )
  {
    needToResize = true;
  }

  if( needToMove &&  needToResize)
  {
    ecore_x_window_move_resize(mX11Window, positionSize.x, positionSize.y, positionSize.width, positionSize.height);
    mPosition = positionSize;
  }
  else if(needToMove)
  {
    ecore_x_window_move(mX11Window, positionSize.x, positionSize.y);
    mPosition = positionSize;
  }
  else if (needToResize)
  {
    ecore_x_window_resize(mX11Window, positionSize.width, positionSize.height);
    mPosition = positionSize;
  }

}

void WindowRenderSurface::Map()
{
  ecore_x_window_show(mX11Window);
}

void WindowRenderSurface::StartRender()
{
}

bool WindowRenderSurface::PreRender( EglInterface&, Integration::GlAbstraction& )
{
  // nothing to do for windows
  return true;
}

void WindowRenderSurface::PostRender( EglInterface& egl, Integration::GlAbstraction& glAbstraction, DisplayConnection* displayConnection, bool replacingSurface )
{
  Internal::Adaptor::EglImplementation& eglImpl = static_cast<Internal::Adaptor::EglImplementation&>( egl );
  eglImpl.SwapBuffers();

  // When the window is deiconified, it approves the deiconify operation to window manager after rendering
  if(mNeedToApproveDeiconify)
  {
    // SwapBuffer is desychronized. So make sure to sychronize when window is deiconified.
    glAbstraction.Finish();

    XDisplay* display = AnyCast<XDisplay *>(displayConnection->GetDisplay());

#ifndef DALI_PROFILE_UBUNTU
    /* client sends immediately reply message using value 1 */
    XEvent xev;

    xev.xclient.window = mX11Window;
    xev.xclient.type = ClientMessage;
    xev.xclient.message_type = ECORE_X_ATOM_E_DEICONIFY_APPROVE;
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = mX11Window;
    xev.xclient.data.l[1] = 1;
    xev.xclient.data.l[2] = 0;
    xev.xclient.data.l[3] = 0;
    xev.xclient.data.l[4] = 0;

    XSendEvent(display, mX11Window, false, ECORE_X_EVENT_MASK_WINDOW_CONFIGURE, &xev);
#endif // DALI_PROFILE_UBUNTU

    XSync(display, false);

    mNeedToApproveDeiconify = false;
  }
}

void WindowRenderSurface::StopRender()
{
}

void WindowRenderSurface::SetViewMode( ViewMode viewMode )
{
  Ecore_X_Atom viewModeAtom( ecore_x_atom_get( "_E_COMP_3D_APP_WIN" ) );

  if( viewModeAtom != None )
  {
    unsigned int value( static_cast<unsigned int>( viewMode ) );
    ecore_x_window_prop_card32_set( mX11Window, viewModeAtom, &value, 1 );
  }
}

void WindowRenderSurface::CreateXRenderable()
{
   // if width or height are zero, go full screen.
  if ( (mPosition.width == 0) || (mPosition.height == 0) )
  {
    // Default window size == screen size
    mPosition.x = 0;
    mPosition.y = 0;

    ecore_x_screen_size_get( ecore_x_default_screen_get(), &mPosition.width, &mPosition.height );
  }

  if(mColorDepth == COLOR_DEPTH_32)
  {
    // create 32 bit window
    mX11Window = ecore_x_window_argb_new( 0, mPosition.x, mPosition.y, mPosition.width, mPosition.height );
  }
  else
  {
    // create 24 bit window
    mX11Window = ecore_x_window_new( 0, mPosition.x, mPosition.y, mPosition.width, mPosition.height );
  }

  if ( mX11Window == 0 )
  {
      DALI_ASSERT_ALWAYS(0 && "Failed to create X window");
  }

  // set up window title which will be helpful for debug utitilty
  ecore_x_icccm_title_set( mX11Window, mTitle.c_str() );
  ecore_x_netwm_name_set( mX11Window, mTitle.c_str() );
  ecore_x_icccm_name_class_set( mX11Window, mTitle.c_str(), mClassName.c_str() );

  // set up etc properties to match with ecore-evas
  char *id = NULL;
  if( ( id = getenv("DESKTOP_STARTUP_ID") ) )
  {
    ecore_x_netwm_startup_id_set( mX11Window, id );
  }

  ecore_x_icccm_hints_set( mX11Window,
                           1,                                // accepts_focus
                           ECORE_X_WINDOW_STATE_HINT_NORMAL, // initial_state
                           0,                                // icon_pixmap
                           0,                                // icon_mask
                           0,                                // icon_window
                           0,                                // window_group
                           0 );                              // is_urgent

  // we SHOULD guarantee the x11 window was created in x server.
  ecore_x_sync();
}

void WindowRenderSurface::UseExistingRenderable( unsigned int surfaceId )
{
  mX11Window = static_cast< Ecore_X_Window >( surfaceId );
}

void WindowRenderSurface::SetThreadSynchronization( ThreadSynchronizationInterface& /* threadSynchronization */ )
{
  // Nothing to do.
}

void WindowRenderSurface::ReleaseLock()
{
  // Nothing to do.
}

} // namespace ECore

} // namespace Dali
