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
#include "pixmap-render-surface.h"

// EXTERNAL INCLUDES
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <X11/extensions/Xfixes.h> // for damage notify
#include <X11/extensions/Xdamage.h> // for damage notify

#include <dali/integration-api/gl-abstraction.h>
#include <dali/integration-api/debug.h>
#include <dali/devel-api/threading/mutex.h>

// INTERNAL INCLUDES

#include <integration-api/thread-synchronization-interface.h>
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

struct PixmapRenderSurface::Impl
{
  Ecore_X_Pixmap                  mX11Pixmap;             ///< X-Pixmap
  ThreadSynchronizationInterface* mThreadSynchronization; ///< A pointer to the thread-synchronization
};

PixmapRenderSurface::PixmapRenderSurface(Dali::PositionSize positionSize,
                                         Any surface,
                                         const std::string& name,
                                         bool isTransparent)
: EcoreXRenderSurface( positionSize, surface, name, isTransparent ),
  mImpl( new Impl )
{
  mImpl->mThreadSynchronization = NULL;
  Init( surface );
}

PixmapRenderSurface::~PixmapRenderSurface()
{
  // release the surface if we own one
  if( mOwnSurface )
  {
    // if we did create the pixmap, delete the pixmap
    DALI_LOG_INFO( gRenderSurfaceLogFilter, Debug::General, "Own pixmap (%x) freed\n", mImpl->mX11Pixmap );
    ecore_x_pixmap_free( mImpl->mX11Pixmap );
  }

  delete mImpl;
}

Ecore_X_Drawable PixmapRenderSurface::GetDrawable()
{
  return (Ecore_X_Drawable) mImpl->mX11Pixmap;
}

Any PixmapRenderSurface::GetSurface()
{
  return Any( mImpl->mX11Pixmap );
}

void PixmapRenderSurface::InitializeEgl( EglInterface& egl )
{
  DALI_LOG_TRACE_METHOD( gRenderSurfaceLogFilter );

  Internal::Adaptor::EglImplementation& eglImpl = static_cast<Internal::Adaptor::EglImplementation&>( egl );

  eglImpl.ChooseConfig(false, mColorDepth);
}

void PixmapRenderSurface::CreateEglSurface( EglInterface& egl )
{
  DALI_LOG_TRACE_METHOD( gRenderSurfaceLogFilter );

  Internal::Adaptor::EglImplementation& eglImpl = static_cast<Internal::Adaptor::EglImplementation&>( egl );

  // create the EGL surface
  // need to cast to X handle as in 64bit system ECore handle is 32 bit whereas EGLnative and XWindow are 64 bit
  XPixmap pixmap = static_cast<XPixmap>( mImpl->mX11Pixmap );
  eglImpl.CreateSurfacePixmap( (EGLNativePixmapType)pixmap, mColorDepth ); // reinterpret_cast does not compile
}

void PixmapRenderSurface::DestroyEglSurface( EglInterface& egl )
{
  DALI_LOG_TRACE_METHOD( gRenderSurfaceLogFilter );

  Internal::Adaptor::EglImplementation& eglImpl = static_cast<Internal::Adaptor::EglImplementation&>( egl );
  eglImpl.DestroySurface();
}

bool PixmapRenderSurface::ReplaceEGLSurface( EglInterface& egl )
{
  DALI_LOG_TRACE_METHOD( gRenderSurfaceLogFilter );

  // a new surface for the new pixmap
  // need to cast to X handle as in 64bit system ECore handle is 32 bit whereas EGLnative and XWindow are 64 bit
  XPixmap pixmap = static_cast<XPixmap>( mImpl->mX11Pixmap );
  Internal::Adaptor::EglImplementation& eglImpl = static_cast<Internal::Adaptor::EglImplementation&>( egl );

  return eglImpl.ReplaceSurfacePixmap( (EGLNativePixmapType)pixmap ); // reinterpret_cast does not compile

}

void PixmapRenderSurface::StartRender()
{
}

bool PixmapRenderSurface::PreRender( EglInterface&, Integration::GlAbstraction& )
{
  // nothing to do for pixmaps
  return true;
}

void PixmapRenderSurface::PostRender( EglInterface& egl, Integration::GlAbstraction& glAbstraction, DisplayConnection* displayConnection, bool replacingSurface )
{
  // flush gl instruction queue
  glAbstraction.Flush();

  if( mImpl->mThreadSynchronization )
  {
    mImpl->mThreadSynchronization->PostRenderStarted();
  }

  // create damage for client applications which wish to know the update timing
  if( mRenderNotification )
  {
    // use notification trigger
    // Tell the event-thread to render the pixmap
    mRenderNotification->Trigger();
  }
  else
  {
    // as a fallback, send damage event.
    Ecore_X_Drawable drawable = GetDrawable();

    if( drawable )
    {
      XRectangle rect;
      XserverRegion region;

      rect.x = 0;
      rect.y = 0;
      rect.width = mPosition.width;
      rect.height = mPosition.height;

      XDisplay* display = AnyCast<XDisplay*>(displayConnection->GetDisplay());

      // make a fixes region as updated area
      region = XFixesCreateRegion( display, &rect, 1 );
      // add damage event to updated drawable
      XDamageAdd( display, (Drawable)drawable, region );
      XFixesDestroyRegion( display, region );

      XFlush( display );
    }
  }

  if( mImpl->mThreadSynchronization )
  {
    mImpl->mThreadSynchronization->PostRenderWaitForCompletion();
  }
}

void PixmapRenderSurface::StopRender()
{
  ReleaseLock();
}

void PixmapRenderSurface::SetThreadSynchronization( ThreadSynchronizationInterface& threadSynchronization )
{
  mImpl->mThreadSynchronization = &threadSynchronization;
}

void PixmapRenderSurface::CreateXRenderable()
{
  // check we're creating one with a valid size
  DALI_ASSERT_ALWAYS( mPosition.width > 0 && mPosition.height > 0 && "Pixmap size is invalid" );

  // create the pixmap
  mImpl->mX11Pixmap = ecore_x_pixmap_new(0, mPosition.width, mPosition.height, mColorDepth);

  // clear the pixmap
  unsigned int foreground;
  Ecore_X_GC gc;
  foreground = 0;
  gc = ecore_x_gc_new( mImpl->mX11Pixmap,
                       ECORE_X_GC_VALUE_MASK_FOREGROUND,
                       &foreground );

  DALI_ASSERT_ALWAYS( gc && "CreateXRenderable(): failed to get gc" );

  ecore_x_drawable_rectangle_fill( mImpl->mX11Pixmap, gc, 0, 0, mPosition.width, mPosition.height );

  DALI_ASSERT_ALWAYS( mImpl->mX11Pixmap && "Failed to create X pixmap" );

  // we SHOULD guarantee the xpixmap/x11 window was created in x server.
  ecore_x_sync();

  ecore_x_gc_free(gc);
}

void PixmapRenderSurface::UseExistingRenderable( unsigned int surfaceId )
{
  mImpl->mX11Pixmap = static_cast< Ecore_X_Pixmap >( surfaceId );
}

void PixmapRenderSurface::ReleaseLock()
{
  if( mImpl->mThreadSynchronization )
  {
    mImpl->mThreadSynchronization->PostRenderComplete();
  }
}

} // namespace ECore

} // namespace Dali
