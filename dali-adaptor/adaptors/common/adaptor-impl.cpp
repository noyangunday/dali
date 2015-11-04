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

// CLASS HEADER
#include "adaptor-impl.h"

// EXTERNAL INCLUDES
#include <dali/public-api/common/dali-common.h>
#include <dali/integration-api/debug.h>
#include <dali/integration-api/core.h>
#include <dali/integration-api/context-notifier.h>
#include <dali/integration-api/profiling.h>
#include <dali/integration-api/input-options.h>
#include <dali/integration-api/events/touch-event-integ.h>

// INTERNAL INCLUDES
#include <base/thread-controller.h>
#if defined(NETWORK_LOGGING_ENABLED)
#  include <base/performance-logging/performance-interface-factory.h>
#endif
#include <base/lifecycle-observer.h>

#include <dali/devel-api/text-abstraction/font-client.h>

#include <callback-manager.h>
#include <render-surface.h>
#include <tts-player-impl.h>
#include <accessibility-adaptor-impl.h>
#include <events/gesture-manager.h>
#include <events/event-handler.h>
#include <gl/gl-proxy-implementation.h>
#include <gl/gl-implementation.h>
#include <gl/egl-sync-implementation.h>
#include <gl/egl-image-extensions.h>
#include <gl/egl-factory.h>
#include <imf-manager-impl.h>
#include <clipboard-impl.h>
#include <vsync-monitor.h>
#include <object-profiler.h>
#include <base/display-connection.h>
#include <window-impl.h>

#include <tizen-logging.h>

using Dali::TextAbstraction::FontClient;

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

namespace
{
__thread Adaptor* gThreadLocalAdaptor = NULL; // raw thread specific pointer to allow Adaptor::Get
} // unnamed namespace

Dali::Adaptor* Adaptor::New( Any nativeWindow, RenderSurface *surface, Dali::Configuration::ContextLoss configuration, EnvironmentOptions* environmentOptions )
{
  Dali::Adaptor* adaptor = new Dali::Adaptor;
  Adaptor* impl = new Adaptor( nativeWindow, *adaptor, surface, environmentOptions );
  adaptor->mImpl = impl;

  impl->Initialize(configuration);

  return adaptor;
}

Dali::Adaptor* Adaptor::New( Dali::Window window, Dali::Configuration::ContextLoss configuration, EnvironmentOptions* environmentOptions )
{
  Any winId = window.GetNativeHandle();

  Window& windowImpl = Dali::GetImplementation(window);
  Dali::Adaptor* adaptor = New( winId, windowImpl.GetSurface(), configuration, environmentOptions );
  windowImpl.SetAdaptor(*adaptor);
  return adaptor;
}

