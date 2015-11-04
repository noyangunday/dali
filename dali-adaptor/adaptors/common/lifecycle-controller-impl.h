#ifndef __DALI_INTERNAL_LIFECYCLE_CONTROLLER_H__
#define __DALI_INTERNAL_LIFECYCLE_CONTROLLER_H__

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
#include <dali/public-api/object/ref-object.h>
#include <dali/public-api/object/base-object.h>

// INTERNAL INCLUDES
#include <lifecycle-controller.h>
#include <application.h>

namespace Dali
{

class Adaptor;

namespace Internal
{

namespace Adaptor
{

/**
 * This provides signals that are emitted according the lifecylce of the program.
 */
class LifecycleController : public BaseObject, public ConnectionTracker
{
public:

  // Creation & Destruction

  /**
   * Constructor.
   */
  LifecycleController();

  /**
   * Retrieve the initialized instance of the LifecycleController.
   * @return Handle to LifecycleController.
   */
  static Dali::LifecycleController Get();

  // Signals

  /**
   * @copydoc Dali::StyleMonitor::InitSignal()
   */
  Dali::LifecycleController::LifecycleSignalType& InitSignal();

  /**
   * @copydoc Dali::StyleMonitor::TerminateSignal()
   */
  Dali::LifecycleController::LifecycleSignalType& TerminateSignal();

  /**
   * @copydoc Dali::StyleMonitor::PauseSignal()
   */
  Dali::LifecycleController::LifecycleSignalType& PauseSignal();

  /**
   * @copydoc Dali::StyleMonitor::ResumeSignal()
   */
  Dali::LifecycleController::LifecycleSignalType& ResumeSignal();

  /**
   * @copydoc Dali::StyleMonitor::ResetSignal()
   */
  Dali::LifecycleController::LifecycleSignalType& ResetSignal();

  /**
   * @copydoc Dali::StyleMonitor::ResizeSignal()
   */
  Dali::LifecycleController::LifecycleSignalType& ResizeSignal();

  /**
   * @copydoc Dali::StyleMonitor::LanguageChangedSignal()
   */
  Dali::LifecycleController::LifecycleSignalType& LanguageChangedSignal();

public:

  /**
   * Called when the framework is initialised.
   *
   * @param[in] app The application instance
   */
  void OnInit( Dali::Application& app );

  /**
   * Called when the framework is terminated.
   *
   * @param[in] app The application instance
   */
  void OnTerminate( Dali::Application& app );

  /**
   * Called when the framework is paused.
   *
   * @param[in] app The application instance
   */
  void OnPause( Dali::Application& app );

  /**
   * Called when the framework resumes from a paused state.
   *
   * @param[in] app The application instance
   */
  void OnResume( Dali::Application& app );

  /**
   * Called when the framework informs the application that it should reset itself.
   *
   * @param[in] app The application instance
   */
  void OnReset( Dali::Application& app );

  /**
   * Called when the framework informs the application that the language of the device has changed.
   *
   * @param[in] app The application instance
   */
  void OnLanguageChanged( Dali::Application& app );

  /**
   * Signal handler when the adaptor's window resizes itself.
   *
   * @param[in] app The application instance
   */
  void OnResize( Dali::Application& app );

protected:

  /**
   * Virtual Destructor.
   */
  virtual ~LifecycleController();

private:

  /**
   * Emit the init signal.
   */
  void EmitInitSignal();

  /**
   * Emit the init signal.
   */
  void EmitTerminateSignal();

  /**
   * Emit the init signal.
   */
  void EmitPauseSignal();

  /**
   * Emit the init signal.
   */
  void EmitResumeSignal();

  /**
   * Emit the init signal.
   */
  void EmitResetSignal();

  /**
   * Emit the init signal.
   */
  void EmitResizeSignal();

  /**
   * Emit the init signal.
   */
  void EmitLanguageChangedSignal();

private:

  // Signals
  Dali::LifecycleController::LifecycleSignalType mInitSignal;
  Dali::LifecycleController::LifecycleSignalType mTerminateSignal;
  Dali::LifecycleController::LifecycleSignalType mPauseSignal;
  Dali::LifecycleController::LifecycleSignalType mResumeSignal;
  Dali::LifecycleController::LifecycleSignalType mResetSignal;
  Dali::LifecycleController::LifecycleSignalType mResizeSignal;
  Dali::LifecycleController::LifecycleSignalType mLanguageChangedSignal;

};

} // namespace Adaptor

} // namespace Internal

// Additional Helpers for public-api forwarding methods

inline Internal::Adaptor::LifecycleController& GetImplementation(Dali::LifecycleController& controller)
{
  DALI_ASSERT_ALWAYS(controller && "Controller handle is empty");
  BaseObject& handle = controller.GetBaseObject();
  return static_cast<Internal::Adaptor::LifecycleController&>(handle);
}

inline const Internal::Adaptor::LifecycleController& GetImplementation(const Dali::LifecycleController& controller)
{
  DALI_ASSERT_ALWAYS(controller && "Controller handle is empty");
  const BaseObject& handle = controller.GetBaseObject();
  return static_cast<const Internal::Adaptor::LifecycleController&>(handle);
}

} // namespace Dali

#endif // __DALI_INTERNAL_LIFECYCLE_CONTROLLER_H__
