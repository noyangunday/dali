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
#include "indicator-impl.h"

// EXTERNAL INCLUDES
#include <Ecore.h>
#include <Evas.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <dali/public-api/images/native-image.h>
#include <dali/public-api/events/touch-event.h>
#include <dali/public-api/events/touch-point.h>
#include <dali/public-api/common/stage.h>
#include <dali/public-api/actors/blending.h>
#include <dali/public-api/shader-effects/shader-effect.h>
#include <dali/public-api/images/buffer-image.h>

#include <dali/integration-api/debug.h>

// INTERNAL INCLUDES
#include <adaptor-impl.h>
#include <accessibility-adaptor-impl.h>
#include <pixmap-image.h>

using Dali::Vector4;

#if defined(DEBUG_ENABLED)
#define STATE_DEBUG_STRING(state) (state==DISCONNECTED?"DISCONNECTED":state==CONNECTED?"CONNECTED":"UNKNOWN")
#endif

namespace
{

const float SLIDING_ANIMATION_DURATION( 0.2f ); // 200 milli seconds
const float AUTO_INDICATOR_STAY_DURATION(3.0f); // 3 seconds
const float SHOWING_DISTANCE_HEIGHT_RATE(0.34f); // 20 pixels

enum
{
  KEEP_SHOWING = -1,
  HIDE_NOW = 0
};

const int NUM_GRADIENT_INTERVALS(5); // Number of gradient intervals
const float GRADIENT_ALPHA[NUM_GRADIENT_INTERVALS+1] = { 0.6f, 0.38f, 0.20f, 0.08f, 0.0f, 0.0f };

#define MAKE_SHADER(A)#A

const char* BACKGROUND_VERTEX_SHADER = MAKE_SHADER(
  attribute mediump vec2 aPosition;
  attribute mediump float aAlpha;
  varying mediump float vAlpha;
  uniform mediump mat4 uMvpMatrix;
  uniform mediump vec3 uSize;

  void main()
  {
    mediump vec4 vertexPosition = vec4( aPosition * uSize.xy, 0.0, 1.0 );
    vertexPosition = uMvpMatrix * vertexPosition;

    vAlpha = aAlpha;
    gl_Position = vertexPosition;
  }
);

const char* BACKGROUND_FRAGMENT_SHADER = MAKE_SHADER(
  uniform lowp vec4 uColor;
  varying mediump float vAlpha;

  void main()
  {
    gl_FragColor = uColor;
    gl_FragColor.a *= vAlpha;
  }
);


const float OPAQUE_THRESHOLD(0.99f);
const float TRANSPARENT_THRESHOLD(0.05f);

// indicator service name
const char* INDICATOR_SERVICE_NAME("elm_indicator");

// Copied from ecore_evas_extn_engine.h

enum // opcodes
{
   OP_RESIZE,
   OP_SHOW,
   OP_HIDE,
   OP_FOCUS,
   OP_UNFOCUS,
   OP_UPDATE,
   OP_UPDATE_DONE,
   OP_SHM_REF0,
   OP_SHM_REF1,
   OP_SHM_REF2,
   OP_PROFILE_CHANGE_REQUEST,
   OP_PROFILE_CHANGE_DONE,
   OP_EV_MOUSE_IN,
   OP_EV_MOUSE_OUT,
   OP_EV_MOUSE_UP,
   OP_EV_MOUSE_DOWN,
   OP_EV_MOUSE_MOVE,
   OP_EV_MOUSE_WHEEL,
   OP_EV_MULTI_UP,
   OP_EV_MULTI_DOWN,
   OP_EV_MULTI_MOVE,
   OP_EV_KEY_UP,
   OP_EV_KEY_DOWN,
   OP_EV_HOLD,
   OP_MSG_PARENT,
   OP_MSG
};

// Copied from elm_conform.c

const int MSG_DOMAIN_CONTROL_INDICATOR( 0x10001 );
const int MSG_ID_INDICATOR_REPEAT_EVENT( 0x10002 );
const int MSG_ID_INDICATOR_ROTATION( 0x10003 );
const int MSG_ID_INDICATOR_OPACITY( 0X1004 );
const int MSG_ID_INDICATOR_TYPE( 0X1005 );
const int MSG_ID_INDICATOR_START_ANIMATION( 0X10006 );

struct IpcDataUpdate
{
   int x, w, y, h;
};

struct IpcDataResize
{
  int w, h;
};

struct IpcIndicatorDataAnimation
{
  unsigned int xwin;
  double       duration;
};

struct IpcDataEvMouseUp
{
  int               b;
  Evas_Button_Flags flags;
  int               mask;
  unsigned int      timestamp;
  Evas_Event_Flags  event_flags;

  IpcDataEvMouseUp(unsigned long timestamp)
  : b(1),
    flags(EVAS_BUTTON_NONE),
    mask(0),
    timestamp(static_cast<unsigned int>(timestamp)),
    event_flags(EVAS_EVENT_FLAG_NONE)
  {
  }
};

struct IpcDataEvMouseDown
{
  int                b;
  Evas_Button_Flags  flags;
  int                mask;
  unsigned int       timestamp;
  Evas_Event_Flags   event_flags;

  IpcDataEvMouseDown(unsigned long timestamp)
  : b(1),
    flags(EVAS_BUTTON_NONE),
    mask(0),
    timestamp(static_cast<unsigned int>(timestamp)),
    event_flags(EVAS_EVENT_FLAG_NONE)
  {
  }
};

struct IpcDataEvMouseMove
{
  int                x, y;
  Evas_Button_Flags  flags;
  int                mask;
  unsigned int       timestamp;
  Evas_Event_Flags   event_flags;

