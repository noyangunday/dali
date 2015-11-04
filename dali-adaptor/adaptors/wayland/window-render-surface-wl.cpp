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
#include <dali/integration-api/gl-abstraction.h>
#include <dali/integration-api/debug.h>

// INTERNAL INCLUDES
#include <ecore-wl-types.h>
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
                                          bool isTransparent)
: EcoreWlRenderSurface( positionSize, surface, name, isTransparent ),
  mNeedToApproveDeiconify(false)
{
  DALI_LOG_INFO( gRenderSurfaceLogFilter, Debug::Verbose, "Creating Window\n" );
  Init( surface );
}

WindowRenderSurface::~WindowRenderSurface()
{
  if( mOwnSurface )
  {
    ecore_wl_window_free( mWlWindow );
  }
}

Ecore_Wl_Window* WindowRenderSurface::GetDrawable()
{
  // already an e-core type
  return mWlWindow;
}

Any WindowRenderSurface::GetSurface()
{
  // already an e-core type
  return Any( mWlWindow );
}

Ecore_Wl_Window* WindowRenderSurface::GetWlWindow()
{
  return mWlWindow;
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
  ecore_wl_window_surface_create(mWlWindow);
  mEglWindow = wl_egl_window_create(ecore_wl_window_surface_get(mWlWindow), mPosition.width, mPosition.height);
  eglImpl.CreateSurfaceWindow( (EGLNativeWindowType)mEglWindow, mColorDepth ); // reinterpret_cast does not compile
}

void WindowRenderSurface::DestroyEglSurface( EglInterface& eglIf )
{
  DALI_LOG_TRACE_METHOD( gRenderSurfaceLogFilter );

  Internal::Adaptor::EglImplementation& eglImpl = static_cast<Internal::Adaptor::EglImplementation&>( eglIf );
  eglImpl.DestroySurface();
  wl_egl_window_destroy(mEglWindow);
  mEglWindow = NULL;
}

bool WindowRenderSurface::ReplaceEGLSurface( EglInterface& egl )
{
  DALI_LOG_TRACE_METHOD( gRenderSurfaceLogFilter );

  wl_egl_window_destroy(mEglWindow);
  mEglWindow = wl_egl_window_create(ecore_wl_window_surface_get(mWlWindow), mPosition.width, mPosition.height);

  Internal::Adaptor::EglImplementation& eglImpl = static_cast<Internal::Adaptor::EglImplementation&>( egl );
  return eglImpl.ReplaceSurfaceWindow( (EGLNativeWindowType)mEglWindow ); // reinterpret_cast does not compile
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

  if(needToMove)
  {
    ecore_wl_window_move(mWlWindow, positionSize.x, positionSize.y);
    mPosition = positionSize;
  }
  if (needToResize)
  {
    ecore_wl_window_resize(mWlWindow, positionSize.width, positionSize.height, 0);
    mPosition = positionSize;
  }

}

void WindowRenderSurface::Map()
{
  ecore_wl_window_show(mWlWindow);
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

    //FIXME

    mNeedToApproveDeiconify = false;
  }
}

void WindowRenderSurface::StopRender()
{
}

void WindowRenderSurface::SetViewMode( ViewMode viewMode )
{
  //FIXME
}

void WindowRenderSurface::CreateWlRenderable()
{
   // if width or height are zero, go full screen.
  if ( (mPosition.width == 0) || (mPosition.height == 0) )
  {
    // Default window size == screen size
    mPosition.x = 0;
    mPosition.y = 0;

    ecore_wl_screen_size_get( &mPosition.width, &mPosition.height );
  }

  mWlWindow = ecore_wl_window_new( 0, mPosition.x, mPosition.y, mPosition.width, mPosition.height, ECORE_WL_WINDOW_BUFFER_TYPE_EGL_WINDOW );

  if ( mWlWindow == 0 )
  {
      DALI_ASSERT_ALWAYS(0 && "Failed to create X window");
  }

  //FIXME
}

void WindowRenderSurface::UseExistingRenderable( unsigned int surfaceId )
{
  mWlWindow = AnyCast< Ecore_Wl_Window* >( surfaceId );
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