void Adaptor::Initialize( Dali::Configuration::ContextLoss configuration )
{
  // all threads here (event, update, and render) will send their logs to TIZEN Platform's LogMessage handler.
  Dali::Integration::Log::LogFunction logFunction( Dali::TizenPlatform::LogMessage );
  mEnvironmentOptions->SetLogFunction( logFunction );
  mEnvironmentOptions->InstallLogFunction(); // install logging for main thread

  mPlatformAbstraction = new TizenPlatform::TizenPlatformAbstraction;

  std::string path;
  GetDataStoragePath( path );
  mPlatformAbstraction->SetDataStoragePath( path );

  ResourcePolicy::DataRetention dataRetentionPolicy = ResourcePolicy::DALI_DISCARDS_ALL_DATA;
  if( configuration == Dali::Configuration::APPLICATION_DOES_NOT_HANDLE_CONTEXT_LOSS )
  {
    dataRetentionPolicy = ResourcePolicy::DALI_DISCARDS_ALL_DATA;
  }
  // Note, Tizen does not use DALI_RETAINS_ALL_DATA, as it can reload images from
  // files automatically.

#if defined(NETWORK_LOGGING_ENABLED)
  if( mEnvironmentOptions->PerformanceServerRequired() )
  {
    mPerformanceInterface = PerformanceInterfaceFactory::CreateInterface( *this, *mEnvironmentOptions );
  }
#endif

  mCallbackManager = CallbackManager::New();

  PositionSize size = mSurface->GetPositionSize();

  mGestureManager = new GestureManager(*this, Vector2(size.width, size.height), mCallbackManager, *mEnvironmentOptions);

  if( mEnvironmentOptions->GetGlesCallTime() > 0 )
  {
    mGLES = new GlProxyImplementation( *mEnvironmentOptions );
  }
  else
  {
    mGLES = new GlImplementation();
  }

  mEglFactory = new EglFactory();

  EglSyncImplementation* eglSyncImpl = mEglFactory->GetSyncImplementation();

  mCore = Integration::Core::New( *this, *mPlatformAbstraction, *mGLES, *eglSyncImpl, *mGestureManager, dataRetentionPolicy );

  const unsigned int timeInterval = mEnvironmentOptions->GetObjectProfilerInterval();
  if( 0u < timeInterval )
  {
    mObjectProfiler = new ObjectProfiler( timeInterval );
  }

  mNotificationTrigger = mTriggerEventFactory.CreateTriggerEvent( MakeCallback( this, &Adaptor::ProcessCoreEvents ), TriggerEventInterface::KEEP_ALIVE_AFTER_TRIGGER);

  mVSyncMonitor = new VSyncMonitor;

  mThreadController = new ThreadController( *this, *mEnvironmentOptions );

  // Should be called after Core creation
  if( mEnvironmentOptions->GetPanGestureLoggingLevel() )
  {
    Integration::EnableProfiling( Dali::Integration::PROFILING_TYPE_PAN_GESTURE );
  }
  if( mEnvironmentOptions->GetPanGesturePredictionMode() >= 0 )
  {
    Integration::SetPanGesturePredictionMode(mEnvironmentOptions->GetPanGesturePredictionMode());
  }
  if( mEnvironmentOptions->GetPanGesturePredictionAmount() >= 0 )
  {
    Integration::SetPanGesturePredictionAmount(mEnvironmentOptions->GetPanGesturePredictionAmount());
  }
  if( mEnvironmentOptions->GetPanGestureMaximumPredictionAmount() >= 0 )
  {
    Integration::SetPanGestureMaximumPredictionAmount(mEnvironmentOptions->GetPanGestureMaximumPredictionAmount());
  }
  if( mEnvironmentOptions->GetPanGestureMinimumPredictionAmount() >= 0 )
  {
    Integration::SetPanGestureMinimumPredictionAmount(mEnvironmentOptions->GetPanGestureMinimumPredictionAmount());
  }
  if( mEnvironmentOptions->GetPanGesturePredictionAmountAdjustment() >= 0 )
  {
    Integration::SetPanGesturePredictionAmountAdjustment(mEnvironmentOptions->GetPanGesturePredictionAmountAdjustment());
  }
  if( mEnvironmentOptions->GetPanGestureSmoothingMode() >= 0 )
  {
    Integration::SetPanGestureSmoothingMode(mEnvironmentOptions->GetPanGestureSmoothingMode());
  }
  if( mEnvironmentOptions->GetPanGestureSmoothingAmount() >= 0.0f )
  {
    Integration::SetPanGestureSmoothingAmount(mEnvironmentOptions->GetPanGestureSmoothingAmount());
  }
}

Adaptor::~Adaptor()
{
  // Ensure stop status
  Stop();

  // set to NULL first as we do not want any access to Adaptor as it is being destroyed.
  gThreadLocalAdaptor = NULL;

  for ( ObserverContainer::iterator iter = mObservers.begin(), endIter = mObservers.end(); iter != endIter; ++iter )
  {
    (*iter)->OnDestroy();
  }

  delete mThreadController; // this will shutdown render thread, which will call Core::ContextDestroyed before exit
  delete mVSyncMonitor;
  delete mEventHandler;
  delete mObjectProfiler;

  delete mCore;
  delete mEglFactory;
  delete mGLES;
  delete mGestureManager;
  delete mPlatformAbstraction;
  delete mCallbackManager;
  delete mPerformanceInterface;

  // uninstall it on this thread (main actor thread)
  Dali::Integration::Log::UninstallLogFunction();

  // Delete environment options if we own it
  if( mEnvironmentOptionsOwned )
  {
    delete mEnvironmentOptions;
  }
}