  IpcDataEvMouseMove(const Dali::TouchPoint& touchPoint, unsigned long timestamp)
  : x(static_cast<Evas_Coord>(touchPoint.local.x)),
    y(static_cast<Evas_Coord>(touchPoint.local.y)),
    flags(EVAS_BUTTON_NONE),
    mask(0),
    timestamp(static_cast<unsigned int>(timestamp)),
    event_flags(EVAS_EVENT_FLAG_NONE)
  {
  }
};

struct IpcDataEvMouseOut
{
  unsigned int     timestamp;
  int              mask;
  Evas_Event_Flags event_flags;

  IpcDataEvMouseOut(unsigned long timestamp)
  : timestamp(static_cast<unsigned int>(timestamp)),
    mask(0),
    event_flags(EVAS_EVENT_FLAG_NONE)
  {
  }
};

} // anonymous namespace


namespace Dali
{
namespace Internal
{
namespace Adaptor
{
#if defined(DEBUG_ENABLED)
Debug::Filter* gIndicatorLogFilter = Debug::Filter::New(Debug::Concise, false, "LOG_INDICATOR");
#endif


Indicator::LockFile::LockFile(const std::string filename)
: mFilename(filename),
  mErrorThrown(false)
{
  mFileDescriptor = open(filename.c_str(), O_RDWR);
  if( mFileDescriptor == -1 )
  {
    mFileDescriptor = 0;
    mErrorThrown = true;
    DALI_LOG_ERROR( "### Cannot open %s for indicator lock ###\n", mFilename.c_str() );
  }
}

Indicator::LockFile::~LockFile()
{
  // Closing file descriptor also unlocks file.
  close( mFileDescriptor );
}

bool Indicator::LockFile::Lock()
{
  DALI_LOG_TRACE_METHOD( gIndicatorLogFilter );

  bool locked = false;
  if( mFileDescriptor > 0 )
  {
    if( lockf( mFileDescriptor, F_LOCK, 0 ) == 0 ) // Note, operation may block.
    {
      locked = true;
    }
    else
    {
      if( errno == EBADF )
      {
        // file descriptor is no longer valid or not writable
        mFileDescriptor = 0;
        mErrorThrown = true;
        DALI_LOG_ERROR( "### Cannot lock indicator: bad file descriptor for %s ###\n", mFilename.c_str() );
      }
    }
  }

  return locked;
}

void Indicator::LockFile::Unlock()
{
  DALI_LOG_TRACE_METHOD( gIndicatorLogFilter );
  if( lockf( mFileDescriptor, F_ULOCK, 0 ) != 0 )
  {
    if( errno == EBADF )
    {
      // file descriptor is no longer valid or not writable
      mFileDescriptor = 0;
      mErrorThrown = true;
      DALI_LOG_ERROR( "### Cannot unlock indicator: bad file descriptor for %s\n", mFilename.c_str() );
    }
  }
}

bool Indicator::LockFile::RetrieveAndClearErrorStatus()
{
  bool error = mErrorThrown;
  mErrorThrown = false;
  return error;
}

Indicator::ScopedLock::ScopedLock(LockFile* lockFile)
: mLockFile(lockFile),
  mLocked(false)
{
  if(mLockFile)
  {
    mLocked = mLockFile->Lock();
  }
}

Indicator::ScopedLock::~ScopedLock()
{
  if( mLockFile )
  {
    mLockFile->Unlock();
  }
}

bool Indicator::ScopedLock::IsLocked()
{
  return mLocked;
}

Indicator::Indicator( Adaptor* adaptor, Dali::Window::WindowOrientation orientation, Observer* observer )
: mPixmap( 0 ),
  mGestureDetected( false ),
  mConnection( this ),
  mOpacityMode( Dali::Window::OPAQUE ),
  mState( DISCONNECTED ),
  mAdaptor(adaptor),
  mServerConnection( NULL ),
  mObserver( observer ),
  mOrientation( orientation ),
  mImageWidth( 0 ),
  mImageHeight( 0 ),
  mVisible( Dali::Window::INVISIBLE ),
  mIsShowing( true ),
  mIsAnimationPlaying( false ),
  mCurrentSharedFile( 0 )
{
  mIndicatorImageActor = Dali::ImageActor::New();
  mIndicatorImageActor.SetBlendFunc( Dali::BlendingFactor::ONE, Dali::BlendingFactor::ONE_MINUS_SRC_ALPHA,
                                    Dali::BlendingFactor::ONE, Dali::BlendingFactor::ONE );

  mIndicatorImageActor.SetParentOrigin( ParentOrigin::TOP_CENTER );
  mIndicatorImageActor.SetAnchorPoint( AnchorPoint::TOP_CENTER );
  mIndicatorImageActor.SetSortModifier( 1.0f );

  // Indicator image handles the touch event including "leave"
  mIndicatorImageActor.SetLeaveRequired( true );
  mIndicatorImageActor.TouchedSignal().Connect( this, &Indicator::OnTouched );

  mBackgroundActor = Dali::Actor::New();
  mBackgroundActor.SetParentOrigin( ParentOrigin::TOP_CENTER );
  mBackgroundActor.SetAnchorPoint( AnchorPoint::TOP_CENTER );
  mBackgroundActor.SetColor( Color::BLACK );

  mIndicatorImageContainerActor = Dali::Actor::New();
  mIndicatorImageContainerActor.SetParentOrigin( ParentOrigin::TOP_CENTER );
  mIndicatorImageContainerActor.SetAnchorPoint( AnchorPoint::TOP_CENTER );
  mIndicatorImageContainerActor.Add( mBackgroundActor );
  mIndicatorImageContainerActor.Add( mIndicatorImageActor );

  mIndicatorActor = Dali::Actor::New();
  mIndicatorActor.Add( mIndicatorImageContainerActor );

  if( mOrientation == Dali::Window::LANDSCAPE || mOrientation == Dali::Window::LANDSCAPE_INVERSE )
  {
    mBackgroundActor.SetVisible( false );
  }

  // Event handler to find out flick down gesture
  mEventActor = Dali::Actor::New();
  mEventActor.SetParentOrigin( ParentOrigin::TOP_CENTER );
  mEventActor.SetAnchorPoint( AnchorPoint::TOP_CENTER );
  mIndicatorActor.Add( mEventActor );

  // Attach pan gesture to find flick down during hiding.
  // It can prevent the problem that scrollview gets pan gesture even indicator area is touched,
  // since it consumes the pan gesture in advance.
  mPanDetector = Dali::PanGestureDetector::New();
  mPanDetector.DetectedSignal().Connect( this, &Indicator::OnPan );
  mPanDetector.Attach( mEventActor );

  Open( orientation );

  // register indicator to accessibility adaptor
  Dali::AccessibilityAdaptor accessibilityAdaptor = AccessibilityAdaptor::Get();
  if(accessibilityAdaptor)
  {
    AccessibilityAdaptor::GetImplementation( accessibilityAdaptor ).SetIndicator( this );
  }
  // hide the indicator by default
  mIndicatorActor.SetVisible( false );
}

Indicator::~Indicator()
{
  if(mEventActor)
  {
    mEventActor.TouchedSignal().Disconnect( this, &Indicator::OnTouched );
  }
  Disconnect();
}

void Indicator::SetAdaptor(Adaptor* adaptor)
{
  mAdaptor = adaptor;
  mIndicatorBuffer->SetAdaptor( adaptor );
}

Dali::Actor Indicator::GetActor()
{
  return mIndicatorActor;
}

void Indicator::Open( Dali::Window::WindowOrientation orientation )
{
  DALI_LOG_TRACE_METHOD( gIndicatorLogFilter );

  // Calls from Window should be set up to ensure we are in a
  // disconnected state before opening a second time.
  DALI_ASSERT_DEBUG( mState == DISCONNECTED );

  mOrientation = orientation;

  Connect();

  // Change background visibility depending on orientation
  if(mOrientation == Dali::Window::LANDSCAPE || mOrientation == Dali::Window::LANDSCAPE_INVERSE  )
  {
    mBackgroundActor.SetVisible( false );
  }
  else
  {
    SetOpacityMode( mOpacityMode );
  }
}

void Indicator::Close()
{
  DALI_LOG_TRACE_METHOD_FMT( gIndicatorLogFilter, "State: %s", STATE_DEBUG_STRING(mState) );

  if( mState == CONNECTED )
  {
    Disconnect();
    if( mObserver != NULL )
    {
      mObserver->IndicatorClosed( this );
    }
  }

  Dali::Image emptyImage;
  mIndicatorImageActor.SetImage(emptyImage);
}

void Indicator::SetOpacityMode( Dali::Window::IndicatorBgOpacity mode )
{
  mOpacityMode = mode;

  //@todo replace with a gradient renderer when that is implemented
  Dali::Geometry geometry = CreateBackgroundGeometry();
  if( geometry )
  {
    mBackgroundActor.SetVisible( true );

    if( mBackgroundActor.GetRendererCount() > 0 )
    {
      Dali::Renderer renderer = mBackgroundActor.GetRendererAt( 0 );
      if( renderer )
      {
        if( renderer.GetGeometry() == geometry )
        {
          return;
        }
        else
        {
          renderer.SetGeometry( geometry );
        }
      }
    }
    else
    {
      if( !mBackgroundMaterial )
      {
        Dali::Shader shader = Dali::Shader::New( BACKGROUND_VERTEX_SHADER, BACKGROUND_FRAGMENT_SHADER, Dali::Shader::HINT_OUTPUT_IS_TRANSPARENT );
        mBackgroundMaterial = Dali::Material::New( shader );
      }

      Dali::Renderer renderer = Dali::Renderer::New( geometry, mBackgroundMaterial );

      mBackgroundActor.AddRenderer( renderer );
    }
  }
  else
  {
    mBackgroundActor.SetVisible( false );
  }
}

void Indicator::SetVisible( Dali::Window::IndicatorVisibleMode visibleMode, bool forceUpdate )
{
  if ( visibleMode != mVisible || forceUpdate )
  {
    // If we were previously hidden, then we should update the image data before we display the indicator
    if ( mVisible == Dali::Window::INVISIBLE )
    {
      UpdateImageData( mCurrentSharedFile );
    }
    if ( visibleMode != Dali::Window::INVISIBLE )
    {
      mIndicatorActor.SetVisible( true );
    }

    mVisible = visibleMode;

    if( mIndicatorImageActor.GetImage() )
    {
      if( CheckVisibleState() && mVisible == Dali::Window::AUTO )
      {
        // hide indicator
        ShowIndicator( AUTO_INDICATOR_STAY_DURATION /* stay n sec */ );
      }
      else if( CheckVisibleState() && mVisible == Dali::Window::VISIBLE )
      {
        // show indicator
        ShowIndicator( KEEP_SHOWING );
      }
      else
      {
        // hide indicator
        ShowIndicator( HIDE_NOW );
      }
    }
  }
}

bool Indicator::IsConnected()
{
  return ( mState == CONNECTED );
}

bool Indicator::SendMessage( int messageDomain, int messageId, const void *data, int size )
{
 if(IsConnected())
 {
   return mServerConnection->SendEvent( OP_MSG, messageDomain, messageId, data, size );
 }
 else
 {
   return false;
 }
}

bool Indicator::OnTouched(Dali::Actor indicator, const Dali::TouchEvent& touchEvent)
{
  if( mServerConnection )
  {
    const TouchPoint& touchPoint = touchEvent.GetPoint( 0 );

    // Send touch event to indicator server when indicator is showing
    if( CheckVisibleState() || mIsShowing )
    {
      switch( touchPoint.state )
      {
        case Dali::TouchPoint::Down:
        {
          IpcDataEvMouseMove ipcMove( touchPoint, touchEvent.time );
          IpcDataEvMouseDown ipcDown( touchEvent.time );
          mServerConnection->SendEvent( OP_EV_MOUSE_MOVE, &ipcMove, sizeof(ipcMove) );
          mServerConnection->SendEvent( OP_EV_MOUSE_DOWN, &ipcDown, sizeof(ipcDown) );

          if( mVisible == Dali::Window::AUTO )
          {
            // Stop hiding indicator
            ShowIndicator( KEEP_SHOWING );
          }
        }
        break;

        case Dali::TouchPoint::Motion:
        {
          IpcDataEvMouseMove ipcMove( touchPoint, touchEvent.time );
          mServerConnection->SendEvent( OP_EV_MOUSE_MOVE, &ipcMove, sizeof(ipcMove) );
        }
        break;

        case Dali::TouchPoint::Up:
        {
          IpcDataEvMouseUp ipcUp( touchEvent.time );
          mServerConnection->SendEvent( OP_EV_MOUSE_UP, &ipcUp, sizeof(ipcUp) );

          if( mVisible == Dali::Window::AUTO )
          {
            // Hide indicator
            ShowIndicator( 0.5f /* hide after 0.5 sec */ );
          }
        }
        break;

        case Dali::TouchPoint::Leave:
        {
          IpcDataEvMouseMove ipcMove( touchPoint, touchEvent.time );
          mServerConnection->SendEvent( OP_EV_MOUSE_MOVE, &ipcMove, sizeof(ipcMove) );
          IpcDataEvMouseUp ipcOut( touchEvent.time );
          mServerConnection->SendEvent( OP_EV_MOUSE_OUT, &ipcOut, sizeof(ipcOut) );
        }
        break;

        default:
          break;
      }

      return true;
    }
  }

  return false;
}

bool Indicator::Connect()
{
  DALI_LOG_TRACE_METHOD( gIndicatorLogFilter );

  DALI_ASSERT_DEBUG( mState == DISCONNECTED );

  bool connected = false;

  mServerConnection = new ServerConnection( INDICATOR_SERVICE_NAME, 0, false, this );
  if( mServerConnection )
  {
    connected = mServerConnection->IsConnected();
    if( ! connected )
    {
      delete mServerConnection;
      mServerConnection = NULL;
    }
  }

  if( !connected )
  {
    StartReconnectionTimer();
  }
  else
  {
    mState = CONNECTED;
  }

  return connected;
}

void Indicator::StartReconnectionTimer()
{
  if( ! mReconnectTimer )
  {
    mReconnectTimer = Dali::Timer::New(1000);
    mConnection.DisconnectAll();
    mReconnectTimer.TickSignal().Connect( mConnection, &Indicator::OnReconnectTimer );
  }
  mReconnectTimer.Start();
}

bool Indicator::OnReconnectTimer()
{
  bool retry = false;

  if( mState == DISCONNECTED )
  {
    if( !Connect() )
    {
      retry = true;
    }
  }

  return retry;
}

void Indicator::Disconnect()
{
  DALI_LOG_TRACE_METHOD( gIndicatorLogFilter );

  mState = DISCONNECTED;

  delete mServerConnection;
  mServerConnection = NULL;

  ClearSharedFileInfo();
}

void Indicator::Resize( int width, int height )
{
  if( width < 1 )
  {
    width = 1;
  }
  if( height < 1 )
  {
    height = 1;
  }

  if( mImageWidth != width || mImageHeight != height )
  {
    mImageWidth = width;
    mImageHeight = height;

    mIndicatorImageActor.SetSize( mImageWidth, mImageHeight );
    mIndicatorActor.SetSize( mImageWidth, mImageHeight );
    mEventActor.SetSize(mImageWidth, mImageHeight);
    mBackgroundActor.SetSize( mImageWidth, mImageHeight );
    mIndicatorImageContainerActor.SetSize( mImageWidth, mImageHeight );
  }
}

void Indicator::SetLockFileInfo( Ecore_Ipc_Event_Server_Data *epcEvent )
{
  DALI_LOG_TRACE_METHOD( gIndicatorLogFilter );

  // epcEvent->ref == w
  // epcEvent->ref_to == h
  // epcEvent->response == buffer num
  // epcEvent->data = lockfile + nul byte

  if( (epcEvent->ref > 0) && (epcEvent->ref_to > 0) && (epcEvent->data) &&
      (((unsigned char *)epcEvent->data)[epcEvent->size - 1] == 0) )
  {
    int n = epcEvent->response;

    if( n >= 0 && n < SHARED_FILE_NUMBER )
    {
      mCurrentSharedFile = n;

      mSharedFileInfo[n].mImageWidth  = epcEvent->ref;
      mSharedFileInfo[n].mImageHeight = epcEvent->ref_to;

      mSharedFileInfo[n].mLockFileName.clear();

      mSharedFileInfo[n].mLockFileName = static_cast< char* >( epcEvent->data );

      DALI_LOG_INFO( gIndicatorLogFilter, Debug::General, "SetLockFileInfo: buffer num = %d, w = %d, h = %d, lock = %s\n",
                     n, mSharedFileInfo[n].mImageWidth, mSharedFileInfo[n].mImageHeight, mSharedFileInfo[n].mLockFileName.c_str() );
    }
  }
}

void Indicator::SetSharedImageInfo( Ecore_Ipc_Event_Server_Data *epcEvent )
{
  DALI_LOG_TRACE_METHOD( gIndicatorLogFilter );

  // epcEvent->ref == shm id
  // epcEvent->ref_to == shm num
  // epcEvent->response == buffer num
  // epcEvent->data = shm ref string + nul byte

  if ( (epcEvent->data) &&
       (epcEvent->size > 0) &&
       (((unsigned char *)epcEvent->data)[epcEvent->size - 1] == 0) )
  {
    int n = epcEvent->response;

    if( n >= 0 && n < SHARED_FILE_NUMBER )
    {
      mCurrentSharedFile = n;

      mSharedFileInfo[n].mSharedFileName.clear();

      mSharedFileInfo[n].mSharedFileName = static_cast< char* >( epcEvent->data );

      mSharedFileInfo[n].mSharedFileID = epcEvent->ref;
      mSharedFileInfo[n].mSharedFileNumber = epcEvent->ref_to;

      DALI_LOG_INFO( gIndicatorLogFilter, Debug::General, "SetSharedImageInfo: buffer num %d, shared file = %s, id = %d, num = %d\n",
                     n, mSharedFileInfo[n].mSharedFileName.c_str(), mSharedFileInfo[n].mSharedFileID, mSharedFileInfo[n].mSharedFileNumber );
    }
  }
}

void Indicator::LoadSharedImage( Ecore_Ipc_Event_Server_Data *epcEvent )
{
  DALI_LOG_TRACE_METHOD( gIndicatorLogFilter );

  // epcEvent->ref == alpha
  // epcEvent->ref_to == sys
  // epcEvent->response == buffer num

  int n = epcEvent->response;

  if( n >= 0 && n < SHARED_FILE_NUMBER )
  {
    mCurrentSharedFile = n;

    delete mSharedFileInfo[n].mSharedFile;
    mSharedFileInfo[n].mSharedFile = NULL;

    delete mSharedFileInfo[n].mLock;
    mSharedFileInfo[n].mLock = NULL;

    std::stringstream sharedFileID;
    std::stringstream sharedFileNumber;

    sharedFileID << mSharedFileInfo[n].mSharedFileID;
    sharedFileNumber << mSharedFileInfo[n].mSharedFileNumber;

    std::string sharedFilename = "/" + mSharedFileInfo[n].mSharedFileName + "-" + sharedFileID.str() + "." + sharedFileNumber.str();

    DALI_LOG_INFO( gIndicatorLogFilter, Debug::General, "LoadSharedImage: file name = %s\n", sharedFilename.c_str() );

    mSharedFileInfo[n].mSharedFile = SharedFile::New( sharedFilename.c_str(), mSharedFileInfo[n].mImageWidth * mSharedFileInfo[n].mImageWidth * 4, true );
    if( mSharedFileInfo[n].mSharedFile != NULL )
    {
      mSharedFileInfo[n].mLock = new Indicator::LockFile( mSharedFileInfo[n].mLockFileName );
      if( mSharedFileInfo[n].mLock->RetrieveAndClearErrorStatus() )
      {
        DALI_LOG_ERROR( "### Indicator error: Cannot open lock file %s ###\n", mSharedFileInfo[n].mLockFileName.c_str() );
      }

      CreateNewImage( n );

      if( CheckVisibleState() )
      {
        // set default indicator type (enable the quick panel)
        OnIndicatorTypeChanged( INDICATOR_TYPE_1 );
      }
      else
      {
        // set default indicator type (disable the quick panel)
        OnIndicatorTypeChanged( INDICATOR_TYPE_2 );
      }

      SetVisible(mVisible, true);
    }
  }
}

void Indicator::LoadPixmapImage( Ecore_Ipc_Event_Server_Data *epcEvent )
{
  DALI_LOG_TRACE_METHOD( gIndicatorLogFilter );

  // epcEvent->ref == w
  // epcEvent->ref_to == h
  // epcEvent->response == alpha
  // epcEvent->data = pixmap id

  if( ( epcEvent->data ) &&
      (epcEvent->size >= (int)sizeof(PixmapId)) )
  {
    ClearSharedFileInfo();

    if( (epcEvent->ref > 0) && (epcEvent->ref_to > 0) )
    {
      mImageWidth  = epcEvent->ref;
      mImageHeight = epcEvent->ref_to;

      mPixmap = *(static_cast<PixmapId*>(epcEvent->data));
      CreateNewPixmapImage();

      if( CheckVisibleState() )
      {
        // set default indicator type (enable the quick panel)
        OnIndicatorTypeChanged( INDICATOR_TYPE_1 );
      }
      else
      {
        // set default indicator type (disable the quick panel)
        OnIndicatorTypeChanged( INDICATOR_TYPE_2 );
      }

      SetVisible(mVisible, true);
    }
  }
}

void Indicator::UpdateImageData( int bufferNumber )
{
  DALI_LOG_TRACE_METHOD_FMT( gIndicatorLogFilter, "State: %s  mVisible: %s", STATE_DEBUG_STRING(mState), mVisible?"T":"F" );

  if( mState == CONNECTED && mVisible )
  {
    if(mPixmap == 0)
    {
      // in case of shm indicator (not pixmap), not sure we can skip it when mIsShowing is false
      CopyToBuffer( bufferNumber );
    }
    else
    {
      if(mIsShowing)
      {
        mAdaptor->RequestUpdateOnce();
      }
    }
  }
}

bool Indicator::CopyToBuffer( int bufferNumber )
{
  bool success = false;

  if( mSharedFileInfo[bufferNumber].mLock )
  {
    Indicator::ScopedLock scopedLock(mSharedFileInfo[bufferNumber].mLock);
    if( mSharedFileInfo[bufferNumber].mLock->RetrieveAndClearErrorStatus() )
    {
      // Do nothing here.
    }
    else if( scopedLock.IsLocked() )
    {
      unsigned char *src = mSharedFileInfo[bufferNumber].mSharedFile->GetAddress();
      size_t size = mSharedFileInfo[bufferNumber].mImageWidth * mSharedFileInfo[bufferNumber].mImageHeight * 4;

      if( mIndicatorBuffer->UpdatePixels( src, size ) )
      {
        mAdaptor->RequestUpdateOnce();
        success = true;
      }
    }
  }

  return success;
}

void Indicator::CreateNewPixmapImage()
{
  DALI_LOG_TRACE_METHOD_FMT( gIndicatorLogFilter, "W:%d H:%d", mImageWidth, mImageHeight );
  Dali::PixmapImagePtr pixmapImage = Dali::PixmapImage::New( mPixmap );

  if( pixmapImage )
  {
    mIndicatorImageActor.SetImage( Dali::NativeImage::New(*pixmapImage) );
    mIndicatorImageActor.SetSize( mImageWidth, mImageHeight );
    mIndicatorActor.SetSize( mImageWidth, mImageHeight );
    mEventActor.SetSize(mImageWidth, mImageHeight);
    mBackgroundActor.SetSize( mImageWidth, mImageHeight );
    mIndicatorImageContainerActor.SetSize( mImageWidth, mImageHeight );
  }
  else
  {
    DALI_LOG_WARNING("### Cannot create indicator image - disconnecting ###\n");
    Disconnect();
    if( mObserver != NULL )
    {
      mObserver->IndicatorClosed( this );
    }
    // Don't do connection in this callback - strange things happen!
    StartReconnectionTimer();
  }
}

void Indicator::CreateNewImage( int bufferNumber )
{
  DALI_LOG_TRACE_METHOD_FMT( gIndicatorLogFilter, "W:%d H:%d", mSharedFileInfo[bufferNumber].mImageWidth, mSharedFileInfo[bufferNumber].mImageHeight );
  mIndicatorBuffer = new IndicatorBuffer( mAdaptor, mSharedFileInfo[bufferNumber].mImageWidth, mSharedFileInfo[bufferNumber].mImageHeight, Pixel::BGRA8888 );
  Dali::Image image = Dali::NativeImage::New( mIndicatorBuffer->GetNativeImage() );

  if( CopyToBuffer( bufferNumber ) ) // Only create images if we have valid image buffer
  {
    mIndicatorImageActor.SetImage( image );
  }
  else
  {
    DALI_LOG_WARNING("### Cannot create indicator image - disconnecting ###\n");
    Disconnect();
    if( mObserver != NULL )
    {
      mObserver->IndicatorClosed( this );
    }
    // Don't do connection in this callback - strange things happen!
    StartReconnectionTimer();
  }
}

//@todo replace with a gradient renderer when that is implemented
Dali::Geometry Indicator::CreateBackgroundGeometry()
{
  switch( mOpacityMode )
  {
    case Dali::Window::TRANSLUCENT:
      if( !mTranslucentGeometry )
      {
        // Construct 5 interval mesh
        // 0  +---+  1
        //    | \ |
        // 2  +---+  3
        //    | \ |
        // 4  +---+  5
        //    | \ |
        // 6  +---+  7
        //    | \ |
        // 8  +---+  9
        //    | \ |
        // 10 +---+  11

        // Create vertices
        struct BackgroundVertex
        {
          Vector2 mPosition;
          float   mAlpha;
        };

        unsigned int numVertices = 2 * ( NUM_GRADIENT_INTERVALS + 1 );
        BackgroundVertex vertices[ numVertices ];

        float d = -0.5f;
        float delta = 1.0f / NUM_GRADIENT_INTERVALS;
        BackgroundVertex* currentVertex = vertices;
        for( int y = 0; y < NUM_GRADIENT_INTERVALS + 1; ++y, d += delta )
        {
          currentVertex->mPosition = Vector2( -0.5f, d );
          currentVertex->mAlpha = GRADIENT_ALPHA[ y ];
          currentVertex++;

          currentVertex->mPosition = Vector2( 0.5f, d );
          currentVertex->mAlpha = GRADIENT_ALPHA[ y ];
          currentVertex++;
        }

        // Create indices
        unsigned int numIndices = 2 * 3 * NUM_GRADIENT_INTERVALS;
        unsigned int indices[ numIndices ];

        unsigned int* currentIndex = indices;
        for( int y = 0; y < NUM_GRADIENT_INTERVALS; ++y )
        {
          *currentIndex++ = (2 * y);
          *currentIndex++ = (2 * y) + 3;
          *currentIndex++ = (2 * y) + 1;

          *currentIndex++ = (2 * y);
          *currentIndex++ = (2 * y) + 2;
          *currentIndex++ = (2 * y) + 3;
        }

        Dali::Property::Map vertexFormat;
        vertexFormat[ "aPosition" ] = Dali::Property::VECTOR2;
        vertexFormat[ "aAlpha" ] = Dali::Property::FLOAT;
        Dali::PropertyBuffer vertexPropertyBuffer = Dali::PropertyBuffer::New( vertexFormat, numVertices );
        vertexPropertyBuffer.SetData( vertices );

        Dali::Property::Map indexFormat;
        indexFormat[ "indices" ] = Dali::Property::INTEGER;
        Dali::PropertyBuffer indexPropertyBuffer = Dali::PropertyBuffer::New( indexFormat, numIndices );
        indexPropertyBuffer.SetData( indices );

        // Create the geometry object
        mTranslucentGeometry = Dali::Geometry::New();
        mTranslucentGeometry.AddVertexBuffer( vertexPropertyBuffer );
        mTranslucentGeometry.SetIndexBuffer( indexPropertyBuffer );
      }

      return mTranslucentGeometry;
    case Dali::Window::OPAQUE:

      if( !mSolidGeometry )
      {
        // Create vertices
        struct BackgroundVertex
        {
          Vector2 mPosition;
          float   mAlpha;
        };

        BackgroundVertex vertices[ 4 ] = { { Vector2( -0.5f, -0.5f ), 1.0f }, { Vector2( 0.5f, -0.5f ), 1.0f },
                                           { Vector2( -0.5f,  0.5f ), 1.0f }, { Vector2( 0.5f,  0.5f ), 1.0f } };

        // Create indices
        unsigned int indices[ 6 ] = { 0, 3, 1, 0, 2, 3 };

        Dali::Property::Map vertexFormat;
        vertexFormat[ "aPosition" ] = Dali::Property::VECTOR2;
        vertexFormat[ "aAlpha" ] = Dali::Property::FLOAT;
        Dali::PropertyBuffer vertexPropertyBuffer = Dali::PropertyBuffer::New( vertexFormat, 4 );
        vertexPropertyBuffer.SetData( vertices );

        Dali::Property::Map indexFormat;
        indexFormat[ "indices" ] = Dali::Property::INTEGER;
        Dali::PropertyBuffer indexPropertyBuffer = Dali::PropertyBuffer::New( indexFormat, 6 );
        indexPropertyBuffer.SetData( indices );

        // Create the geometry object
        mSolidGeometry = Dali::Geometry::New();
        mSolidGeometry.AddVertexBuffer( vertexPropertyBuffer );
        mSolidGeometry.SetIndexBuffer( indexPropertyBuffer );
      }

      return mSolidGeometry;
    case Dali::Window::TRANSPARENT:
      break;
  }

  return Dali::Geometry();
}

void Indicator::OnIndicatorTypeChanged( Type indicatorType )
{
  if( mObserver != NULL )
  {
    mObserver->IndicatorTypeChanged( indicatorType );
  }
}

void Indicator::DataReceived( void* event )
{
  DALI_LOG_TRACE_METHOD( gIndicatorLogFilter );
  Ecore_Ipc_Event_Server_Data *epcEvent = static_cast<Ecore_Ipc_Event_Server_Data *>( event );

  switch( epcEvent->minor )
  {
    case OP_UPDATE:
    {
      DALI_LOG_INFO( gIndicatorLogFilter, Debug::General, "Indicator client received: OP_UPDATE\n" );
      if( mIsShowing )
      {
        mAdaptor->RequestUpdateOnce();
      }
      break;
    }
    case OP_UPDATE_DONE:
    {
      DALI_LOG_INFO( gIndicatorLogFilter, Debug::General, "Indicator client received: OP_UPDATE_DONE [%d]\n", epcEvent->response );
      // epcEvent->response == display buffer #
      UpdateImageData( epcEvent->response );
      break;
    }
    case OP_SHM_REF0:
    {
      DALI_LOG_INFO( gIndicatorLogFilter, Debug::General, "Indicator client received: OP_SHM_REF0\n" );
      SetSharedImageInfo( epcEvent );
      break;
    }
    case OP_SHM_REF1:
    {
      DALI_LOG_INFO( gIndicatorLogFilter, Debug::General, "Indicator client received: OP_SHM_REF1\n" );
      SetLockFileInfo( epcEvent );
      break;
    }
    case OP_SHM_REF2:
    {
      DALI_LOG_INFO( gIndicatorLogFilter, Debug::General, "Indicator client received: OP_SHM_REF2\n" );
      LoadSharedImage( epcEvent );
      break;
    }
    case OP_RESIZE:
    {
      DALI_LOG_INFO( gIndicatorLogFilter, Debug::General, "Indicator client received: OP_RESIZE\n" );

      if( (epcEvent->data) && (epcEvent->size >= (int)sizeof(IpcDataResize)) )
      {
        IpcDataResize *newSize = static_cast<IpcDataResize*>( epcEvent->data );
        Resize( newSize->w, newSize->h );
      }
      break;
    }
    case OP_MSG_PARENT:
    {
      int msgDomain = epcEvent->ref;
      int msgId = epcEvent->ref_to;

      void *msgData = NULL;
      int msgDataSize = 0;
      msgData = epcEvent->data;
      msgDataSize = epcEvent->size;

      DALI_LOG_INFO( gIndicatorLogFilter, Debug::General, "Indicator client received: OP_MSG_PARENT. msgDomain = %d\n", msgDomain );

      if( msgDomain == MSG_DOMAIN_CONTROL_INDICATOR )
      {
        switch( msgId )
        {
          case MSG_ID_INDICATOR_TYPE:
          {
            DALI_LOG_INFO( gIndicatorLogFilter, Debug::General, "Indicator client received: OP_MSG_PARENT, INDICATOR_TYPE\n" );
            Type* indicatorType = static_cast<Type*>( epcEvent->data );
            OnIndicatorTypeChanged( *indicatorType );
            break;
          }

          case MSG_ID_INDICATOR_START_ANIMATION:
          {
            DALI_LOG_INFO( gIndicatorLogFilter, Debug::General, "Indicator client received: MSG_ID_INDICATOR_START_ANIMATION\n" );

            if (msgDataSize != (int)sizeof(IpcIndicatorDataAnimation))
            {
              DALI_LOG_ERROR("Message data is incorrect");
              break;
            }

            IpcIndicatorDataAnimation *animData = static_cast<IpcIndicatorDataAnimation*>(msgData);

            if(!CheckVisibleState())
            {
              ShowIndicator( animData->duration /* n sec */ );
            }
            break;
          }

        }
      }
      break;
    }
  }
}

void Indicator::ConnectionClosed()
{
  DALI_LOG_TRACE_METHOD( gIndicatorLogFilter );

  // Will get this callback if the server connection failed to start up.
  delete mServerConnection;
  mServerConnection = NULL;
  mState = DISCONNECTED;

  // Attempt to re-connect
  Connect();
}

bool Indicator::CheckVisibleState()
{
  if( mOrientation == Dali::Window::LANDSCAPE
    || mOrientation == Dali::Window::LANDSCAPE_INVERSE
    || (mVisible != Dali::Window::VISIBLE) )
  {
    return false;
  }

  return true;
}

void Indicator::ClearSharedFileInfo()
{
  for( int i = 0; i < SHARED_FILE_NUMBER; i++ )
  {
    delete mSharedFileInfo[i].mLock;
    mSharedFileInfo[i].mLock = NULL;

    delete mSharedFileInfo[i].mSharedFile;
    mSharedFileInfo[i].mSharedFile = NULL;

    mSharedFileInfo[i].mLockFileName.clear();
    mSharedFileInfo[i].mSharedFileName.clear();
  }
}

/**
 * duration can be this
 *
 * enum
 * {
 *  KEEP_SHOWING = -1,
 *  HIDE_NOW = 0
 * };
 */
void Indicator::ShowIndicator(float duration)
{
  if( !mIndicatorAnimation )
  {
    mIndicatorAnimation = Dali::Animation::New(SLIDING_ANIMATION_DURATION);
    mIndicatorAnimation.FinishedSignal().Connect(this, &Indicator::OnAnimationFinished);
  }

  if(mIsShowing && !EqualsZero(duration))
  {
    // If need to show during showing, do nothing.
    // In 2nd phase (below) will update timer
  }
  else if(!mIsShowing && mIsAnimationPlaying && EqualsZero(duration))
  {
    // If need to hide during hiding or hidden already, do nothing
  }
  else
  {
    if( EqualsZero(duration) )
    {
      mIndicatorAnimation.AnimateTo( Property( mIndicatorImageContainerActor, Dali::Actor::Property::POSITION ), Vector3(0, -mImageHeight, 0), Dali::AlphaFunction::EASE_OUT );

      mIsShowing = false;

      OnIndicatorTypeChanged( INDICATOR_TYPE_2 ); // un-toucable
    }
    else
    {
      mIndicatorAnimation.AnimateTo( Property( mIndicatorImageContainerActor, Dali::Actor::Property::POSITION ), Vector3(0, 0, 0), Dali::AlphaFunction::EASE_OUT );

      mIsShowing = true;

      OnIndicatorTypeChanged( INDICATOR_TYPE_1 ); // touchable
    }

    mIndicatorAnimation.Play();
    mIsAnimationPlaying = true;
  }

  if(duration > 0)
  {
    if(!mShowTimer)
    {
      mShowTimer = Dali::Timer::New(1000 * duration);
      mShowTimer.TickSignal().Connect(this, &Indicator::OnShowTimer);
    }
    mShowTimer.SetInterval(1000* duration);
    mShowTimer.Start();

    if( mVisible == Dali::Window::AUTO )
    {
      // check the stage touch
      Dali::Stage::GetCurrent().TouchedSignal().Connect( this, &Indicator::OnStageTouched );
    }
  }
  else
  {
    if(mShowTimer && mShowTimer.IsRunning())
    {
      mShowTimer.Stop();
    }

    if( mVisible == Dali::Window::AUTO )
    {
      // check the stage touch
      Dali::Stage::GetCurrent().TouchedSignal().Disconnect( this, &Indicator::OnStageTouched );
    }
  }
}

bool Indicator::OnShowTimer()
{
  // after time up, hide indicator
  ShowIndicator( HIDE_NOW );

  return false;
}

void Indicator::OnAnimationFinished(Dali::Animation& animation)
{
  mIsAnimationPlaying = false;
  // once animation is finished and indicator is hidden, take it off stage
  if( !mIsShowing )
  {
    if( mObserver != NULL )
    {
      mObserver->IndicatorVisibilityChanged( mIsShowing ); // is showing?
    }
  }
}

void Indicator::OnPan( Dali::Actor actor, const Dali::PanGesture& gesture )
{
  if( mServerConnection )
  {
    switch( gesture.state )
    {
      case Gesture::Started:
      {
        mGestureDetected = false;

        // The gesture position is the current position after it has moved by the displacement.
        // We want to reference the original position.
        mGestureDeltaY = gesture.position.y - gesture.displacement.y;
      }

      // No break, Fall through
      case Gesture::Continuing:
      {
        if( mVisible == Dali::Window::AUTO && !mIsShowing )
        {
          // Only take one touch point
          if( gesture.numberOfTouches == 1 && mGestureDetected == false )
          {
            mGestureDeltaY += gesture.displacement.y;

            if( mGestureDeltaY >= mImageHeight * SHOWING_DISTANCE_HEIGHT_RATE )
            {
              ShowIndicator( AUTO_INDICATOR_STAY_DURATION );
              mGestureDetected = true;
            }
          }
        }

        break;
      }

      case Gesture::Finished:
      case Gesture::Cancelled:
      {
        // if indicator is showing, hide again when touching is finished (Since touch leave is activated, checking it in gesture::finish instead of touch::up)
        if( mVisible == Dali::Window::AUTO && mIsShowing )
        {
          ShowIndicator( AUTO_INDICATOR_STAY_DURATION );
        }
        break;
      }


      default:
        break;
    }
  }
}

void Indicator::OnStageTouched(const Dali::TouchEvent& touchEvent)
{
  const TouchPoint& touchPoint = touchEvent.GetPoint( 0 );

  // when stage is touched while indicator is showing temporary, hide it
  if( mIsShowing && ( CheckVisibleState() == false || mVisible == Dali::Window::AUTO ) )
  {
    switch( touchPoint.state )
    {
      case Dali::TouchPoint::Down:
      {
        ShowIndicator( HIDE_NOW );
        break;
      }

      default:
      break;
    }
  }
}

} // Adaptor
} // Internal
} // Dali
