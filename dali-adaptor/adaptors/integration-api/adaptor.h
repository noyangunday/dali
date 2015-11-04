#ifndef __DALI_INTEGRATION_ADAPTOR_H__
#define __DALI_INTEGRATION_ADAPTOR_H__

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
#include <dali/public-api/signals/callback.h>
#include <dali/public-api/signals/dali-signal.h>
#include <dali/public-api/math/rect.h>
#include <dali/public-api/events/touch-event.h>
#include <dali/public-api/common/view-mode.h>

// INTERNAL INCLUDES


#ifdef DALI_ADAPTOR_COMPILATION  // full path doesn't exist until adaptor is installed so we have to use relative
// @todo Make dali-adaptor code folder structure mirror the folder structure installed to dali-env
#include <window.h>
#include <application-configuration.h>
#else
#include <dali/public-api/adaptor-framework/window.h>
#include <dali/public-api/adaptor-framework/application-configuration.h>
#endif


namespace Dali
{

class RenderSurface;

namespace Internal
{
namespace Adaptor
{
class Adaptor;
}
}

/**
 * @brief An Adaptor object is used to initialize and control how Dali runs.
 *
 * It provides a lifecycle interface that allows the application
 * writer to provide their own main loop and other platform related
 * features.
 *
 * The Adaptor class provides a means for initialising the resources required by the Dali::Core.
 *
 * When dealing with platform events, the application writer MUST ensure that Dali is called in a
 * thread-safe manner.
 *
 * As soon as the Adaptor class is created and started, the application writer can initialise their
 * Dali::Actor objects straight away or as required by the main loop they intend to use (there is no
 * need to wait for an initialise signal as per the Dali::Application class).
 *
 * The Adaptor does emit a Resize signal which informs the user when the surface is resized.
 * Tizen and Linux Adaptors should follow the example below:
 *
 * @code
 * void CreateProgram(DaliAdaptor& adaptor)
 * {
 *   // Create Dali components...
 *   // Can instantiate adaptor here instead, if required
 * }
 *
 * int main ()
 * {
 *   // Initialise platform
 *   MyPlatform.Init();
 *
 *   // Create an 800 by 1280 window positioned at (0,0).
 *   Dali::PositionSize positionSize(0, 0, 800, 1280);
 *   Dali::Window window = Dali::Window::New( positionSize, "My Application" );
 *
 *   // Create an adaptor which uses that window for rendering
 *   Dali::Adaptor adaptor = Dali::Adaptor::New( window );
 *   adaptor.Start();
 *
 *   CreateProgram(adaptor);
 *   // Or use this as a callback function depending on the platform initialisation sequence.
 *
 *   // Start Main Loop of your platform
 *   MyPlatform.StartMainLoop();
 *
 *   return 0;
 * }
 * @endcode
 *
 * If required, you can also connect class member functions to a signal:
 *
 * @code
 * MyApplication application;
 * adaptor.ResizedSignal().Connect(&application, &MyApplication::Resize);
 * @endcode
 *
 * @see RenderSurface
 */
class DALI_IMPORT_API Adaptor
{
public:

  typedef Signal< void (Adaptor&) > AdaptorSignalType; ///< Generic Type for adaptor signals

public:
  /**
   * @brief Create a new adaptor using the window.
   *
   * @param[in] window The window to draw onto
   * @return a reference to the adaptor handle
   */
  static Adaptor& New( Window window );

  /**
   * @brief Create a new adaptor using the window.
   *
   * @param[in] window The window to draw onto
   * @param[in] configuration The context loss configuration.
   * @return a reference to the adaptor handle
   */
  static Adaptor& New( Window window, Configuration::ContextLoss configuration );

  /**
   * @brief Create a new adaptor using render surface.
   *
   * @param[in] nativeWindow native window handle
   * @param[in] surface The surface to draw onto
   * @return a reference to the adaptor handle
   */
  static Adaptor& New( Any nativeWindow, const Dali::RenderSurface& surface );

  /**
   * @brief Create a new adaptor using render surface.
   *
   * @param[in] nativeWindow native window handle
   * @param[in] surface The surface to draw onto
   * @param[in] configuration The context loss configuration.
   * @return a reference to the adaptor handle
   */
  static Adaptor& New( Any nativeWindow, const Dali::RenderSurface& surface, Configuration::ContextLoss configuration = Configuration::APPLICATION_DOES_NOT_HANDLE_CONTEXT_LOSS);

  /**
   * @brief Virtual Destructor.
   */
  virtual ~Adaptor();

public:

  /**
   * @brief Starts the Adaptor.
   */
  void Start();

  /**
   * @brief Pauses the Adaptor.
   */
  void Pause();

  /**
   * @brief Resumes the Adaptor, if previously paused.
   *
   * @note If the adaptor is not paused, this does not do anything.
   */
  void Resume();

