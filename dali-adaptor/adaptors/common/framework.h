#ifndef __DALI_INTERNAL_FRAMEWORK_H__
#define __DALI_INTERNAL_FRAMEWORK_H__

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
#include <string>
#include <dali/public-api/signals/callback.h>

// INTERNAL INCLUDES
#include "abort-handler.h"

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

/**
 * The Framework class is used to register callbacks with the TIZEN platform so that
 * we know when any of the application lifecycle events occur.  This includes events
 * like when our application is to be initialised, terminated, paused, resumed etc.
 */
class Framework
{
public:

  /**
   * Observer class for the framework.
   */
  class Observer
  {
  public:

    /**
     * Invoked when the application is to be initialised.
     */
    virtual void OnInit() {}

    /**
     * Invoked when the application is to be terminated.
     */
    virtual void OnTerminate() {}

    /**
     * Invoked when the application is to be paused.
     */
    virtual void OnPause() {}

    /**
     * Invoked when the application is to be resumed.
     */
    virtual void OnResume() {}

    /**
     * Invoked when the application is to be reset.
     */
    virtual void OnReset() {}

    /**
    * Invoked when the AppControl message is received.
    * @param[in] The bundle data of AppControl message.
    */
    virtual void OnAppControl(void *) {}

    /**
     * Invoked when the language of the device is changed.
     */
    virtual void OnLanguageChanged() {}

    /**
    * Invoked when the region is changed.
    */
    virtual void OnRegionChanged() {}

    /**
    * Invoked when the battery level of the device is low.
    */
    virtual void OnBatteryLow() {}

    /**
    * Invoked when the memory level of the device is low.
    */
    virtual void OnMemoryLow() {}
  };

public:

  /**
   * Constructor
   * @param[in]  observer  The observer of the Framework.
   * @param[in]  argc      A pointer to the number of arguments.
   * @param[in]  argv      A pointer the the argument list.
   */
  Framework( Observer& observer, int* argc, char ***argv );

  /**
   * Destructor
   */
  ~Framework();

public:

  /**
   * Runs the main loop of framework
   */
  void Run();

  /**
   * Quits the main loop
   */
  void Quit();

  /**
   * Checks whether the main loop of the framework is running.
   * @return true, if the main loop is running, false otherwise.
   */
  bool IsMainLoopRunning();

  /**
   * If the main loop aborts unexpectedly, then the connected callback function is called.
   * @param[in]  callBack  The function to call.
   * @note Only one callback can be registered.  The last callback to be set will be called on abort.
   * @note The ownership of callback is passed onto this class.
   */
  void AddAbortCallback( CallbackBase* callback );

  /**
   * Gets bundle name which was passed in app_reset callback.
   */
  std::string GetBundleName() const;

  /**
   * Gets bundle id which was passed in app_reset callback.
   */
  std::string GetBundleId() const;

private:

  // Undefined
  Framework(const Framework&);
  Framework& operator=(Framework&);

private:
  /**
   * Called by the App framework when an application lifecycle event occurs.
   * @param[in] type The type of event occurred.
   * @param[in] bundleData The bundle data of event occurred.
   */
  bool AppStatusHandler(int type, void *bundleData);

  /**
   * Called app_reset callback was called with bundle.
   */
  void SetBundleName(const std::string& name);

  /**
   * Called app_reset callback was called with bundle.
   */
  void SetBundleId(const std::string& id);

  /**
   * Called if the application is aborted.
   */
  void AbortCallback();

  /**
   * Called for initializing on specified backend. (X11 or Wayland)
   */
  void InitThreads();

private:
  Observer&          mObserver;
  bool               mInitialised;
  bool               mRunning;
  int*               mArgc;
  char***            mArgv;
  std::string        mBundleName;
  std::string        mBundleId;
  AbortHandler       mAbortHandler;

private: // impl members

  struct Impl;
  Impl* mImpl;

};

} // namespace Adaptor

} // namespace Internal

} // namespace Dali

#endif // __DALI_INTERNAL_FRAMEWORK_H__
