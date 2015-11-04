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
#include <Ecore.h>
#include <Elementary.h>
#include <X11/Xlib.h>

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
  APP_LANGUAGE_CHANGE,
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
    mCallbackManager( CallbackManager::New() )
  {
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
  // Static methods

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

  /**
   * Called by AppCore when the language changes on the device.
   */
  static void AppLanguageChange(void* data)
  {
    static_cast<Framework*>(data)->AppStatusHandler(APP_LANGUAGE_CHANGE, NULL);
  }

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

  elm_init(*mArgc, *mArgv);

  Impl::AppCreate(this);

  elm_run();

  mRunning = false;
}

void Framework::Quit()
{
  Impl::AppTerminate(this);

  elm_exit();
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

      mObserver.OnInit();
      break;
    }

    case APP_RESET:
      mObserver.OnReset();
      break;

    case APP_RESUME:
      mObserver.OnResume();
      break;

    case APP_TERMINATE:
     mObserver.OnTerminate();
      break;

    case APP_PAUSE:
      mObserver.OnPause();
      break;

    case APP_LANGUAGE_CHANGE:
      mObserver.OnLanguageChanged();
      break;

    default:
      break;
  }

  return true;
}

void Framework::InitThreads()
{
  XInitThreads();
}

} // namespace Adaptor

} // namespace Internal

} // namespace Dali
