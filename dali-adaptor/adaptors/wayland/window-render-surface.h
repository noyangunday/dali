#ifndef __DALI_INTERNAL_ECORE_WL_WINDOW_RENDER_SURFACE_H__
#define __DALI_INTERNAL_ECORE_WL_WINDOW_RENDER_SURFACE_H__

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

// INTERNAL INCLUDES
#include <ecore-wl-render-surface.h>
#include <wayland-egl.h>

namespace Dali
{

namespace ECore
{

/**
 * @copydoc Dali::ECore::EcoreWlRenderSurface.
 * Window specialization.
 */
class WindowRenderSurface : public EcoreWlRenderSurface
{
public:

  /**
    * Uses an Wayland surface to render to.
    * @param [in] positionSize the position and size of the surface
    * @param [in] surface can be a Wayland-window or Wayland-pixmap (type must be unsigned int).
    * @param [in] name optional name of surface passed in
    * @param [in] isTransparent if it is true, surface has 32 bit color depth, otherwise, 24 bit
    */
  WindowRenderSurface( Dali::PositionSize positionSize,
                       Any surface,
                       const std::string& name,
                       bool isTransparent = false );

  /**
   * @copydoc Dali::ECore::EcoreWlRenderSurface::~EcoreWlRenderSurface
   */
  virtual ~WindowRenderSurface();

public: // API

  /**
   * @copydoc Dali::RenderSurface::GetDrawable()
   */
  virtual Ecore_Wl_Window* GetDrawable();

  /**
   * Request to approve deiconify operation
   * If it is requested, it will send ECORE_X_ATOM_E_DEICONIFY_APPROVE event to window manager after rendering
   */
  void RequestToApproveDeiconify();

  /**
   * Map window
   */
  virtual void Map();

  /**
   * @copydoc Dali::ECore::EcoreWlRenderSurface::GetSurface()
   */
  virtual Any GetSurface();

  /**
   * @copydoc Dali::ECore::EcoreWlRenderSurface::GetWlWindow()
   */
  virtual Ecore_Wl_Window* GetWlWindow();

public: // from Dali::RenderSurface

  /**
   * @copydoc Dali::RenderSurface::InitializeEgl()
   */
  virtual void InitializeEgl( EglInterface& egl );

  /**
   * @copydoc Dali::RenderSurface::CreateEglSurface()
   */
  virtual void CreateEglSurface( EglInterface& egl );

  /**
   * @copydoc Dali::RenderSurface::DestroyEglSurface()
   */
  virtual void DestroyEglSurface( EglInterface& egl );

  /**
   * @copydoc Dali::RenderSurface::ReplaceEGLSurface()
   */
  virtual bool ReplaceEGLSurface( EglInterface& egl );

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
  virtual void StartRender();

  /**
   * @copydoc Dali::RenderSurface::PreRender()
   */
  virtual bool PreRender( EglInterface& egl, Integration::GlAbstraction& glAbstraction );

  /**
   * @copydoc Dali::RenderSurface::PostRender()
   */
  virtual void PostRender( EglInterface& egl, Integration::GlAbstraction& glAbstraction, DisplayConnection* displayConnection, bool replacingSurface );

  /**
   * @copydoc Dali::RenderSurface::StopRender()
   */
  virtual void StopRender();

  /**
   * @copydoc Dali::RenderSurface::SetThreadSynchronization
   */
  virtual void SetThreadSynchronization( ThreadSynchronizationInterface& threadSynchronization );

  /**
   * @copydoc Dali::RenderSurface::ReleaseLock()
   */
  virtual void ReleaseLock();

protected:

  /**
   * Create WlWindow
   */
  virtual void CreateWlRenderable();

  /**
   * @copydoc Dali::Internal::Adaptor::ECore::EcoreWlRenderSurface::UseExistingRenderable
   */
  virtual void UseExistingRenderable( unsigned int surfaceId );

private: // Data

  Ecore_Wl_Window*   mWlWindow; ///< Wayland-Window
  wl_egl_window*     mEglWindow;
  bool             mNeedToApproveDeiconify; ///< Whether need to send ECORE_X_ATOM_E_DEICONIFY_APPROVE event

}; // class WindowRenderSurface

} // namespace ECore

} // namespace Dali

#endif // __DALI_INTERNAL_ECORE_X_WINDOW_RENDER_SURFACE_H__
