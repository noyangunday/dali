#ifndef __DALI_INTERNAL_EGL_IMPLEMENTATION_H__
#define __DALI_INTERNAL_EGL_IMPLEMENTATION_H__

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
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <dali/public-api/common/dali-vector.h>

// INTERNAL INCLUDES
#include <egl-interface.h>

namespace Dali
{

namespace Internal
{
namespace Adaptor
{

/**
 * EglImplementation class provides an EGL implementation.
 */
class EglImplementation : public EglInterface
{
public:
  /**
   * Constructor
   */
  EglImplementation();

  /**
   * Destructor
   */
  virtual ~EglImplementation();

public:

  /**
   * (Called from  ECoreX::RenderSurface, not RenderThread, so not in i/f, hence, not virtual)
   * Initialize GL
   * @param display The display
   * @param isOwnSurface whether the surface is own or not
   * @return true on success, false on failure
   */
  bool InitializeGles( EGLNativeDisplayType display, bool isOwnSurface = true );

  /**
    * Create the OpenGL context.
    * @return true if successful
    */
  virtual bool CreateContext();

  /**
    * Destroy the OpenGL context.
    */
  void DestroyContext();

  /**
    * Destroy the OpenGL surface.
    */
  void DestroySurface();

  /**
   * Make the OpenGL context current
   */
  virtual void MakeContextCurrent();

  /**
   * clear the OpenGL context
   */
  void MakeContextNull();

  /**
   * Terminate GL
   */
  virtual void TerminateGles();

  /**
   * Checks if GL is initialised
   * @return true if it is
   */
  bool IsGlesInitialized() const;

  /**
   * Performs an OpenGL swap buffers command
   */
  virtual void SwapBuffers();

  /**
   * Performs an OpenGL copy buffers command
   */
  virtual void CopyBuffers();

  /**
   * Performs an EGL wait GL command
   */
  virtual void WaitGL();

  /**
   * Choose config of egl
   * @param isWindowType whether the config for window or pixmap
   * @param colorDepth Bit per pixel value (ex. 32 or 24)
  */
  void ChooseConfig( bool isWindowType, ColorDepth depth );

  /**
    * Create an OpenGL surface using a window
    * @param window The window to create the surface on
    * @param colorDepth Bit per pixel value (ex. 32 or 24)
    * @return true on success, false on failure
    */
  void CreateSurfaceWindow( EGLNativeWindowType window, ColorDepth depth );

  /**
   * Create the OpenGL surface using a pixmap
   * @param pixmap The pixmap to create the surface on
   * @param colorDepth Bit per pixel value (ex. 32 or 24)
   * @return true on success, false on failure
   */
  void CreateSurfacePixmap( EGLNativePixmapType pixmap, ColorDepth depth );

  /**
   * Replaces the render surface
   * @param[in] window, the window to create the new surface on
   * @return true if the context was lost due to a change in display
   *         between old surface and new surface
   */
  bool ReplaceSurfaceWindow( EGLNativeWindowType window );

  /**
   * Replaces the render surface
   * @param[in] pixmap, the pixmap to create the new surface on
   * @return true if the context was lost due to a change in x-display
   *         between old surface and new surface
   */
  bool ReplaceSurfacePixmap( EGLNativePixmapType pixmap );

  /**
   * returns the display with which this object was initialized
   * @return the EGL Display.
   */
  EGLDisplay GetDisplay() const;

  /**
   * Returns the EGL context
   * @return the EGL context.
   */
  EGLContext GetContext() const;

private:

  Vector<EGLint>       mContextAttribs;

  EGLNativeDisplayType mEglNativeDisplay;
  EGLNativeWindowType  mEglNativeWindow;
  EGLNativePixmapType  mEglNativePixmap;

  EGLDisplay           mEglDisplay;
  EGLConfig            mEglConfig;
  EGLContext           mEglContext;
  EGLSurface           mEglSurface;

  bool                 mGlesInitialized;
  bool                 mIsOwnSurface;
  bool                 mContextCurrent;
  bool                 mIsWindow;
  ColorDepth           mColorDepth;
};

} // namespace Adaptor

} // namespace Internal

} // namespace Dali

#endif // __DALI_INTERNAL_EGL_IMPLEMENTATION_H__