void Adaptor::Start()
{
  // it doesn't support restart after stop at this moment
  // to support restarting, need more testing
  if( READY != mState )
  {
    return;
  }

  // Start the callback manager
  mCallbackManager->Start();

  // create event handler
  mEventHandler = new EventHandler( mSurface, *this, *mGestureManager, *this, mDragAndDropDetector );

  if( mDeferredRotationObserver != NULL )
  {
    mEventHandler->SetRotationObserver(mDeferredRotationObserver);
    mDeferredRotationObserver = NULL;
  }

  unsigned int dpiHor, dpiVer;
  dpiHor = dpiVer = 0;
  Dali::DisplayConnection::GetDpi(dpiHor, dpiVer);

  // tell core about the DPI value
  mCore->SetDpi(dpiHor, dpiVer);

  // set the DPI value for font rendering
  FontClient fontClient = FontClient::Get();
  fontClient.SetDpi( dpiHor, dpiVer );

  // Tell the core the size of the surface just before we start the render-thread
  PositionSize size = mSurface->GetPositionSize();
  mCore->SurfaceResized( size.width, size.height );

  // Initialize the thread controller
  mThreadController->Initialize();

  mState = RUNNING;

  ProcessCoreEvents(); // Ensure any startup messages are processed.

  for ( ObserverContainer::iterator iter = mObservers.begin(), endIter = mObservers.end(); iter != endIter; ++iter )
  {
    (*iter)->OnStart();
  }
}

// Dali::Internal::Adaptor::Adaptor::Pause
void Adaptor::Pause()
{
  // Only pause the adaptor if we're actually running.
  if( RUNNING == mState )
  {
    // Inform observers that we are about to be paused.
    for( ObserverContainer::iterator iter = mObservers.begin(), endIter = mObservers.end(); iter != endIter; ++iter )
    {
      (*iter)->OnPause();
    }

    // Reset the event handler when adaptor paused
    if( mEventHandler )
    {
      mEventHandler->Pause();
    }

    mThreadController->Pause();
    mCore->Suspend();
    mState = PAUSED;
  }
}

// Dali::Internal::Adaptor::Adaptor::Resume
void Adaptor::Resume()
{
  // Only resume the adaptor if we are in the suspended state.
  if( PAUSED == mState )
  {
    mState = RUNNING;

    // Reset the event handler when adaptor resumed
    if( mEventHandler )
    {
      mEventHandler->Resume();
    }

    // Inform observers that we have resumed.
    for( ObserverContainer::iterator iter = mObservers.begin(), endIter = mObservers.end(); iter != endIter; ++iter )
    {
      (*iter)->OnResume();
    }

    // Resume core so it processes any requests as well
    mCore->Resume();

    // Do at end to ensure our first update/render after resumption includes the processed messages as well
    mThreadController->Resume();
  }
}

void Adaptor::Stop()
{
  if( RUNNING == mState ||
      PAUSED  == mState ||
      PAUSED_WHILE_HIDDEN == mState )
  {
    for( ObserverContainer::iterator iter = mObservers.begin(), endIter = mObservers.end(); iter != endIter; ++iter )
    {
      (*iter)->OnStop();
    }

    mThreadController->Stop();
    mCore->Suspend();

    // Delete the TTS player
    for(int i =0; i < Dali::TtsPlayer::MODE_NUM; i++)
    {
      if(mTtsPlayers[i])
      {
        mTtsPlayers[i].Reset();
      }
    }

    delete mEventHandler;
    mEventHandler = NULL;

    delete mNotificationTrigger;
    mNotificationTrigger = NULL;

    mCallbackManager->Stop();

    mState = STOPPED;
  }
}

void Adaptor::ContextLost()
{
  mCore->GetContextNotifier()->NotifyContextLost(); // Inform stage
}

