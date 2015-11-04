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
#include "ecore-wl-render-surface.h"

// EXTERNAL INCLUDES
#include <dali/integration-api/gl-abstraction.h>
#include <dali/integration-api/debug.h>

// INTERNAL INCLUDES
#include <ecore-wl-types.h>
#include <trigger-event.h>
#include <gl/egl-implementation.h>

namespace Dali
{

#if defined(DEBUG_ENABLED)
Debug::Filter* gRenderSurfaceLogFilter = Debug::Filter::New(Debug::Verbose, false, "LOG_ECORE_X_RENDER_SURFACE");
#endif

namespace ECore
{

EcoreWlRenderSurface::EcoreWlRenderSurface(Dali::PositionSize positionSize,
                                           Any surface,
                                           const std::string& name,
                                           bool isTransparent)
: mPosition(positionSize),
  mTitle(name),
  mRenderNotification(NULL),
  mColorDepth(isTransparent ? COLOR_DEPTH_32 : COLOR_DEPTH_24),
  mOwnSurface(false)
{
}

void EcoreWlRenderSurface::Init( Any surface )
{
  // see if there is a surface in Any surface
  unsigned int surfaceId  = GetSurfaceId( surface );

  // if the surface is empty, create a new one.
  if ( surfaceId == 0 )
  {
    // we own the surface about to created
    ecore_wl_init(NULL);
    mOwnSurface = true;
    CreateWlRenderable();
  }
  else
  {
    // XLib should already be initialized so no point in calling XInitThreads
    UseExistingRenderable( surfaceId );
  }

#ifdef DEBUG_ENABLED
  // prints out 'INFO: DALI: new EcoreWlRenderSurface, created display xx, used existing surface xx
  // we can not use LOG_INFO because the surface can be created before Dali Core is created.
  printf( "INFO: DALI: new EcoreWlRenderSurface, %s %s surface %X \n",
          mOwnSurface?"created":"used existing",
          Dali::RenderSurface::PIXMAP==mType?" pixmap" :"window",
          GetDrawable() );
#endif
}

EcoreWlRenderSurface::~EcoreWlRenderSurface()
{
}

void EcoreWlRenderSurface::SetRenderNotification(TriggerEventInterface* renderNotification)
{
  mRenderNotification = renderNotification;
}

Ecore_Wl_Window* EcoreWlRenderSurface::GetWlWindow()
{
  return 0;
}

Ecore_Wl_Window* EcoreWlRenderSurface::GetDrawable()
{
  return 0;
}

PositionSize EcoreWlRenderSurface::GetPositionSize() const
{
  return mPosition;
}

void EcoreWlRenderSurface::MoveResize( Dali::PositionSize positionSize )
{
  // nothing to do in base class
}

void EcoreWlRenderSurface::SetViewMode( ViewMode viewMode )
{
}

unsigned int EcoreWlRenderSurface::GetSurfaceId( Any surface ) const
{
  unsigned int surfaceId = 0;

  if ( surface.Empty() == false )
  {
    // check we have a valid type
    DALI_ASSERT_ALWAYS( ( (surface.GetType() == typeid (Ecore_Wl_Window *) ) )
                        && "Surface type is invalid" );

    surfaceId = AnyCast<unsigned int>( surface );
  }
  return surfaceId;
}

} // namespace ECore

} // namespace Dali
