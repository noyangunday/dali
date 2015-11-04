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
#include "framework.h"

// EXTERNAL INCLUDES
#include <app.h>
#include <bundle.h>
#include <Ecore.h>

#ifdef OVER_TIZEN_SDK_2_2
#include <system_info.h>
#include <app_control_internal.h>
#endif

#include <dali/integration-api/debug.h>

// INTERNAL INCLUDES
#include <callback-manager.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

namespace
{

/// Application Status Enum
enum
{
  APP_CREATE,
  APP_TERMINATE,
  APP_PAUSE,
  APP_RESUME,
  APP_RESET,
  APP_CONTROL,
  APP_LANGUAGE_CHANGE,
  APP_DEVICE_ROTATED,
  APP_REGION_CHANGED,
  APP_BATTERY_LOW,
  APP_MEMORY_LOW
};

} // Unnamed namespace

/**
 * Impl to hide EFL data members
 */
struct Framework::Impl
{
  // Constructor

  Impl(void* data)
  : mAbortCallBack( NULL ),
    mCallbackManager( NULL )
  {
    mEventCallback.create = AppCreate;
    mEventCallback.terminate = AppTerminate;
    mEventCallback.pause = AppPause;
    mEventCallback.resume = AppResume;
#ifndef OVER_TIZEN_SDK_2_2
    mEventCallback.service = AppService;

    mEventCallback.low_memory = NULL;
    mEventCallback.low_battery = NULL;
    mEventCallback.device_orientation = AppDeviceRotated;
    mEventCallback.language_changed = AppLanguageChanged;
    mEventCallback.region_format_changed = NULL;

#else
    mEventCallback.app_control = AppControl;

    ui_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY], APP_EVENT_LOW_BATTERY, AppBatteryLow, data);
    ui_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY], APP_EVENT_LOW_MEMORY, AppMemoryLow, data);
    ui_app_add_event_handler(&handlers[APP_EVENT_DEVICE_ORIENTATION_CHANGED], APP_EVENT_DEVICE_ORIENTATION_CHANGED, AppDeviceRotated, data);
    ui_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, AppLanguageChanged, data);
    ui_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED, AppRegionChanged, data);

#endif

    mCallbackManager = CallbackManager::New();
  }

  ~Impl()
  {
    delete mAbortCallBack;

    // we're quiting the main loop so
    // mCallbackManager->RemoveAllCallBacks() does not need to be called
    // to delete our abort handler
    delete mCallbackManager;
  }

  // Data

  CallbackBase* mAbortCallBack;
  CallbackManager *mCallbackManager;

#ifndef OVER_TIZEN_SDK_2_2
  app_event_callback_s mEventCallback;
#else
  ui_app_lifecycle_callback_s mEventCallback;
  app_event_handler_h handlers[5];
#endif

  /**
   * Called by AppCore on application creation.
   */
  static bool AppCreate(void *data)
  {
    return static_cast<Framework*>(data)->AppStatusHandler(APP_CREATE, NULL);
  }

  /**
   * Called by AppCore when the application should terminate.
   */
  static void AppTerminate(void *data)
  {
    static_cast<Framework*>(data)->AppStatusHandler(APP_TERMINATE, NULL);
  }

  /**
   * Called by AppCore when the application is paused.
   */
  static void AppPause(void *data)
  {
    static_cast<Framework*>(data)->AppStatusHandler(APP_PAUSE, NULL);
  }

  /**
   * Called by AppCore when the application is resumed.
   */
  static void AppResume(void *data)
  {
    static_cast<Framework*>(data)->AppStatusHandler(APP_RESUME, NULL);
  }

  static void ProcessBundle(Framework* framework, bundle *bundleData)
  {
    if(bundleData == NULL)
    {
      return;
    }

    // get bundle name
    char* bundleName = const_cast<char*>(bundle_get_val(bundleData, "name"));
    if(bundleName != NULL)
    {
      framework->SetBundleName(bundleName);
    }

    // get bundle id
    char* bundleId = const_cast<char*>(bundle_get_val(bundleData, "id"));
    if(bundleId != NULL)
    {
      framework->SetBundleId(bundleId);
    }
  }

#ifndef OVER_TIZEN_SDK_2_2
  /**
   * Called by AppCore when the application is launched from another module (e.g. homescreen).
   * @param[in] b the bundle data which the launcher module sent
   */
  static void AppService(service_h service, void *data)
  {
    Framework* framework = static_cast<Framework*>(data);

    if(framework == NULL)
    {
      return;
    }
    bundle *bundleData = NULL;

    service_to_bundle(service, &bundleData);
    ProcessBundle(framework, bundleData);

    framework->AppStatusHandler(APP_RESET, NULL);
  }

  static void AppLanguageChanged(void* user_data)
  {
    static_cast<Framework*>(user_data)->AppStatusHandler(APP_LANGUAGE_CHANGE, NULL);
  }

  static void AppDeviceRotated(app_device_orientation_e orientation, void *user_data)
  {
    static_cast<Framework*>(user_data)->AppStatusHandler(APP_DEVICE_ROTATED, NULL);
  }

