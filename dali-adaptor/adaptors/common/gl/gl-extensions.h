#ifndef __DALI_INTERNAL_GL_EXTENSION_H__
#define __DALI_INTERNAL_GL_EXTENSION_H__

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

#if DALI_GLES_VERSION >= 30
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif


namespace Dali
{

namespace Internal
{

namespace Adaptor
{

namespace ECoreX
{

/**
 * GlExtensions class provides GL extensions support
 */
class GlExtensions
{
public:

  /**
   * Constructor
   */
  GlExtensions();

  /**
   * Destructor
   */
  ~GlExtensions();


public:

#if DALI_GLES_VERSION < 30

  /**
   * If the GL extension is available this function discards specified data in attachments
   * from being copied from the target to improve performance.
   *
   * Usage: GLenum attachments[] = { GL_DEPTH_ATTACHMENT, GL_STENCIL_ATTACHMENT };
   *        DiscardFrameBufferEXT(GL_FRAMEBUFFER, 2, attachments);
   *
   * @param target is usually GL_FRAMEBUFFER
   * @param numAttachments is the count of attachments
   * @param attachments is a pointer to the attachments
   */
  void DiscardFrameBuffer (GLenum target, GLsizei numAttachments, const GLenum *attachments);

  /**
   * GLES extension
   * Returns the program object's executable bytecode.
   * @param[in] program       The program object's name/id
   * @param[in] bufSize       The maximum number of bytes that may be written into binary
   * @param[out] length       The actual number of bytes written into binary
   * @param[out] binaryFormat The format of the program binary
   * @param[out] binary       The actual program bytecode
   */
  void GetProgramBinaryOES (GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, GLvoid *binary);

  /**
   * GLES extension
   * Loads a program object with a program binary previously returned from GetProgramBinaryOES
   * @param[in] program       The program object's name/id
   * @param[in] binaryFormat  The format of the program binary
   * @param[in] binary        The program bytecode
   * @param[in] length        The number of bytes in binary
   */
  void ProgramBinaryOES (GLuint program, GLenum binaryFormat, const GLvoid *binary, GLint length);

#endif // DALI_GLES_VERSION < 30

private:

  /**
   * Lazy Initialize extensions on first use
   */
  void Initialize();

#if DALI_GLES_VERSION < 30

#ifdef GL_EXT_discard_framebuffer
  PFNGLDISCARDFRAMEBUFFEREXTPROC mGlDiscardFramebuffer;
#endif

#ifdef GL_OES_get_program_binary
  PFNGLGETPROGRAMBINARYOESPROC mGlGetProgramBinaryOES;
  PFNGLPROGRAMBINARYOESPROC mGlProgramBinaryOES;
#endif

#endif // DALI_GLES_VERSION < 30

  bool mInitialized;

};

} // namespace ECoreX

} // namespace Adaptor

} // namespace Internal

} // namespace Dali

#endif /* __DALI_INTERNAL_GL_EXTENSION_H__ */