void Adaptor::ContextRegained()
{
  // Inform core, so that texture resources can be reloaded
  mCore->RecoverFromContextLoss();

  mCore->GetContextNotifier()->NotifyContextRegained(); // Inform stage
}

void Adaptor::FeedTouchPoint( TouchPoint& point, int timeStamp )
{
  mEventHandler->FeedTouchPoint( point, timeStamp );
}

void Adaptor::FeedWheelEvent( WheelEvent& wheelEvent )
{
  mEventHandler->FeedWheelEvent( wheelEvent );
}

void Adaptor::FeedKeyEvent( KeyEvent& keyEvent )
{
  mEventHandler->FeedKeyEvent( keyEvent );
}

bool Adaptor::MoveResize( const PositionSize& positionSize )
{
  PositionSize old = mSurface->GetPositionSize();

  // just resize the surface. The driver should automatically resize the egl Surface (untested)
  // EGL Spec says : EGL window surfaces need to be resized when their corresponding native window
  // is resized. Implementations typically use hooks into the OS and native window
  // system to perform this resizing on demand, transparently to the client.
  mSurface->MoveResize( positionSize );

  if(old.width != positionSize.width || old.height != positionSize.height)
  {
    SurfaceSizeChanged(positionSize);
  }

  return true;
}

void Adaptor::SurfaceResized( const PositionSize& positionSize )
{
  PositionSize old = mSurface->GetPositionSize();

  // Called by an application, when it has resized a window outside of Dali.
  // The EGL driver automatically detects X Window resize calls, and resizes
  // the EGL surface for us.
  mSurface->MoveResize( positionSize );

  if(old.width != positionSize.width || old.height != positionSize.height)
  {
    SurfaceSizeChanged(positionSize);
  }
}

void Adaptor::ReplaceSurface( Any nativeWindow, RenderSurface& surface )
{
  mNativeWindow = nativeWindow;
  mSurface = &surface;

  SurfaceSizeChanged(mSurface->GetPositionSize());

  // flush the event queue to give update and render threads chance
  // to start processing messages for new camera setup etc as soon as possible
  ProcessCoreEvents();

  // this method blocks until the render thread has completed the replace.
  mThreadController->ReplaceSurface(mSurface);
}

RenderSurface& Adaptor::GetSurface() const
{
  return *mSurface;
}

void Adaptor::ReleaseSurfaceLock()
{
  mSurface->ReleaseLock();
}

Dali::TtsPlayer Adaptor::GetTtsPlayer(Dali::TtsPlayer::Mode mode)
{
  if(!mTtsPlayers[mode])
  {
    // Create the TTS player when it needed, because it can reduce launching time.
    mTtsPlayers[mode] = TtsPlayer::New(mode);
  }

  return mTtsPlayers[mode];
}

bool Adaptor::AddIdle( CallbackBase* callback )
{
  bool idleAdded(false);

  // Only add an idle if the Adaptor is actually running
  if( RUNNING == mState )
  {
    idleAdded = mCallbackManager->AddIdleCallback( callback );
  }

  return idleAdded;
}


Dali::Adaptor& Adaptor::Get()
{
  DALI_ASSERT_ALWAYS( IsAvailable() && "Adaptor not instantiated" );
  return gThreadLocalAdaptor->mAdaptor;
}

bool Adaptor::IsAvailable()
{
  return gThreadLocalAdaptor != NULL;
}

void Adaptor::SceneCreated()
{
  mCore->SceneCreated();
}

Dali::Integration::Core& Adaptor::GetCore()
{
  return *mCore;
}

void Adaptor::SetRenderRefreshRate( unsigned int numberOfVSyncsPerRender )
{
  mThreadController->SetRenderRefreshRate( numberOfVSyncsPerRender );
}

void Adaptor::SetUseHardwareVSync( bool useHardware )
{
  mVSyncMonitor->SetUseHardwareVSync( useHardware );
}

EglFactory& Adaptor::GetEGLFactory() const
{
  DALI_ASSERT_DEBUG( mEglFactory && "EGL Factory not created" );
  return *mEglFactory;
}

