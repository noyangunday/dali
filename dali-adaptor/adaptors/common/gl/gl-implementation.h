#ifndef __DALI_INTERNAL_GL_IMPLEMENTATION_H__
#define __DALI_INTERNAL_GL_IMPLEMENTATION_H__

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
#ifndef DALI_GLES_VERSION
#error "OpenGL ES version not specified"
#endif

#if DALI_GLES_VERSION >= 30
#include <GLES3/gl3.h>
#else
#include <cstdlib>
#include <GLES2/gl2.h>
#endif

#include <dali/integration-api/gl-abstraction.h>

// INTERNAL INCLUDES
#include <gl/gl-extensions.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

/**
 * GlImplementation is a concrete implementation for GlAbstraction.
 * The class provides an OpenGL-ES 2.0 implementation.
 * The class is provided when creating the Integration::Core object.
 */
class GlImplementation: public Dali::Integration::GlAbstraction
{

public:
  virtual ~GlImplementation() {}

  void PreRender()
  {
    /* Do nothing in main implementation */
  }

  void PostRender()
  {
    /* Do nothing in main implementation */
  }

  /* OpenGL ES 2.0 */

  void ActiveTexture (GLenum texture)
  {
    glActiveTexture(texture);
  }

  void AttachShader (GLuint program, GLuint shader)
  {
    glAttachShader(program,shader);
  }

  void BindAttribLocation (GLuint program, GLuint index, const char* name)
  {
    glBindAttribLocation(program,index,name);
  }

  void BindBuffer (GLenum target, GLuint buffer)
  {
    glBindBuffer(target,buffer);
  }

  void BindFramebuffer (GLenum target, GLuint framebuffer)
  {
    glBindFramebuffer(target,framebuffer);
  }

  void BindRenderbuffer (GLenum target, GLuint renderbuffer)
  {
    glBindRenderbuffer(target,renderbuffer);
  }

  void BindTexture (GLenum target, GLuint texture)
  {
    glBindTexture(target,texture);
  }

