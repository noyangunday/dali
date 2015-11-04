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

PixmapRenderSurface::PixmapRenderSurface(Dali::PositionSize positionSize,
                                         Any surface,
                                         const std::string& name,
                                         bool isTransparent)
: EcoreWlRenderSurface( positionSize, surface, name, isTransparent )
{
  Init( surface );
}

PixmapRenderSurface::~PixmapRenderSurface()
{
  // release the surface if we own one
  if( mOwnSurface )
  {
    // if we did create the pixmap, delete the pixmap
    DALI_LOG_INFO( gRenderSurfaceLogFilter, Debug::General, "Own pixmap (%x) freed\n", mX11Pixmap );
  }
}

Ecore_Wl_Window* PixmapRenderSurface::GetDrawable()
{
  return NULL;
}

Any PixmapRenderSurface::GetSurface()
{
  return Any( NULL );
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

  // create the EGL surface
  // need to cast to X handle as in 64bit system ECore handle is 32 bit whereas EGLnative and XWindow are 64 bit
  // FIXME
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
  // FIXME
  return false;
}

void PixmapRenderSurface::StartRender()
{
  // FIXME
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

 // create damage for client applications which wish to know the update timing
  if( mRenderNotification )
  {
    // use notification trigger
    // Tell the event-thread to render the pixmap
    mRenderNotification->Trigger();
  }
  else
  {
    // FIXME
  }

  // Do render synchronisation
  // AcquireLock( replacingSurface ? SYNC_MODE_NONE : SYNC_MODE_WAIT );
}

void PixmapRenderSurface::StopRender()
{
  // FIXME
}

void PixmapRenderSurface::SetThreadSynchronization( ThreadSynchronizationInterface& threadSynchronization )
{
  // Nothing to do
}

void PixmapRenderSurface::CreateWlRenderable()
{
  // check we're creating one with a valid size
  DALI_ASSERT_ALWAYS( mPosition.width > 0 && mPosition.height > 0 && "Pixmap size is invalid" );

  // FIXME
}

void PixmapRenderSurface::UseExistingRenderable( unsigned int surfaceId )
{
  // FIXME
}

void PixmapRenderSurface::SetSyncMode( SyncMode syncMode )
{
  // FIXME
}

void PixmapRenderSurface::AcquireLock()
{
  // FIXME
}

void PixmapRenderSurface::ReleaseLock()
{
  // FIXME
}

} // namespace ECore

} // namespace Dali