EglFactoryInterface& Adaptor::GetEGLFactoryInterface() const
{
  return *mEglFactory;
}

Integration::GlAbstraction& Adaptor::GetGlAbstraction() const
{
  DALI_ASSERT_DEBUG( mGLES && "GLImplementation not created" );
  return *mGLES;
}

Dali::Integration::PlatformAbstraction& Adaptor::GetPlatformAbstractionInterface()
{
  return *mPlatformAbstraction;
}

Dali::Integration::GlAbstraction& Adaptor::GetGlesInterface()
{
  return *mGLES;
}

TriggerEventInterface& Adaptor::GetProcessCoreEventsTrigger()
{
  return *mNotificationTrigger;
}

TriggerEventFactoryInterface& Adaptor::GetTriggerEventFactoryInterface()
{
  return mTriggerEventFactory;
}

SocketFactoryInterface& Adaptor::GetSocketFactoryInterface()
{
  return mSocketFactory;
}

RenderSurface* Adaptor::GetRenderSurfaceInterface()
{
  return mSurface;
}

VSyncMonitorInterface* Adaptor::GetVSyncMonitorInterface()
{
  return mVSyncMonitor;
}

TraceInterface& Adaptor::GetKernelTraceInterface()
{
  return mKernelTracer;
}

TraceInterface& Adaptor::GetSystemTraceInterface()
{
  return mSystemTracer;
}

PerformanceInterface* Adaptor::GetPerformanceInterface()
{
  return mPerformanceInterface;
}

Integration::PlatformAbstraction& Adaptor::GetPlatformAbstraction() const
{
  DALI_ASSERT_DEBUG( mPlatformAbstraction && "PlatformAbstraction not created" );
  return *mPlatformAbstraction;
}

void Adaptor::SetDragAndDropDetector( DragAndDropDetectorPtr detector )
{
  mDragAndDropDetector = detector;

  if ( mEventHandler )
  {
    mEventHandler->SetDragAndDropDetector( detector );
  }
}

void Adaptor::SetRotationObserver( RotationObserver* observer )
{
  if( mEventHandler )
  {
    mEventHandler->SetRotationObserver( observer );
  }
  else if( mState == READY )
  {
    // Set once event handler exists
    mDeferredRotationObserver = observer;
  }
}

void Adaptor::DestroyTtsPlayer(Dali::TtsPlayer::Mode mode)
{
  if(mTtsPlayers[mode])
  {
    mTtsPlayers[mode].Reset();
  }
}

void Adaptor::SetMinimumPinchDistance(float distance)
{
  if( mGestureManager )
  {
    mGestureManager->SetMinimumPinchDistance(distance);
  }
}

Any Adaptor::GetNativeWindowHandle()
{
  return mNativeWindow;
}

void Adaptor::AddObserver( LifeCycleObserver& observer )
{
  ObserverContainer::iterator match ( find(mObservers.begin(), mObservers.end(), &observer) );

  if ( match == mObservers.end() )
  {
    mObservers.push_back( &observer );
  }
}

void Adaptor::RemoveObserver( LifeCycleObserver& observer )
{
  ObserverContainer::iterator match ( find(mObservers.begin(), mObservers.end(), &observer) );

  if ( match != mObservers.end() )
  {
    mObservers.erase( match );
  }
}

void Adaptor::QueueCoreEvent(const Dali::Integration::Event& event)
{
  if( mCore )
  {
    mCore->QueueEvent(event);
  }
}

void Adaptor::ProcessCoreEvents()
{
  if( mCore )
  {
    if( mPerformanceInterface )
    {
      mPerformanceInterface->AddMarker( PerformanceInterface::PROCESS_EVENTS_START );
    }

    mCore->ProcessEvents();

    if( mPerformanceInterface )
    {
      mPerformanceInterface->AddMarker( PerformanceInterface::PROCESS_EVENTS_END );
    }
  }
}

void Adaptor::RequestUpdate()
{
  // When Dali applications are partially visible behind the lock-screen,
  // the indicator must be updated (therefore allow updates in the PAUSED state)
  if ( PAUSED  == mState ||
       RUNNING == mState )
  {
    mThreadController->RequestUpdate();
  }
}