#else

  /**
   * Called by AppCore when the application is launched from another module (e.g. homescreen).
   * @param[in] b the bundle data which the launcher module sent
   */
  static void AppControl(app_control_h app_control, void *data)
  {
    Framework* framework = static_cast<Framework*>(data);
    if(framework == NULL)
    {
      return;
    }
    bundle *bundleData = NULL;

    app_control_to_bundle(app_control, &bundleData);
    ProcessBundle(framework, bundleData);

    framework->AppStatusHandler(APP_RESET, NULL);
    framework->AppStatusHandler(APP_CONTROL, app_control);
  }

  static void AppLanguageChanged(app_event_info_h event_info, void *user_data)
  {
    static_cast<Framework*>(user_data)->AppStatusHandler(APP_LANGUAGE_CHANGE, NULL);
  }

  static void AppDeviceRotated(app_event_info_h event_info, void *user_data)
  {
    static_cast<Framework*>(user_data)->AppStatusHandler(APP_DEVICE_ROTATED, NULL);
  }

  static void AppRegionChanged(app_event_info_h event_info, void *user_data)
  {
    static_cast<Framework*>(user_data)->AppStatusHandler(APP_REGION_CHANGED, NULL);
  }

  static void AppBatteryLow(app_event_info_h event_info, void *user_data)
  {
    static_cast<Framework*>(user_data)->AppStatusHandler(APP_BATTERY_LOW, NULL);
  }

  static void AppMemoryLow(app_event_info_h event_info, void *user_data)
  {
    static_cast<Framework*>(user_data)->AppStatusHandler(APP_MEMORY_LOW, NULL);
  }

#endif

private:
  // Undefined
  Impl( const Impl& impl );

  // Undefined
  Impl& operator=( const Impl& impl );
};

Framework::Framework( Framework::Observer& observer, int *argc, char ***argv )
: mObserver(observer),
  mInitialised(false),
  mRunning(false),
  mArgc(argc),
  mArgv(argv),
  mBundleName(""),
  mBundleId(""),
  mAbortHandler( MakeCallback( this, &Framework::AbortCallback ) ),
  mImpl(NULL)
{

#ifdef OVER_TIZEN_SDK_2_2
  bool featureFlag = true;
  system_info_get_platform_bool( "tizen.org/feature/opengles.version.2_0", &featureFlag );

  if( featureFlag == false )
  {
    set_last_result( TIZEN_ERROR_NOT_SUPPORTED );
    throw Dali::DaliException( "", "OpenGL ES 2.0 is not supported." );
  }
#endif

  InitThreads();
  mImpl = new Impl(this);
}

Framework::~Framework()
{
  if (mRunning)
  {
    Quit();
  }

  delete mImpl;
}

void Framework::Run()
{
  mRunning = true;

#ifndef OVER_TIZEN_SDK_2_2
  app_efl_main(mArgc, mArgv, &mImpl->mEventCallback, this);

#else
  int ret = ui_app_main(*mArgc, *mArgv, &mImpl->mEventCallback, this);
  if (ret != APP_ERROR_NONE)
  {
    DALI_LOG_ERROR("Framework::Run(), ui_app_main() is failed. err = %d", ret);
  }
#endif

  mRunning = false;
}

void Framework::Quit()
{
  app_efl_exit();
}

bool Framework::IsMainLoopRunning()
{
  return mRunning;
}

void Framework::AddAbortCallback( CallbackBase* callback )
{
  mImpl->mAbortCallBack = callback;
}

std::string Framework::GetBundleName() const
{
  return mBundleName;
}

void Framework::SetBundleName(const std::string& name)
{
  mBundleName = name;
}

std::string Framework::GetBundleId() const
{
  return mBundleId;
}

void Framework::SetBundleId(const std::string& id)
{
  mBundleId = id;
}

void Framework::AbortCallback( )
{
  // if an abort call back has been installed run it.
  if (mImpl->mAbortCallBack)
  {
    CallbackBase::Execute( *mImpl->mAbortCallBack );
  }
  else
  {
    Quit();
  }
}

bool Framework::AppStatusHandler(int type, void *bundleData)
{
  switch (type)
  {
    case APP_CREATE:
    {
      mInitialised = true;

      // Connect to abnormal exit signals
      mAbortHandler.AbortOnSignal( SIGINT );
      mAbortHandler.AbortOnSignal( SIGQUIT );
      mAbortHandler.AbortOnSignal( SIGKILL );
      mAbortHandler.AbortOnSignal( SIGTERM );
      mAbortHandler.AbortOnSignal( SIGHUP );

      mObserver.OnInit();
      break;
    }

    case APP_RESET:
    {
      mObserver.OnReset();
      break;
    }

    case APP_RESUME:
    {
      mObserver.OnResume();
      break;
    }

    case APP_TERMINATE:
    {
      mObserver.OnTerminate();
      break;
    }

    case APP_PAUSE:
    {
      mObserver.OnPause();
      break;
    }

    case APP_CONTROL:
    {
      mObserver.OnAppControl(bundleData);
      break;
    }

    case APP_LANGUAGE_CHANGE:
    {
      mObserver.OnLanguageChanged();
      break;
    }

    case APP_REGION_CHANGED:
    {
      mObserver.OnRegionChanged();
      break;
    }

    case APP_BATTERY_LOW:
    {
      mObserver.OnBatteryLow();
      break;
    }

    case APP_MEMORY_LOW:
    {
      mObserver.OnMemoryLow();
      break;
    }

    default:
      break;
  }

  return true;
}

} // namespace Adaptor

} // namespace Internal

} // namespace Dali
