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
#include "egl-image-extensions.h"

// EXTERNAL INCLUDES
#if DALI_GLES_VERSION >= 30
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#else
#include <GLES2/gl2.h>
#endif // DALI_GLES_VERSION >= 30

#include <GLES2/gl2ext.h>

#include <EGL/eglext.h>

#include <dali/integration-api/debug.h>

// INTERNAL INCLUDES
#include <gl/egl-implementation.h>


namespace
{
// function pointers assigned in InitializeEglImageKHR
PFNEGLCREATEIMAGEKHRPROC eglCreateImageKHR = 0;
PFNEGLDESTROYIMAGEKHRPROC eglDestroyImageKHR = 0;
PFNGLEGLIMAGETARGETTEXTURE2DOESPROC glEGLImageTargetTexture2DOES = 0;
} // unnamed namespace


namespace Dali
{

namespace Internal
{

namespace Adaptor
{

EglImageExtensions::EglImageExtensions(EglImplementation* eglImpl)
: mEglImplementation(eglImpl),
  mImageKHRInitialized(false),
  mImageKHRInitializeFailed(false)
{
  DALI_ASSERT_ALWAYS( eglImpl && "EGL Implementation not instantiated" );
}

EglImageExtensions::~EglImageExtensions()
{
}

void* EglImageExtensions::CreateImageKHR(EGLClientBuffer pixmap)
{
  if (mImageKHRInitialized == false)
  {
    InitializeEglImageKHR();
  }

  if (mImageKHRInitialized == false)
  {
    return NULL;
  }

  // Use the EGL image extension
  const EGLint attribs[] =
  {
    EGL_IMAGE_PRESERVED_KHR, EGL_TRUE,
    EGL_NONE
  };

  EGLImageKHR eglImage  = eglCreateImageKHR( mEglImplementation->GetDisplay(),
                                             EGL_NO_CONTEXT,
                                             EGL_NATIVE_PIXMAP_KHR,
                                             pixmap,
                                             attribs );

  DALI_ASSERT_DEBUG( EGL_NO_IMAGE_KHR != eglImage && "X11Image::GlExtensionCreate eglCreateImageKHR failed!\n");
  if( EGL_NO_IMAGE_KHR == eglImage )
  {
    switch( eglGetError() )
    {
      case EGL_SUCCESS :
      {
        break;
      }
      case EGL_BAD_DISPLAY:
      {
        DALI_LOG_ERROR( "EGL_BAD_DISPLAY: Invalid EGLDisplay object" );
        break;
      }
      case EGL_BAD_CONTEXT:
      {
        DALI_LOG_ERROR( "EGL_BAD_CONTEXT: Invalid EGLContext object" );
        break;
      }
      case EGL_BAD_PARAMETER:
      {
        DALI_LOG_ERROR( "EGL_BAD_PARAMETER: Invalid target parameter or attribute in attrib_list" );
        break;
      }
      case EGL_BAD_MATCH:
      {
        DALI_LOG_ERROR( "EGL_BAD_MATCH: attrib_list does not match target" );
        break;
      }
      case EGL_BAD_ACCESS:
      {
        DALI_LOG_ERROR( "EGL_BAD_ACCESS: Previously bound off-screen, or EGLImage sibling error" );
        break;
      }
      case EGL_BAD_ALLOC:
      {
        DALI_LOG_ERROR( "EGL_BAD_ALLOC: Insufficient memory is available" );
        break;
      }
      default:
      {
        break;
      }
    }
  }

  return (void*)eglImage;
}

void EglImageExtensions::DestroyImageKHR(void* eglImageKHR)
{
  DALI_ASSERT_DEBUG( mImageKHRInitialized );

  if( ! mImageKHRInitialized )
  {
    return;
  }

  if( eglImageKHR == NULL )
  {
    return;
  }

  EGLImageKHR eglImage = static_cast<EGLImageKHR>(eglImageKHR);

  EGLBoolean result = eglDestroyImageKHR(mEglImplementation->GetDisplay(), eglImage);

  if( EGL_FALSE == result )
  {
    switch( eglGetError() )
    {
      case EGL_BAD_DISPLAY:
      {
        DALI_LOG_ERROR( "EGL_BAD_DISPLAY: Invalid EGLDisplay object" );
        break;
      }
      case EGL_BAD_PARAMETER:
      {
        DALI_LOG_ERROR( "EGL_BAD_PARAMETER: eglImage is not a valid EGLImageKHR object created with respect to EGLDisplay" );
        break;
      }
      case EGL_BAD_ACCESS:
      {
        DALI_LOG_ERROR( "EGL_BAD_ACCESS: EGLImage sibling error" );
        break;
      }
      default:
      {
        break;
      }
    }
  }
}

void EglImageExtensions::TargetTextureKHR(void* eglImageKHR)
{
  DALI_ASSERT_DEBUG( mImageKHRInitialized );

  if( eglImageKHR != NULL )
  {
    EGLImageKHR eglImage = static_cast<EGLImageKHR>(eglImageKHR);

#ifdef EGL_ERROR_CHECKING
    GLint glError = glGetError();
#endif

    glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, (GLeglImageOES)eglImage);

#ifdef EGL_ERROR_CHECKING
    glError = glGetError();
    if( GL_NO_ERROR != glError )
    {
      DALI_LOG_ERROR(" glEGLImageTargetTexture2DOES returned error %0x04x\n", glError );
    }
#endif
  }
}

void EglImageExtensions::InitializeEglImageKHR()
{
  // avoid trying to reload extended KHR functions, if it fails the first time
  if( ! mImageKHRInitializeFailed )
  {
    eglCreateImageKHR  = (PFNEGLCREATEIMAGEKHRPROC) eglGetProcAddress("eglCreateImageKHR"); /* parasoft-suppress MISRA2004-11_1_DMC "Using EGL defined functions." */
    eglDestroyImageKHR = (PFNEGLDESTROYIMAGEKHRPROC) eglGetProcAddress("eglDestroyImageKHR"); /* parasoft-suppress MISRA2004-11_1_DMC "Using EGL defined functions." */
    glEGLImageTargetTexture2DOES = (PFNGLEGLIMAGETARGETTEXTURE2DOESPROC) eglGetProcAddress("glEGLImageTargetTexture2DOES"); /* parasoft-suppress MISRA2004-11_1_DMC "Using EGL defined functions." */
  }

  if (eglCreateImageKHR && eglDestroyImageKHR && glEGLImageTargetTexture2DOES)
  {
    mImageKHRInitialized = true;
  }
  else
  {
    mImageKHRInitializeFailed = true;
  }
}

} // namespace Adaptor

} // namespace Internal

} // namespace Dali
