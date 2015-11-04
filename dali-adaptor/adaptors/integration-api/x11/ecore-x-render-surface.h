#ifndef __DALI_ECORE_X_RENDER_SURFACE_H__
#define __DALI_ECORE_X_RENDER_SURFACE_H__

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
#include <Ecore_X.h>
#include <dali/public-api/common/dali-common.h>

// INTERNAL INCLUDES
#include <render-surface.h>
#include <ecore-x-types.h>
#include <egl-interface.h>

namespace Dali
{

class TriggerEventInterface;

namespace ECore
{

/**
 * Ecore X11 implementation of render surface.
 * @todo change namespace to ECore_X11 as the class
 * is no longer pure X11.
 */
class DALI_IMPORT_API EcoreXRenderSurface : public Dali::RenderSurface
{
public:
  /**
    * Uses an X11 surface to render to.
    * @param [in] positionSize the position and size of the surface
    * @param [in] surface can be a X-window or X-pixmap (type must be unsigned int).
    * @param [in] name optional name of surface passed in
    * @param [in] isTransparent if it is true, surface has 32 bit color depth, otherwise, 24 bit
    */
  EcoreXRenderSurface(Dali::PositionSize positionSize,
                      Any surface,
                      const std::string& name,
                      bool isTransparent = false);

  /**
   * Destructor.
   * Will delete the display, if it has ownership.
   * Will delete the window/pixmap if it has owner ship
   */
  virtual ~EcoreXRenderSurface();

protected:
  /**
   * Second stage construction
   * Creates the surface (window, pixmap or native buffer)
   */
  void Init( Any surface );

public: // API

  /**
   * @brief Sets the render notification trigger to call when render thread is completed a frame
   *
   * @param renderNotification to use
   */
  void SetRenderNotification(TriggerEventInterface* renderNotification);

  /**
   * @brief Get window handle
   *
   * @return the Ecore X window handle
   */
  Ecore_X_Window GetXWindow();

  /**
   * Get the surface as an Ecore_X_drawable
   */
  virtual Ecore_X_Drawable GetDrawable();

  /**
   * @brief Get the render surface the adaptor is using to render to.
   *
   * @return reference to current render surface (eg, pixmap / window)
   */
  virtual Any GetSurface() = 0;

public: // from Dali::RenderSurface

  /**
   * @copydoc Dali::RenderSurface::GetPositionSize()
   */
  virtual PositionSize GetPositionSize() const;

  /**
   * @copydoc Dali::RenderSurface::InitializeEgl()
   */
  virtual void InitializeEgl( EglInterface& egl ) = 0;

  /**
   * @copydoc Dali::RenderSurface::CreateEglSurface()
   */
  virtual void CreateEglSurface( EglInterface& egl ) = 0;

  /**
   * @copydoc Dali::RenderSurface::DestroyEglSurface()
   */
  virtual void DestroyEglSurface( EglInterface& egl ) = 0;

  /**
   * @copydoc Dali::RenderSurface::ReplaceEGLSurface()
   */
  virtual bool ReplaceEGLSurface( EglInterface& egl ) = 0;

  /**
   * @copydoc Dali::RenderSurface::MoveResize()
   */
  virtual void MoveResize( Dali::PositionSize positionSize);

  /**
   * @copydoc Dali::RenderSurface::SetViewMode()
   */
  void SetViewMode( ViewMode viewMode );

  /**
   * @copydoc Dali::RenderSurface::StartRender()
   */
  virtual void StartRender() = 0;

  /**
   * @copydoc Dali::RenderSurface::PreRender()
   */
  virtual bool PreRender( EglInterface& egl, Integration::GlAbstraction& glAbstraction ) = 0;

  /**
   * @copydoc Dali::RenderSurface::PostRender()
   */
  virtual void PostRender( EglInterface& egl, Integration::GlAbstraction& glAbstraction, DisplayConnection* displayConnection, bool replacingSurface ) = 0;

  /**
   * @copydoc Dali::RenderSurface::ReleaseLock()
   */
  virtual void ReleaseLock() = 0;

private:

  /**
   * Get the surface id if the surface parameter is not empty
   * @param surface Any containing a surface id, or can be empty
   * @return surface id, or zero if surface is empty
   */
  unsigned int GetSurfaceId( Any surface ) const;

protected:

  /**
   * Create XRenderable
   */
  virtual void CreateXRenderable() = 0;

  /**
   * Use an existing render surface
   * @param surfaceId the id of the surface
   */
  virtual void UseExistingRenderable( unsigned int surfaceId ) = 0;

protected: // Data

  PositionSize                mPosition;           ///< Position
  std::string                 mTitle;              ///< Title of window which shows from "xinfo -topvwins" command
  TriggerEventInterface*      mRenderNotification; ///< Render notification trigger
  ColorDepth                  mColorDepth;         ///< Color depth of surface (32 bit or 24 bit)
  bool                        mOwnSurface;         ///< Whether we own the surface (responsible for deleting it)
};

} // namespace ECore

} // namespace Dali

#endif // __DALI_ECORE_X_RENDER_SURFACE_H__