  void BlendColor (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
  {
    glBlendColor(red,green,blue,alpha);
  }

  void BlendEquation ( GLenum mode )
  {
    glBlendEquation(mode);
  }

  void BlendEquationSeparate (GLenum modeRGB, GLenum modeAlpha)
  {
    glBlendEquationSeparate(modeRGB,modeAlpha);
  }

  void BlendFunc (GLenum sfactor, GLenum dfactor)
  {
    glBlendFunc(sfactor,dfactor);
  }

  void BlendFuncSeparate (GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)
  {
    glBlendFuncSeparate(srcRGB,dstRGB,srcAlpha,dstAlpha);
  }

  void BufferData (GLenum target, GLsizeiptr size, const void* data, GLenum usage)
  {
    glBufferData(target,size,data,usage);
  }

  void BufferSubData (GLenum target, GLintptr offset, GLsizeiptr size, const void* data)
  {
    glBufferSubData(target,offset,size,data);
  }

  GLenum CheckFramebufferStatus (GLenum target)
  {
    return glCheckFramebufferStatus(target);
  }

  void Clear (GLbitfield mask)
  {
    glClear(mask);
  }

  void ClearColor (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
  {
    glClearColor(red,green,blue,alpha);
  }

  void ClearDepthf (GLclampf depth)
  {
    glClearDepthf(depth);
  }

  void ClearStencil (GLint s)
  {
    glClearStencil(s);
  }

  void ColorMask (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
  {
    glColorMask(red,green,blue,alpha);
  }

  void CompileShader (GLuint shader)
  {
    glCompileShader(shader);
  }

  void CompressedTexImage2D (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data)
  {
    glCompressedTexImage2D(target,level,internalformat,width,height,border,imageSize,data);
  }

  void CompressedTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data)
  {
    glCompressedTexSubImage2D(target,level,xoffset,yoffset,width,height,format,imageSize,data);
  }

  void CopyTexImage2D (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
  {
    glCopyTexImage2D(target,level,internalformat,x,y,width,height,border);
  }

  void CopyTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
  {
    glCopyTexSubImage2D(target,level,xoffset,yoffset,x,y,width,height);
  }

  GLuint CreateProgram (void)
  {
    return glCreateProgram();
  }

  GLuint CreateShader (GLenum type)
  {
    return glCreateShader(type);
  }

  void CullFace (GLenum mode)
  {
    glCullFace(mode);
  }

  void DeleteBuffers (GLsizei n, const GLuint* buffers)
  {
    glDeleteBuffers(n,buffers);
  }

  void DeleteFramebuffers (GLsizei n, const GLuint* framebuffers)
  {
    glDeleteFramebuffers(n,framebuffers);
  }

  void DeleteProgram (GLuint program)
  {
    glDeleteProgram(program);
  }

  void DeleteRenderbuffers (GLsizei n, const GLuint* renderbuffers)
  {
    glDeleteRenderbuffers(n,renderbuffers);
  }

  void DeleteShader (GLuint shader)
  {
    glDeleteShader(shader);
  }

  void DeleteTextures (GLsizei n, const GLuint* textures)
  {
    glDeleteTextures(n,textures);
  }

  void DepthFunc (GLenum func)
  {
    glDepthFunc(func);
  }

  void DepthMask (GLboolean flag)
  {
    glDepthMask(flag);
  }

  void DepthRangef (GLclampf zNear, GLclampf zFar)
  {
    glDepthRangef(zNear,zFar);
  }

  void DetachShader (GLuint program, GLuint shader)
  {
    glDetachShader(program,shader);
  }

  void Disable (GLenum cap)
  {
    glDisable(cap);
  }

  void DisableVertexAttribArray (GLuint index)
  {
    glDisableVertexAttribArray(index);
  }

  void DrawArrays (GLenum mode, GLint first, GLsizei count)
  {
    glDrawArrays(mode,first,count);
  }

  void DrawElements (GLenum mode, GLsizei count, GLenum type, const void* indices)
  {
    glDrawElements(mode,count,type,indices);
  }

  void Enable (GLenum cap)
  {
    glEnable(cap);
  }

  void EnableVertexAttribArray (GLuint index)
  {
    glEnableVertexAttribArray(index);
  }

  void Finish (void)
  {
    glFinish();
  }

  void Flush (void)
  {
    glFlush();
  }

  void FramebufferRenderbuffer (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
  {
    glFramebufferRenderbuffer(target,attachment,renderbuffertarget,renderbuffer);
  }

  void FramebufferTexture2D (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
  {
    glFramebufferTexture2D(target,attachment,textarget,texture,level);
  }

  void FrontFace (GLenum mode)
  {
    glFrontFace(mode);
  }

  void GenBuffers (GLsizei n, GLuint* buffers)
  {
    glGenBuffers(n,buffers);
  }

  void GenerateMipmap (GLenum target)
  {
    glGenerateMipmap(target);
  }

  void GenFramebuffers (GLsizei n, GLuint* framebuffers)
  {
    glGenFramebuffers(n,framebuffers);
  }

  void GenRenderbuffers (GLsizei n, GLuint* renderbuffers)
  {
    glGenRenderbuffers(n,renderbuffers);
  }

  void GenTextures (GLsizei n, GLuint* textures)
  {
    glGenTextures(n,textures);
  }

  void GetActiveAttrib (GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name)
  {
    glGetActiveAttrib(program,index,bufsize,length,size,type,name);
  }

  void GetActiveUniform (GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name)
  {
    glGetActiveUniform(program,index,bufsize,length,size,type,name);
  }

  void GetAttachedShaders (GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders)
  {
    glGetAttachedShaders(program,maxcount,count,shaders);
  }

  int  GetAttribLocation (GLuint program, const char* name)
  {
    return glGetAttribLocation(program,name);
  }

  void GetBooleanv (GLenum pname, GLboolean* params)
  {
    glGetBooleanv(pname,params);
  }

  void GetBufferParameteriv (GLenum target, GLenum pname, GLint* params)
  {
    glGetBufferParameteriv(target,pname,params);
  }

  GLenum       GetError (void)
  {
    return glGetError();
  }

  void GetFloatv (GLenum pname, GLfloat* params)
  {
    glGetFloatv(pname,params);
  }

  void GetFramebufferAttachmentParameteriv (GLenum target, GLenum attachment, GLenum pname, GLint* params)
  {
    glGetFramebufferAttachmentParameteriv(target,attachment,pname,params);
  }

  void GetIntegerv (GLenum pname, GLint* params)
  {
    glGetIntegerv(pname,params);
  }

  void GetProgramiv (GLuint program, GLenum pname, GLint* params)
  {
    glGetProgramiv(program,pname,params);
  }

  void GetProgramInfoLog (GLuint program, GLsizei bufsize, GLsizei* length, char* infolog)
  {
    glGetProgramInfoLog(program,bufsize,length,infolog);
  }

  void GetRenderbufferParameteriv (GLenum target, GLenum pname, GLint* params)
  {
    glGetRenderbufferParameteriv(target,pname,params);
  }

  void GetShaderiv (GLuint shader, GLenum pname, GLint* params)
  {
    glGetShaderiv(shader,pname,params);
  }

  void GetShaderInfoLog (GLuint shader, GLsizei bufsize, GLsizei* length, char* infolog)
  {
    glGetShaderInfoLog(shader,bufsize,length,infolog);
  }

  void GetShaderPrecisionFormat (GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision)
  {
    glGetShaderPrecisionFormat(shadertype,precisiontype,range,precision);
  }

  void GetShaderSource (GLuint shader, GLsizei bufsize, GLsizei* length, char* source)
  {
    glGetShaderSource(shader,bufsize,length,source);
  }

  const GLubyte* GetString (GLenum name)
  {
    return glGetString(name);
  }

  void GetTexParameterfv (GLenum target, GLenum pname, GLfloat* params)
  {
    glGetTexParameterfv(target,pname,params);
  }

  void GetTexParameteriv (GLenum target, GLenum pname, GLint* params)
  {
    glGetTexParameteriv(target,pname,params);
  }

  void GetUniformfv (GLuint program, GLint location, GLfloat* params)
  {
    glGetUniformfv(program,location,params);
  }

  void GetUniformiv (GLuint program, GLint location, GLint* params)
  {
    glGetUniformiv(program,location,params);
  }

  int  GetUniformLocation (GLuint program, const char* name)
  {
    return glGetUniformLocation(program,name);
  }

  void GetVertexAttribfv (GLuint index, GLenum pname, GLfloat* params)
  {
    glGetVertexAttribfv(index,pname,params);
  }

  void GetVertexAttribiv (GLuint index, GLenum pname, GLint* params)
  {
    glGetVertexAttribiv(index,pname,params);
  }

  void GetVertexAttribPointerv (GLuint index, GLenum pname, void** pointer)
  {
    glGetVertexAttribPointerv(index,pname,pointer);
  }

  void Hint (GLenum target, GLenum mode)
  {
    glHint(target,mode);
  }

  GLboolean IsBuffer (GLuint buffer)
  {
    return glIsBuffer(buffer);
  }

  GLboolean IsEnabled (GLenum cap)
  {
    return glIsEnabled(cap);
  }

  GLboolean IsFramebuffer (GLuint framebuffer)
  {
    return glIsFramebuffer(framebuffer);
  }

  GLboolean IsProgram (GLuint program)
  {
    return glIsProgram(program);
  }

  GLboolean IsRenderbuffer (GLuint renderbuffer)
  {
    return glIsRenderbuffer(renderbuffer);
  }

  GLboolean IsShader (GLuint shader)
  {
    return glIsShader(shader);
  }

  GLboolean IsTexture (GLuint texture)
  {
    return glIsTexture(texture);
  }

  void LineWidth (GLfloat width)
  {
    glLineWidth(width);
  }

  void LinkProgram (GLuint program)
  {
    glLinkProgram(program);
  }

  void PixelStorei (GLenum pname, GLint param)
  {
    glPixelStorei(pname,param);
  }

  void PolygonOffset (GLfloat factor, GLfloat units)
  {
    glPolygonOffset(factor,units);
  }

  void ReadPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels)
  {
    glReadPixels(x,y,width,height,format,type,pixels);
  }

  void ReleaseShaderCompiler (void)
  {
    glReleaseShaderCompiler();
  }

  void RenderbufferStorage (GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
  {
    glRenderbufferStorage(target,internalformat,width,height);
  }

  void SampleCoverage (GLclampf value, GLboolean invert)
  {
    glSampleCoverage(value,invert);
  }

  void Scissor (GLint x, GLint y, GLsizei width, GLsizei height)
  {
    glScissor(x,y,width,height);
  }

  void ShaderBinary (GLsizei n, const GLuint* shaders, GLenum binaryformat, const void* binary, GLsizei length)
  {
    glShaderBinary(n,shaders,binaryformat,binary,length);
  }

  void ShaderSource (GLuint shader, GLsizei count, const char** string, const GLint* length)
  {
    glShaderSource(shader,count,string,length);
  }

  void StencilFunc (GLenum func, GLint ref, GLuint mask)
  {
    glStencilFunc(func,ref,mask);
  }

  void StencilFuncSeparate (GLenum face, GLenum func, GLint ref, GLuint mask)
  {
    glStencilFuncSeparate(face,func,ref,mask);
  }

  void StencilMask (GLuint mask)
  {
    glStencilMask(mask);
  }

  void StencilMaskSeparate (GLenum face, GLuint mask)
  {
    glStencilMaskSeparate(face,mask);
  }

  void StencilOp (GLenum fail, GLenum zfail, GLenum zpass)
  {
    glStencilOp(fail,zfail,zpass);
  }

  void StencilOpSeparate (GLenum face, GLenum fail, GLenum zfail, GLenum zpass)
  {
    glStencilOpSeparate(face,fail,zfail,zpass);
  }

  void TexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels)
  {
    glTexImage2D(target,level,internalformat,width,height,border,format,type,pixels);
  }

  void TexParameterf (GLenum target, GLenum pname, GLfloat param)
  {
    glTexParameterf(target,pname,param);
  }

  void TexParameterfv (GLenum target, GLenum pname, const GLfloat* params)
  {
    glTexParameterfv(target,pname,params);
  }

  void TexParameteri (GLenum target, GLenum pname, GLint param)
  {
    glTexParameteri(target,pname,param);
  }

  void TexParameteriv (GLenum target, GLenum pname, const GLint* params)
  {
    glTexParameteriv(target,pname,params);
  }

  void TexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels)
  {
    glTexSubImage2D(target,level,xoffset,yoffset,width,height,format,type,pixels);
  }

  void Uniform1f (GLint location, GLfloat x)
  {
    glUniform1f(location,x);
  }

  void Uniform1fv (GLint location, GLsizei count, const GLfloat* v)
  {
    glUniform1fv(location,count,v);
  }

  void Uniform1i (GLint location, GLint x)
  {
    glUniform1i(location,x);
  }

  void Uniform1iv (GLint location, GLsizei count, const GLint* v)
  {
    glUniform1iv(location,count,v);
  }

  void Uniform2f (GLint location, GLfloat x, GLfloat y)
  {
    glUniform2f(location,x,y);
  }

  void Uniform2fv (GLint location, GLsizei count, const GLfloat* v)
  {
    glUniform2fv(location,count,v);
  }

  void Uniform2i (GLint location, GLint x, GLint y)
  {
    glUniform2i(location,x,y);
  }

  void Uniform2iv (GLint location, GLsizei count, const GLint* v)
  {
    glUniform2iv(location,count,v);
  }

  void Uniform3f (GLint location, GLfloat x, GLfloat y, GLfloat z)
  {
    glUniform3f(location,x,y,z);
  }

  void Uniform3fv (GLint location, GLsizei count, const GLfloat* v)
  {
    glUniform3fv(location,count,v);
  }

  void Uniform3i (GLint location, GLint x, GLint y, GLint z)
  {
    glUniform3i(location,x,y,z);
  }

  void Uniform3iv (GLint location, GLsizei count, const GLint* v)
  {
    glUniform3iv(location,count,v);
  }

  void Uniform4f (GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
  {
    glUniform4f(location,x,y,z,w);
  }

  void Uniform4fv (GLint location, GLsizei count, const GLfloat* v)
  {
    glUniform4fv(location,count,v);
  }

  void Uniform4i (GLint location, GLint x, GLint y, GLint z, GLint w)
  {
    glUniform4i(location,x,y,z,w);
  }

  void Uniform4iv (GLint location, GLsizei count, const GLint* v)
  {
    glUniform4iv(location,count,v);
  }

  void UniformMatrix2fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
  {
    glUniformMatrix2fv(location,count,transpose,value);
  }

  void UniformMatrix3fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
  {
    glUniformMatrix3fv(location,count,transpose,value);
  }

  void UniformMatrix4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
  {
    glUniformMatrix4fv(location,count,transpose,value);
  }

  void UseProgram (GLuint program)
  {
    glUseProgram(program);
  }

  void ValidateProgram (GLuint program)
  {
    glValidateProgram(program);
  }

  void VertexAttrib1f (GLuint indx, GLfloat x)
  {
    glVertexAttrib1f(indx,x);
  }

  void VertexAttrib1fv (GLuint indx, const GLfloat* values)
  {
    glVertexAttrib1fv(indx,values);
  }

  void VertexAttrib2f (GLuint indx, GLfloat x, GLfloat y)
  {
    glVertexAttrib2f(indx,x,y);
  }

  void VertexAttrib2fv (GLuint indx, const GLfloat* values)
  {
    glVertexAttrib2fv(indx,values);
  }

  void VertexAttrib3f (GLuint indx, GLfloat x, GLfloat y, GLfloat z)
  {
    glVertexAttrib3f(indx,x,y,z);
  }

  void VertexAttrib3fv (GLuint indx, const GLfloat* values)
  {
    glVertexAttrib3fv(indx,values);
  }

  void VertexAttrib4f (GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
  {
    glVertexAttrib4f(indx,x,y,z,w);
  }

  void VertexAttrib4fv (GLuint indx, const GLfloat* values)
  {
    glVertexAttrib4fv(indx,values);
  }

  void VertexAttribPointer (GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr)
  {
    glVertexAttribPointer(indx,size,type,normalized,stride,ptr);
  }

  void Viewport (GLint x, GLint y, GLsizei width, GLsizei height)
  {
    glViewport(x,y,width,height);
  }

  /* OpenGL ES 3.0 */

  void ReadBuffer(GLenum mode)
  {
#if DALI_GLES_VERSION >= 30
    glReadBuffer(mode);
#endif // DALI_GLES_VERSION >= 30
  }

  void DrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid* indices)
  {
#if DALI_GLES_VERSION >= 30
    glDrawRangeElements(mode,start,end,count,type,indices);
#endif // DALI_GLES_VERSION >= 30
  }

  void TexImage3D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid* pixels)
  {
#if DALI_GLES_VERSION >= 30
    glTexImage3D(target,level,internalformat,width,height,depth,border,format,type,pixels);
#endif // DALI_GLES_VERSION >= 30
  }

  void TexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid* pixels)
  {
#if DALI_GLES_VERSION >= 30
    glTexSubImage3D(target,level,xoffset,yoffset,zoffset,width,height,depth,format,type,pixels);
#endif // DALI_GLES_VERSION >= 30
  }

  void CopyTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)
  {
#if DALI_GLES_VERSION >= 30
    glCopyTexSubImage3D(target,level,xoffset,yoffset,zoffset,x,y,width,height);
#endif // DALI_GLES_VERSION >= 30
  }

  void CompressedTexImage3D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid* data)
  {
#if DALI_GLES_VERSION >= 30
    glCompressedTexImage3D(target,level,internalformat,width,height,depth,border,imageSize,data);
#endif // DALI_GLES_VERSION >= 30
  }