  /**
   * @brief Stops the Adaptor.
   */
  void Stop();

  /**
   * @brief Ensures that the function passed in is called from the main loop when it is idle.
   * @note Function must be called from the main event thread only.
   *
   * A callback of the following type may be used:
   * @code
   *   void MyFunction();
   * @endcode
   *
   * @param[in] callback The function to call.
   * @return true if added successfully, false otherwise
   *
   * @note Ownership of the callback is passed onto this class.
   */
  bool AddIdle( CallbackBase* callback );

  /**
   * @brief Replaces the rendering surface
   *
   * @param[in] nativeWindow native window handle
   * @param[in] surface to use
   */
  void ReplaceSurface( Any nativeWindow, Dali::RenderSurface& surface );

  /**
   * @brief Get the render surface the adaptor is using to render to.
   *
   * @return reference to current render surface
   */
  RenderSurface& GetSurface();

  /**
   * @brief Release any locks the surface may hold.
   *
   * For example, after compositing an offscreen surface, use this method to allow
   * rendering to continue.
   */
  void ReleaseSurfaceLock();

  /**
   * @brief Set the number of frames per render.
   *
   * This enables an application to deliberately render with a reduced FPS.
   * @param[in] numberOfVSyncsPerRender The number of vsyncs between successive renders.
   * Suggest this is a power of two:
   * 1 - render each vsync frame
   * 2 - render every other vsync frame
   * 4 - render every fourth vsync frame
   * 8 - render every eighth vsync frame
   */
  void SetRenderRefreshRate( unsigned int numberOfVSyncsPerRender );

  /**
   * @brief Set whether the frame count per render is managed using the hardware VSync or
   * manually timed.
   *
   * @param[in] useHardware True if the hardware VSync should be used
   */
  void SetUseHardwareVSync(bool useHardware);

  /**
   * @brief Returns a reference to the instance of the adaptor used by the current thread.
   *
   * @return A reference to the adaptor.
   * @pre The adaptor has been initialised.
   * @note This is only valid in the main thread.
   */
  static Adaptor& Get();

  /**
   * @brief Checks whether the adaptor is available.
   *
   * @return true, if it is available, false otherwise.
   */
  static bool IsAvailable();

  /**
   * @brief Call this method to notify Dali when scene is created and initialized.
   *
   * Notify Adaptor that the scene has been created.
   */
  void NotifySceneCreated();

  /**
   * @brief Call this method to notify Dali when the system language changes.
   *
   * Use this only when NOT using Dali::Application, As Application created using
   * Dali::Application will automatically receive notification of language change.
   * When Dali::Application is not used, the application developer should
   * use app-core to receive language change notifications and should update Dali
   * by calling this method.
   */
  void NotifyLanguageChanged();

  /**
   * @brief Sets minimum distance in pixels that the fingers must move towards/away from each other in order to
   * trigger a pinch gesture
   *
   * @param[in] distance The minimum pinch distance in pixels
   */
  void SetMinimumPinchDistance(float distance);

  /**
   * @brief Feed a touch point to the adaptor.
   *
   * @param[in] point touch point
   * @param[in] timeStamp time value of event
   */
  void FeedTouchPoint( TouchPoint& point, int timeStamp );

  /**
   * @brief Feed a wheel event to the adaptor.
   *
   * @param[in]  wheelEvent wheel event
   */
  void FeedWheelEvent( WheelEvent& wheelEvent );

  /**
   * @brief Feed a key event to the adaptor.
   *
   * @param[in] keyEvent The key event holding the key information.
   */
  void FeedKeyEvent( KeyEvent& keyEvent );

  /**
   * @copydoc Dali::Core::SceneCreated();
   */
  void SceneCreated();

  /**
   * @copydoc Dali::Application::SetViewMode();
   */
  void SetViewMode( ViewMode viewMode );

  /**
   * @copydoc Dali::Application::SetStereoBase();
   */
  void SetStereoBase( float stereoBase );

public:  // Signals

  /**
   * @brief The user should connect to this signal if they need to perform any
   * special activities when the surface Dali is being rendered on is resized.
   *
   * @return The signal to connect to
   */
  AdaptorSignalType& ResizedSignal();

  /**
   * @brief This signal is emitted when the language is changed on the device.
   *
   * @return The signal to connect to
   */
  AdaptorSignalType& LanguageChangedSignal();

private:

  // Undefined
  Adaptor(const Adaptor&);
  Adaptor& operator=(Adaptor&);

private:

  /**
   * @brief Create an uninitialized Adaptor.
   */
  Adaptor();

  Internal::Adaptor::Adaptor* mImpl; ///< Implementation object
  friend class Internal::Adaptor::Adaptor;
};

} // namespace Dali

#endif // __DALI_INTEGRATION_ADAPTOR_H__
