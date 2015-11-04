#ifndef __DALI_INTEGRATION_EGL_INTERFACE_H__
#define __DALI_INTEGRATION_EGL_INTERFACE_H__

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

namespace Dali
{

enum ColorDepth
{
  COLOR_DEPTH_24 = 24,
  COLOR_DEPTH_32 = 32
};

/**
 * EglInterface provides an interface for managing EGL contexts
 */
class EglInterface
{
public:
  /**
    * Create the OpenGL context.
    * @return true if successful
    */
  virtual bool CreateContext() = 0;

  /**
   * Make the OpenGL context current
   */
  virtual void MakeContextCurrent() = 0;

  /**
   * Terminate GL
   */
  virtual void TerminateGles() = 0;

  /**
   * Performs an OpenGL swap buffers command
   */
  virtual void SwapBuffers() = 0;

  /**
   * Performs an OpenGL copy buffers command
   */
  virtual void CopyBuffers() = 0;

  /**
   * Performs an EGL wait GL command
   */
  virtual void WaitGL() = 0;

protected:
  /**
   * Virtual protected destructor, no deletion through this interface
   */
  virtual ~EglInterface() {}
};

} // namespace Dali

#endif // __DALI_INTEGRATION_EGL_INTERFACE_H__
