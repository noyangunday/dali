#ifndef __DALI_RENDER_SURFACE_H__
#define __DALI_RENDER_SURFACE_H__

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
#include <dali/public-api/math/rect.h>
#include <dali/public-api/object/any.h>
#include <dali/public-api/common/dali-common.h>
#include <dali/public-api/common/view-mode.h>

// INTERNAL INCLUDES

namespace Dali
{

class EglInterface;
class DisplayConnection;
class ThreadSynchronizationInterface;

namespace Integration
{

class GlAbstraction;

} // namespace Integration

/**
 * @brief The position and size of the render surface.
 */
typedef Dali::Rect<int> PositionSize;

/**
 * @brief Interface for a render surface onto which Dali draws.
 *
 * Dali::Adaptor requires a render surface to draw on to. This is
 * usually a window in the native windowing system, or some other
 * mapped pixel buffer.
 *
 * Dali::Application will automatically create a render surface using a window.
 *
 * The implementation of the factory method below should choose an appropriate
 * implementation of RenderSurface for the given platform
 */

class RenderSurface
{
public:

  /**
   * @brief Constructor
   * Inlined as this is a pure abstract interface
   */
  RenderSurface() {}

  /**
   * @brief Virtual Destructor.
   * Inlined as this is a pure abstract interface
   */
  virtual ~RenderSurface() {}

  /**
   * @brief Return the size and position of the surface.
   * @return The position and size
   */
  virtual PositionSize GetPositionSize() const = 0;

  /**
   * Initialize EGL, RenderSurface should create egl display and initialize
   * @param egl implementation to use for the creation
   */
  virtual void InitializeEgl( EglInterface& egl ) = 0;

  /**
   * @brief Creates EGL Surface
   * @param egl implementation to use for the creation
   */
  virtual void CreateEglSurface( EglInterface& egl ) = 0;

  /**
   * @brief Destroys EGL Surface
   * @param egl implementation to use for the destruction
   */
  virtual void DestroyEglSurface( EglInterface& egl ) = 0;

  /**
   * @brief Replace the EGL Surface
   * @param egl implementation to use for the creation
   * @return true if context was lost
   */
  virtual bool ReplaceEGLSurface( EglInterface& egl ) = 0;

  /**
   * @brief Resizes the underlying surface. Only available for x window
   */
  virtual void MoveResize( Dali::PositionSize positionSize ) = 0;

  /**
   * @brief Set the stereoscopic 3D view mode
   * @param[in] viewMode The new view mode
   */
  virtual void SetViewMode( ViewMode viewMode ) = 0;

  /**
   * @brief Called when Render thread has started
   */
  virtual void StartRender() = 0;

  /**
   * @brief Invoked by render thread before Core::Render
   * If the operation fails, then Core::Render should not be called until there is
   * a surface to render onto.
   * @param[in] egl The Egl interface
   * @param[in] glAbstraction OpenGLES abstraction interface
   * @return True if the operation is successful, False if the operation failed
   */
  virtual bool PreRender( EglInterface& egl, Integration::GlAbstraction& glAbstraction ) = 0;

  /**
   * @brief Invoked by render thread after Core::Render
   * @param[in] egl The Egl interface
   * @param[in] glAbstraction OpenGLES abstraction interface
   * @param[in] displayConnection display connection
   * @param[in] replacingSurface True if the surface is being replaced.
   */
  virtual void PostRender( EglInterface& egl, Integration::GlAbstraction& glAbstraction, DisplayConnection* displayConnection, bool replacingSurface ) = 0;

  /**
   * @brief Invoked by render thread when the thread should be stop
   */
  virtual void StopRender() = 0;

  /**
   * @brief Invoked by Event Thread when the compositor lock should be released and rendering should resume.
   */
  virtual void ReleaseLock() = 0;

  /**
   * @brief Sets the ThreadSynchronizationInterface
   *
   * @param threadSynchronization The thread-synchronization implementation.
   */
  virtual void SetThreadSynchronization( ThreadSynchronizationInterface& threadSynchronization ) = 0;

private:

  /**
   * @brief Undefined copy constructor. RenderSurface cannot be copied
   */
  RenderSurface( const RenderSurface& rhs );

  /**
   * @brief Undefined assignment operator. RenderSurface cannot be copied
   */
  RenderSurface& operator=( const RenderSurface& rhs );
};

} // namespace Dali

#endif // __DALI_RENDER_SURFACE_H__