void Adaptor::RequestProcessEventsOnIdle()
{
  // Only request a notification if the Adaptor is actually running
  // and we haven't installed the idle notification
  if( ( ! mNotificationOnIdleInstalled ) && ( RUNNING == mState ) )
  {
    mNotificationOnIdleInstalled = AddIdle( MakeCallback( this, &Adaptor::ProcessCoreEventsFromIdle ) );
  }
}

void Adaptor::OnWindowShown()
{
  if ( PAUSED_WHILE_HIDDEN == mState )
  {
    // Adaptor can now be resumed
    mState = PAUSED;

    Resume();

    // Force a render task
    RequestUpdateOnce();
  }
}

void Adaptor::OnWindowHidden()
{
  if ( STOPPED != mState )
  {
    Pause();

    // Adaptor cannot be resumed until the window is shown
    mState = PAUSED_WHILE_HIDDEN;
  }
}

// Dali::Internal::Adaptor::Adaptor::OnDamaged
void Adaptor::OnDamaged( const DamageArea& area )
{
  // This is needed for the case where Dali window is partially obscured
  RequestUpdate();
}

void Adaptor::SurfaceSizeChanged(const PositionSize& positionSize)
{
  // let the core know the surface size has changed
  mCore->SurfaceResized(positionSize.width, positionSize.height);

  mResizedSignal.Emit( mAdaptor );
}

void Adaptor::NotifySceneCreated()
{
  GetCore().SceneCreated();

  // Start thread controller after the scene has been created
  mThreadController->Start();
}

void Adaptor::NotifyLanguageChanged()
{
  mLanguageChangedSignal.Emit( mAdaptor );
}

void Adaptor::RequestUpdateOnce()
{
  if( PAUSED_WHILE_HIDDEN != mState )
  {
    if( mThreadController )
    {
      mThreadController->RequestUpdateOnce();
    }
  }
}

void Adaptor::ProcessCoreEventsFromIdle()
{
  ProcessCoreEvents();

  // the idle handle automatically un-installs itself
  mNotificationOnIdleInstalled = false;
}

Adaptor::Adaptor(Any nativeWindow, Dali::Adaptor& adaptor, RenderSurface* surface, EnvironmentOptions* environmentOptions)
: mResizedSignal(),
  mLanguageChangedSignal(),
  mAdaptor(adaptor),
  mState(READY),
  mCore(NULL),
  mThreadController(NULL),
  mVSyncMonitor(NULL),
  mGLES( NULL ),
  mEglFactory( NULL ),
  mNativeWindow( nativeWindow ),
  mSurface( surface ),
  mPlatformAbstraction( NULL ),
  mEventHandler( NULL ),
  mCallbackManager( NULL ),
  mNotificationOnIdleInstalled( false ),
  mNotificationTrigger(NULL),
  mGestureManager(NULL),
  mObservers(),
  mDragAndDropDetector(),
  mDeferredRotationObserver(NULL),
  mEnvironmentOptions( environmentOptions ? environmentOptions : new EnvironmentOptions /* Create the options if not provided */),
  mPerformanceInterface(NULL),
  mObjectProfiler(NULL),
  mEnvironmentOptionsOwned( environmentOptions ? false : true /* If not provided then we own the object */ )
{
  DALI_ASSERT_ALWAYS( !IsAvailable() && "Cannot create more than one Adaptor per thread" );
  gThreadLocalAdaptor = this;
}

// Stereoscopy

void Adaptor::SetViewMode( ViewMode viewMode )
{
  mSurface->SetViewMode( viewMode );
  mCore->SetViewMode( viewMode );
}

ViewMode Adaptor::GetViewMode() const
{
  return mCore->GetViewMode();
}

void Adaptor::SetStereoBase( float stereoBase )
{
  mCore->SetStereoBase( stereoBase );
}

float Adaptor::GetStereoBase() const
{
  return mCore->GetStereoBase();
}

} // namespace Adaptor

} // namespace Internal

} // namespace Dali