  void CompressedTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid* data)
  {
#if DALI_GLES_VERSION >= 30
    glCompressedTexSubImage3D(target,level,xoffset,yoffset,zoffset,width,height,depth,format,imageSize,data);
#endif // DALI_GLES_VERSION >= 30
  }

  void GenQueries(GLsizei n, GLuint* ids)
  {
#if DALI_GLES_VERSION >= 30
    glGenQueries(n,ids);
#endif // DALI_GLES_VERSION >= 30
  }

  void DeleteQueries(GLsizei n, const GLuint* ids)
  {
#if DALI_GLES_VERSION >= 30
    glDeleteQueries(n,ids);
#endif // DALI_GLES_VERSION >= 30
  }

  GLboolean IsQuery(GLuint id)
  {
#if DALI_GLES_VERSION >= 30
    return glIsQuery(id);
#else
    return 0;
#endif // DALI_GLES_VERSION >= 30
  }

  void BeginQuery(GLenum target, GLuint id)
  {
#if DALI_GLES_VERSION >= 30
    glBeginQuery(target,id);
#endif // DALI_GLES_VERSION >= 30
  }

  void EndQuery(GLenum target)
  {
#if DALI_GLES_VERSION >= 30
    glEndQuery(target);
#endif // DALI_GLES_VERSION >= 30
  }

  void GetQueryiv(GLenum target, GLenum pname, GLint* params)
  {
#if DALI_GLES_VERSION >= 30
    glGetQueryiv(target,pname,params);
#endif // DALI_GLES_VERSION >= 30
  }

  void GetQueryObjectuiv(GLuint id, GLenum pname, GLuint* params)
  {
#if DALI_GLES_VERSION >= 30
    glGetQueryObjectuiv(id,pname,params);
#endif // DALI_GLES_VERSION >= 30
  }

  GLboolean UnmapBuffer(GLenum target)
  {
#if DALI_GLES_VERSION >= 30
    return glUnmapBuffer(target);
#else
    return 0;
#endif // DALI_GLES_VERSION >= 30
  }

  void GetBufferPointerv(GLenum target, GLenum pname, GLvoid** params)
  {
#if DALI_GLES_VERSION >= 30
    glGetBufferPointerv(target,pname,params);
#endif // DALI_GLES_VERSION >= 30
  }

  void DrawBuffers(GLsizei n, const GLenum* bufs)
  {
#if DALI_GLES_VERSION >= 30
    glDrawBuffers(n,bufs);
#endif // DALI_GLES_VERSION >= 30
  }

  void UniformMatrix2x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
  {
#if DALI_GLES_VERSION >= 30
    glUniformMatrix2x3fv(location,count,transpose,value);
#endif // DALI_GLES_VERSION >= 30
  }

  void UniformMatrix3x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
  {
#if DALI_GLES_VERSION >= 30
    glUniformMatrix3x2fv(location,count,transpose,value);
#endif // DALI_GLES_VERSION >= 30
  }

  void UniformMatrix2x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
  {
#if DALI_GLES_VERSION >= 30
    glUniformMatrix2x4fv(location,count,transpose,value);
#endif // DALI_GLES_VERSION >= 30
  }

  void UniformMatrix4x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
  {
#if DALI_GLES_VERSION >= 30
    glUniformMatrix4x2fv(location,count,transpose,value);
#endif // DALI_GLES_VERSION >= 30
  }

  void UniformMatrix3x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
  {
#if DALI_GLES_VERSION >= 30
    glUniformMatrix3x4fv(location,count,transpose,value);
#endif // DALI_GLES_VERSION >= 30
  }

  void UniformMatrix4x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
  {
#if DALI_GLES_VERSION >= 30
    glUniformMatrix4x3fv(location,count,transpose,value);
#endif // DALI_GLES_VERSION >= 30
  }

  void BlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)
  {
#if DALI_GLES_VERSION >= 30
    glBlitFramebuffer(srcX0,srcY0,srcX1,srcY1,dstX0,dstY0,dstX1,dstY1,mask,filter);
#endif // DALI_GLES_VERSION >= 30
  }

  void RenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)
  {
#if DALI_GLES_VERSION >= 30
    glRenderbufferStorageMultisample(target,samples,internalformat,width,height);
#endif // DALI_GLES_VERSION >= 30
  }

  void FramebufferTextureLayer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer)
  {
#if DALI_GLES_VERSION >= 30
    glFramebufferTextureLayer(target,attachment,texture,level,layer);
#endif // DALI_GLES_VERSION >= 30
  }

  GLvoid* MapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access)
  {
#if DALI_GLES_VERSION >= 30
    return glMapBufferRange(target,offset,length,access);
#else
    return NULL;
#endif // DALI_GLES_VERSION >= 30
  }

  void FlushMappedBufferRange(GLenum target, GLintptr offset, GLsizeiptr length)
  {
#if DALI_GLES_VERSION >= 30
    glFlushMappedBufferRange(target,offset,length);
#endif // DALI_GLES_VERSION >= 30
  }

  void BindVertexArray(GLuint array)
  {
#if DALI_GLES_VERSION >= 30
    glBindVertexArray(array);
#endif // DALI_GLES_VERSION >= 30
  }

  void DeleteVertexArrays(GLsizei n, const GLuint* arrays)
  {
#if DALI_GLES_VERSION >= 30
    glDeleteVertexArrays(n,arrays);
#endif // DALI_GLES_VERSION >= 30
  }

  void GenVertexArrays(GLsizei n, GLuint* arrays)
  {
#if DALI_GLES_VERSION >= 30
    glGenVertexArrays(n,arrays);
#endif // DALI_GLES_VERSION >= 30
  }

  GLboolean IsVertexArray(GLuint array)
  {
#if DALI_GLES_VERSION >= 30
    return glIsVertexArray(array);
#else
    return 0;
#endif // DALI_GLES_VERSION >= 30
  }

  void GetIntegeri_v(GLenum target, GLuint index, GLint* data)
  {
#if DALI_GLES_VERSION >= 30
    glGetIntegeri_v(target,index,data);
#endif // DALI_GLES_VERSION >= 30
  }

  void BeginTransformFeedback(GLenum primitiveMode)
  {
#if DALI_GLES_VERSION >= 30
    glBeginTransformFeedback(primitiveMode);
#endif // DALI_GLES_VERSION >= 30
  }

  void EndTransformFeedback(void)
  {
#if DALI_GLES_VERSION >= 30
    glEndTransformFeedback();
#endif // DALI_GLES_VERSION >= 30
  }

  void BindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size)
  {
#if DALI_GLES_VERSION >= 30
    glBindBufferRange(target,index,buffer,offset,size);
#endif // DALI_GLES_VERSION >= 30
  }

  void BindBufferBase(GLenum target, GLuint index, GLuint buffer)
  {
#if DALI_GLES_VERSION >= 30
    glBindBufferBase(target,index,buffer);
#endif // DALI_GLES_VERSION >= 30
  }

  void TransformFeedbackVaryings(GLuint program, GLsizei count, const GLchar* const* varyings, GLenum bufferMode)
  {
#if DALI_GLES_VERSION >= 30
    glTransformFeedbackVaryings(program,count,varyings,bufferMode);
#endif // DALI_GLES_VERSION >= 30
  }

  void GetTransformFeedbackVarying(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLsizei* size, GLenum* type, GLchar* name)
  {
#if DALI_GLES_VERSION >= 30
    glGetTransformFeedbackVarying(program,index,bufSize,length,size,type,name);
#endif // DALI_GLES_VERSION >= 30
  }

  void VertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer)
  {
#if DALI_GLES_VERSION >= 30
    glVertexAttribIPointer(index,size,type,stride,pointer);
#endif // DALI_GLES_VERSION >= 30
  }

  void GetVertexAttribIiv(GLuint index, GLenum pname, GLint* params)
  {
#if DALI_GLES_VERSION >= 30
    glGetVertexAttribIiv(index,pname,params);
#endif // DALI_GLES_VERSION >= 30
  }

  void GetVertexAttribIuiv(GLuint index, GLenum pname, GLuint* params)
  {
#if DALI_GLES_VERSION >= 30
    glGetVertexAttribIuiv(index,pname,params);
#endif // DALI_GLES_VERSION >= 30
  }

  void VertexAttribI4i(GLuint index, GLint x, GLint y, GLint z, GLint w)
  {
#if DALI_GLES_VERSION >= 30
    glVertexAttribI4i(index,x,y,z,w);
#endif // DALI_GLES_VERSION >= 30
  }

  void VertexAttribI4ui(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w)
  {
#if DALI_GLES_VERSION >= 30
    glVertexAttribI4ui(index,x,y,z,w);
#endif // DALI_GLES_VERSION >= 30
  }

  void VertexAttribI4iv(GLuint index, const GLint* v)
  {
#if DALI_GLES_VERSION >= 30
    glVertexAttribI4iv(index,v);
#endif // DALI_GLES_VERSION >= 30
  }

  void VertexAttribI4uiv(GLuint index, const GLuint* v)
  {
#if DALI_GLES_VERSION >= 30
    glVertexAttribI4uiv(index,v);
#endif // DALI_GLES_VERSION >= 30
  }

  void GetUniformuiv(GLuint program, GLint location, GLuint* params)
  {
#if DALI_GLES_VERSION >= 30
    glGetUniformuiv(program,location,params);
#endif // DALI_GLES_VERSION >= 30
  }

  GLint GetFragDataLocation(GLuint program, const GLchar *name)
  {
#if DALI_GLES_VERSION >= 30
    return glGetFragDataLocation(program,name);
#else
    return -1;
#endif // DALI_GLES_VERSION >= 30
  }

  void Uniform1ui(GLint location, GLuint v0)
  {
#if DALI_GLES_VERSION >= 30
    glUniform1ui(location,v0);
#endif // DALI_GLES_VERSION >= 30
  }

  void Uniform2ui(GLint location, GLuint v0, GLuint v1)
  {
#if DALI_GLES_VERSION >= 30
    glUniform2ui(location,v0,v1);
#endif // DALI_GLES_VERSION >= 30
  }

  void Uniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2)
  {
#if DALI_GLES_VERSION >= 30
    glUniform3ui(location,v0,v1,v2);
#endif // DALI_GLES_VERSION >= 30
  }

  void Uniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
  {
#if DALI_GLES_VERSION >= 30
    glUniform4ui(location,v0,v1,v2,v3);
#endif // DALI_GLES_VERSION >= 30
  }

  void Uniform1uiv(GLint location, GLsizei count, const GLuint* value)
  {
#if DALI_GLES_VERSION >= 30
    glUniform1uiv(location,count,value);
#endif // DALI_GLES_VERSION >= 30
  }

  void Uniform2uiv(GLint location, GLsizei count, const GLuint* value)
  {
#if DALI_GLES_VERSION >= 30
    glUniform2uiv(location,count,value);
#endif // DALI_GLES_VERSION >= 30
  }

  void Uniform3uiv(GLint location, GLsizei count, const GLuint* value)
  {
#if DALI_GLES_VERSION >= 30
    glUniform3uiv(location,count,value);
#endif // DALI_GLES_VERSION >= 30
  }

  void Uniform4uiv(GLint location, GLsizei count, const GLuint* value)
  {
#if DALI_GLES_VERSION >= 30
    glUniform4uiv(location,count,value);
#endif // DALI_GLES_VERSION >= 30
  }

  void ClearBufferiv(GLenum buffer, GLint drawbuffer, const GLint* value)
  {
#if DALI_GLES_VERSION >= 30
    glClearBufferiv(buffer,drawbuffer,value);
#endif // DALI_GLES_VERSION >= 30
  }

  void ClearBufferuiv(GLenum buffer, GLint drawbuffer, const GLuint* value)
  {
#if DALI_GLES_VERSION >= 30
    glClearBufferuiv(buffer,drawbuffer,value);
#endif // DALI_GLES_VERSION >= 30
  }

  void ClearBufferfv(GLenum buffer, GLint drawbuffer, const GLfloat* value)
  {
#if DALI_GLES_VERSION >= 30
    glClearBufferfv(buffer,drawbuffer,value);
#endif // DALI_GLES_VERSION >= 30
  }

  void ClearBufferfi(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil)
  {
#if DALI_GLES_VERSION >= 30
    glClearBufferfi(buffer,drawbuffer,depth,stencil);
#endif // DALI_GLES_VERSION >= 30
  }

  const GLubyte* GetStringi(GLenum name, GLuint index)
  {
#if DALI_GLES_VERSION >= 30
    return glGetStringi(name,index);
#else
    return NULL;
#endif // DALI_GLES_VERSION >= 30
  }

  void CopyBufferSubData(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size)
  {
#if DALI_GLES_VERSION >= 30
    glCopyBufferSubData(readTarget,writeTarget,readOffset,writeOffset,size);
#endif // DALI_GLES_VERSION >= 30
  }

  void GetUniformIndices(GLuint program, GLsizei uniformCount, const GLchar* const* uniformNames, GLuint* uniformIndices)
  {
#if DALI_GLES_VERSION >= 30
    glGetUniformIndices(program,uniformCount,uniformNames,uniformIndices);
#endif // DALI_GLES_VERSION >= 30
  }

  void GetActiveUniformsiv(GLuint program, GLsizei uniformCount, const GLuint* uniformIndices, GLenum pname, GLint* params)
  {
#if DALI_GLES_VERSION >= 30
    glGetActiveUniformsiv(program,uniformCount,uniformIndices,pname,params);
#endif // DALI_GLES_VERSION >= 30
  }

  GLuint GetUniformBlockIndex(GLuint program, const GLchar* uniformBlockName)
  {
#if DALI_GLES_VERSION >= 30
    return glGetUniformBlockIndex(program,uniformBlockName);
#else
    return 0;
#endif // DALI_GLES_VERSION >= 30
  }

  void GetActiveUniformBlockiv(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint* params)
  {
#if DALI_GLES_VERSION >= 30
    glGetActiveUniformBlockiv(program,uniformBlockIndex,pname,params);
#endif // DALI_GLES_VERSION >= 30
  }

  void GetActiveUniformBlockName(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformBlockName)
  {
#if DALI_GLES_VERSION >= 30
    glGetActiveUniformBlockName(program,uniformBlockIndex,bufSize,length,uniformBlockName);
#endif // DALI_GLES_VERSION >= 30
  }

  void UniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding)
  {
#if DALI_GLES_VERSION >= 30
    glUniformBlockBinding(program,uniformBlockIndex,uniformBlockBinding);
#endif // DALI_GLES_VERSION >= 30
  }

  void DrawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei instanceCount)
  {
#if DALI_GLES_VERSION >= 30
    glDrawArraysInstanced(mode,first,count,instanceCount);
#endif // DALI_GLES_VERSION >= 30
  }

  void DrawElementsInstanced(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices, GLsizei instanceCount)
  {
#if DALI_GLES_VERSION >= 30
    glDrawElementsInstanced(mode,count,type,indices,instanceCount);
#endif // DALI_GLES_VERSION >= 30
  }

  GLsync FenceSync(GLenum condition, GLbitfield flags)
  {
#if DALI_GLES_VERSION >= 30
    return glFenceSync(condition,flags);
#else
    return NULL;
#endif // DALI_GLES_VERSION >= 30
  }

  GLboolean IsSync(GLsync sync)
  {
#if DALI_GLES_VERSION >= 30
    return glIsSync(sync);
#else
    return 0;
#endif // DALI_GLES_VERSION >= 30
  }

  void DeleteSync(GLsync sync)
  {
#if DALI_GLES_VERSION >= 30
    glDeleteSync(sync);
#endif // DALI_GLES_VERSION >= 30
  }

  GLenum ClientWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout)
  {
#if DALI_GLES_VERSION >= 30
    return glClientWaitSync(sync,flags,timeout);
#else
    return 0;
#endif // DALI_GLES_VERSION >= 30
  }

  void WaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout)
  {
#if DALI_GLES_VERSION >= 30
    glWaitSync(sync,flags,timeout);
#endif // DALI_GLES_VERSION >= 30
  }

  void GetInteger64v(GLenum pname, GLint64* params)
  {
#if DALI_GLES_VERSION >= 30
    glGetInteger64v(pname,params);
#endif // DALI_GLES_VERSION >= 30
  }

  void GetSynciv(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei* length, GLint* values)
  {
#if DALI_GLES_VERSION >= 30
    glGetSynciv(sync,pname,bufSize,length,values);
#endif // DALI_GLES_VERSION >= 30
  }

  void GetInteger64i_v(GLenum target, GLuint index, GLint64* data)
  {
#if DALI_GLES_VERSION >= 30
    glGetInteger64i_v(target,index,data);
#endif // DALI_GLES_VERSION >= 30
  }

  void GetBufferParameteri64v(GLenum target, GLenum pname, GLint64* params)
  {
#if DALI_GLES_VERSION >= 30
    glGetBufferParameteri64v(target,pname,params);
#endif // DALI_GLES_VERSION >= 30
  }

  void GenSamplers(GLsizei count, GLuint* samplers)
  {
#if DALI_GLES_VERSION >= 30
    glGenSamplers(count,samplers);
#endif // DALI_GLES_VERSION >= 30
  }

  void DeleteSamplers(GLsizei count, const GLuint* samplers)
  {
#if DALI_GLES_VERSION >= 30
    glDeleteSamplers(count,samplers);
#endif // DALI_GLES_VERSION >= 30
  }

  GLboolean IsSampler(GLuint sampler)
  {
#if DALI_GLES_VERSION >= 30
    return glIsSampler(sampler);
#else
    return 0;
#endif // DALI_GLES_VERSION >= 30
  }

  void BindSampler(GLuint unit, GLuint sampler)
  {
#if DALI_GLES_VERSION >= 30
    glBindSampler(unit,sampler);
#endif // DALI_GLES_VERSION >= 30
  }

  void SamplerParameteri(GLuint sampler, GLenum pname, GLint param)
  {
#if DALI_GLES_VERSION >= 30
    glSamplerParameteri(sampler,pname,param);
#endif // DALI_GLES_VERSION >= 30
  }

  void SamplerParameteriv(GLuint sampler, GLenum pname, const GLint* param)
  {
#if DALI_GLES_VERSION >= 30
    glSamplerParameteriv(sampler,pname,param);
#endif // DALI_GLES_VERSION >= 30
  }

  void SamplerParameterf(GLuint sampler, GLenum pname, GLfloat param)
  {
#if DALI_GLES_VERSION >= 30
    glSamplerParameterf(sampler,pname,param);
#endif // DALI_GLES_VERSION >= 30
  }

  void SamplerParameterfv(GLuint sampler, GLenum pname, const GLfloat* param)
  {
#if DALI_GLES_VERSION >= 30
    glSamplerParameterfv(sampler,pname,param);
#endif // DALI_GLES_VERSION >= 30
  }

  void GetSamplerParameteriv(GLuint sampler, GLenum pname, GLint* params)
  {
#if DALI_GLES_VERSION >= 30
    glGetSamplerParameteriv(sampler,pname,params);
#endif // DALI_GLES_VERSION >= 30
  }

  void GetSamplerParameterfv(GLuint sampler, GLenum pname, GLfloat* params)
  {
#if DALI_GLES_VERSION >= 30
    glGetSamplerParameterfv(sampler,pname,params);
#endif // DALI_GLES_VERSION >= 30
  }

  void VertexAttribDivisor(GLuint index, GLuint divisor)
  {
#if DALI_GLES_VERSION >= 30
    glVertexAttribDivisor(index,divisor);
#endif // DALI_GLES_VERSION >= 30
  }

  void BindTransformFeedback(GLenum target, GLuint id)
  {
#if DALI_GLES_VERSION >= 30
    glBindTransformFeedback(target,id);
#endif // DALI_GLES_VERSION >= 30
  }

  void DeleteTransformFeedbacks(GLsizei n, const GLuint* ids)
  {
#if DALI_GLES_VERSION >= 30
    glDeleteTransformFeedbacks(n,ids);
#endif // DALI_GLES_VERSION >= 30
  }

  void GenTransformFeedbacks(GLsizei n, GLuint* ids)
  {
#if DALI_GLES_VERSION >= 30
    glGenTransformFeedbacks(n,ids);
#endif // DALI_GLES_VERSION >= 30
  }

  GLboolean IsTransformFeedback(GLuint id)
  {
#if DALI_GLES_VERSION >= 30
    return glIsTransformFeedback(id);
#else
    return 0;
#endif // DALI_GLES_VERSION >= 30
  }

  void PauseTransformFeedback(void)
  {
#if DALI_GLES_VERSION >= 30
    glPauseTransformFeedback();
#endif // DALI_GLES_VERSION >= 30
  }

  void ResumeTransformFeedback(void)
  {
#if DALI_GLES_VERSION >= 30
    glResumeTransformFeedback();
#endif // DALI_GLES_VERSION >= 30
  }

  void GetProgramBinary(GLuint program, GLsizei bufSize, GLsizei* length, GLenum* binaryFormat, GLvoid* binary)
  {
#if DALI_GLES_VERSION >= 30
    // if OpenGL ES 2.0 compatibility is need this can be implemented with
    // glGetProgramBinaryOES
    glGetProgramBinary(program,bufSize,length,binaryFormat,binary);
#else
    mGlExtensions.GetProgramBinaryOES(program, bufSize, length, binaryFormat, binary);
#endif // DALI_GLES_VERSION >= 30
  }

  void ProgramBinary(GLuint program, GLenum binaryFormat, const GLvoid* binary, GLsizei length)
  {
#if DALI_GLES_VERSION >= 30
    // if OpenGL ES 2.0 compatibility is need this can be implemented with
    // glProgramBinaryOES
    glProgramBinary(program,binaryFormat,binary,length);
#else
    mGlExtensions.ProgramBinaryOES(program, binaryFormat, binary, length);
#endif // DALI_GLES_VERSION >= 30
  }

  void ProgramParameteri(GLuint program, GLenum pname, GLint value)
  {
#if DALI_GLES_VERSION >= 30
    glProgramParameteri(program,pname,value);
#endif // DALI_GLES_VERSION >= 30
  }

  void InvalidateFramebuffer(GLenum target, GLsizei numAttachments, const GLenum* attachments)
  {
#if DALI_GLES_VERSION >= 30
    // if OpenGL ES 2.0 compatibility is need this can be implemented with
    // glDiscardFramebufferEXT
    glInvalidateFramebuffer(target,numAttachments,attachments);
#endif // DALI_GLES_VERSION >= 30
  }

  void InvalidateSubFramebuffer(GLenum target, GLsizei numAttachments, const GLenum* attachments, GLint x, GLint y, GLsizei width, GLsizei height)
  {
#if DALI_GLES_VERSION >= 30
    glInvalidateSubFramebuffer(target,numAttachments,attachments,x,y,width,height);
#endif // DALI_GLES_VERSION >= 30
  }

  void TexStorage2D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)
  {
#if DALI_GLES_VERSION >= 30
    glTexStorage2D(target,levels,internalformat,width,height);
#endif // DALI_GLES_VERSION >= 30
  }

  void TexStorage3D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth)
  {
#if DALI_GLES_VERSION >= 30
    glTexStorage3D(target,levels,internalformat,width,height,depth);
#endif // DALI_GLES_VERSION >= 30
  }

  void GetInternalformativ(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint* params)
  {
#if DALI_GLES_VERSION >= 30
    glGetInternalformativ(target,internalformat,pname,bufSize,params);
#endif // DALI_GLES_VERSION >= 30
  }

private:
  ECoreX::GlExtensions mGlExtensions;

};

} // namespace Adaptor

} // namespace Internal

} // namespace Dali

#endif // __DALI_INTERNAL_GL_IMPLEMENTATION_H__
