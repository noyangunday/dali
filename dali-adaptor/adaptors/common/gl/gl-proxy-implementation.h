#ifndef __DALI_INTERNAL_GL_PROXY_IMPLEMENTATION_H__
#define __DALI_INTERNAL_GL_PROXY_IMPLEMENTATION_H__

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
#include <gl/gl-implementation.h>

namespace Dali
{
namespace Internal
{
namespace Adaptor
{
class EnvironmentOptions;

/**
 * Helper class to calculate the statistics for Open GLES calls
 */
class Sampler
{
public:

  /**
   * Constructor
   * @param description to write to the log
   */
  Sampler( const char* description );

  /**
   * Increment the counter for this frame
   */
  void  Increment();

  /**
   * Reset the counter
   */
  void  Reset();

  /**
   * Accumulate the count onto statistics
   */
  void  Accumulate();

  /**
   * @return the description of the sampler
   */
  const char* GetDescription() const;

  /**
   * @return the mean value
   */
  float GetMeanValue() const;

  /**
   * @return the standard deviation
   */
  float GetStandardDeviation() const;

  /**
   * @return the minimum value
   */
  float GetMin() const;

  /**
   * @return the maximum value
   */
  float GetMax() const;

private: // Data

  const char* mDescription;
  float mAccumulated;
  float mAccumulatedSquare;
  float mMin;
  float mMax;
  unsigned int mNumSamples;
  unsigned int mCurrentFrameCount;
};

/**
 * Helper class to calculate number of OpenGL objects
 */
class ObjectCounter
{
public:
  ObjectCounter( const char* description );

  /**
   * Increment the counter
   */
  void  Increment();

  /**
   * Decrement the counter
   */
  void Decrement();

  /**
   * @return The current number of objects
   */
  unsigned int GetCount() const;

  /**
   * @return The maximum number of objects created
   */
  unsigned int GetPeak() const;

  /**
   * @return the description of the sampler
   */
  const char* GetDescription() const;

private:
  const char* mDescription;
  unsigned int mCount;
  unsigned int mPeak;
};

/**
 * GlProxyImplementation is a wrapper for the concrete implementation
 * of GlAbstraction that also gathers statistical information.
 */
class GlProxyImplementation : public GlImplementation
{
public:

  /**
   * Constructor
   * @param environmentOptions to check how often to log results
   */
  GlProxyImplementation(EnvironmentOptions& environmentOptions);

  /**
   * Virtual destructor
   */
  virtual ~GlProxyImplementation();

  /**
   * @copydoc GlAbstraction::PreRender();
   */
  virtual void PreRender();

  /**
   * @copydoc GlAbstraction::PostRender();
   */
  virtual void PostRender();

  /* OpenGL ES 2.0 API */
  virtual void Clear( GLbitfield mask );

  virtual void GenBuffers (GLsizei n, GLuint* buffers);
  virtual void DeleteBuffers (GLsizei n, const GLuint* buffers);
  virtual void BindBuffer( GLenum target, GLuint buffer );

  virtual void GenTextures (GLsizei n, GLuint* textures);
  virtual void DeleteTextures (GLsizei n, const GLuint* textures);
  virtual void ActiveTexture(GLenum texture);
  virtual void BindTexture( GLenum target, GLuint texture );

  virtual void DrawArrays( GLenum mode, GLint first, GLsizei count );
  virtual void DrawElements( GLenum mode, GLsizei count, GLenum type, const void* indices );

  virtual void Uniform1f ( GLint location, GLfloat x );
  virtual void Uniform1fv( GLint location, GLsizei count, const GLfloat* v );
  virtual void Uniform1i ( GLint location, GLint x );
  virtual void Uniform1iv( GLint location, GLsizei count, const GLint* v );
  virtual void Uniform2f ( GLint location, GLfloat x, GLfloat y );
  virtual void Uniform2fv( GLint location, GLsizei count, const GLfloat* v );
  virtual void Uniform2i ( GLint location, GLint x, GLint y );
  virtual void Uniform2iv( GLint location, GLsizei count, const GLint* v );
  virtual void Uniform3f ( GLint location, GLfloat x, GLfloat y, GLfloat z );
  virtual void Uniform3fv( GLint location, GLsizei count, const GLfloat* v );
  virtual void Uniform3i ( GLint location, GLint x, GLint y, GLint z );
  virtual void Uniform3iv( GLint location, GLsizei count, const GLint* v );
  virtual void Uniform4f ( GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w );
  virtual void Uniform4fv( GLint location, GLsizei count, const GLfloat* v );
  virtual void Uniform4i ( GLint location, GLint x, GLint y, GLint z, GLint w );
  virtual void Uniform4iv( GLint location, GLsizei count, const GLint* v );
  virtual void UniformMatrix2fv( GLint location, GLsizei count, GLboolean transpose, const GLfloat* value );
  virtual void UniformMatrix3fv( GLint location, GLsizei count, GLboolean transpose, const GLfloat* value );
  virtual void UniformMatrix4fv( GLint location, GLsizei count, GLboolean transpose, const GLfloat* value );

  virtual GLuint CreateProgram (void);
  virtual void DeleteProgram (GLuint program);
  virtual void UseProgram( GLuint program );

private: // Helpers

  void AccumulateSamples();
  void LogResults();
  void LogCalls( const Sampler& sampler );
  void LogObjectCounter( const ObjectCounter& sampler );
  void ResetSamplers();

private: // Data

  EnvironmentOptions& mEnvironmentOptions;
  Sampler mActiveTextureSampler;
  Sampler mClearSampler;
  Sampler mBindBufferSampler;
  Sampler mBindTextureSampler;
  Sampler mDrawSampler;
  Sampler mUniformSampler;
  Sampler mUseProgramSampler;
  ObjectCounter mBufferCount;
  ObjectCounter mTextureCount;
  ObjectCounter mProgramCount;

  int mFrameCount;

};

} // namespace Adaptor

} // namespace Internal

} // namespace Dali

#endif // __DALI_INTERNAL_GL_PROXY_IMPLEMENTATION_H__
