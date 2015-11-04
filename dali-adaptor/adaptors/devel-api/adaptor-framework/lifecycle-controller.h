#ifndef __DALI_LIFECYCLE_CONTROLLER_H__
#define __DALI_LIFECYCLE_CONTROLLER_H__

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

// EXTERNAL INCLUDES
#include <dali/public-api/object/base-handle.h>
#include <dali/public-api/signals/dali-signal.h>

namespace Dali
{

namespace Internal DALI_INTERNAL
{
namespace Adaptor
{
class LifecycleController;
}
}

/**
 * @brief Provides application lifecycle events.
 *
 * Connect to the signals of this class to receive notification of events in the lifecycle
 * of the application. The following example shows how to connect to the Init signal. This
 * could be done for example in the constructor of a singleton that is known to be created
 * at startup.
 *
 * @code
 * void MyClass::MyClass()
 * {
 *   LifecycleController::Get().InitSignal().Connect( this, &MyClass::OnApplicationInit );
 * }
 *
 * void MyClass::OnApplicationInit()
 * {
 *   // ... Do something on init
 * }
 * @endcode
 */
class DALI_IMPORT_API LifecycleController : public BaseHandle
{
public: // Typedefs

  typedef Signal< void (void) > LifecycleSignalType;   ///< Lifecycle Signal type

public: // Creation & Destruction

  /**
   * @brief Create an uninitialized LifecycleController handle.
   *
   * Calling member functions when uninitialized is not allowed.
   */
  LifecycleController();

  /**
   * @brief Creates a copy of the handle.
   *
   * The copy will point to the same implementation as the original.
   * @param[in]  monitor  The LifecycleController to copy from.
   */
  LifecycleController(const LifecycleController& monitor);

  /**
   * @brief Retrieve the initialized instance of the LifecycleController.
   * @return Handle to LifecycleController.
   */
  static LifecycleController Get();

  /**
   * @brief Destructor
   *
   * This is non-virtual since derived Handle types must not contain data or virtual methods.
   */
  ~LifecycleController();

public: // Signals

  /**
   * The user should connect to this signal to determine when they should initialise
   * their application.
   */
  LifecycleSignalType& InitSignal();

  /**
   * The user should connect to this signal to determine when they should terminate
   * their application
   */
  LifecycleSignalType& TerminateSignal();

  /**
   * The user should connect to this signal if they need to perform any special
   * activities when the application is about to be paused.
   */
  LifecycleSignalType& PauseSignal();

  /**
   * The user should connect to this signal if they need to perform any special
   * activities when the application has resumed.
   */
  LifecycleSignalType& ResumeSignal();

  /**
   * This signal is sent when the system requires the user to reinitialise itself.
   */
  LifecycleSignalType& ResetSignal();

  /**
   * This signal is emitted when the window the application is rendering on is resized.
   */
  LifecycleSignalType& ResizeSignal();

  /**
   * This signal is emitted when the language is changed on the device.
   */
  LifecycleSignalType& LanguageChangedSignal();

public: // Operators

  /**
   * @brief Assignment operator.
   *
   * The handle points to the same implementation as the one being copied from.
   * @param[in]  controller  The LifecycleController to copy from.
   * @return reference to this object
   */
  LifecycleController& operator=(const LifecycleController& controller);

public: // Not intended for application developers
  /**
   * @brief This constructor is used internally to create a handle from an object pointer.
   * @param [in] lifecycleController A pointer to the internal LifecycleController.
   */
  explicit DALI_INTERNAL LifecycleController(Internal::Adaptor::LifecycleController* lifecycleController);
};

} // namespace Dali

#endif // __DALI_LIFECYCLE_CONTROLLER_H__
