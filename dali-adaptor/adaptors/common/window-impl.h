#ifndef __DALI_INTERNAL_WINDOW_H__
#define __DALI_INTERNAL_WINDOW_H__

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
#include <base/lifecycle-observer.h>
#include <adaptor-impl.h>
#include <indicator-impl.h>
#include <window.h>
#include <orientation.h>
#include <render-surface.h>
#include <drag-and-drop-detector.h>

namespace Dali
{
class Adaptor;
class RenderSurface;

namespace Integration
{
class SystemOverlay;
}

namespace Internal
{
namespace Adaptor
{
class Indicator;
class Orientation;

class Window;
typedef IntrusivePtr<Window> WindowPtr;
typedef IntrusivePtr<Orientation> OrientationPtr;

/**
 * Window provides a surface to render onto with orientation & indicator properties.
 */
class Window : public Dali::BaseObject, public Indicator::Observer, public LifeCycleObserver
{
public:
  typedef Dali::Window::IndicatorSignalType IndicatorSignalType;
  typedef Signal< void () > SignalType;

  /**
   * Create a new Window. This should only be called once by the Application class
   * @param[in] windowPosition The position and size of the window
   * @param[in] name The window title
   * @param[in] className The window class name
   * @param[in] isTransparent Whether window is transparent
   * @return A newly allocated Window
   */
  static Window* New(const PositionSize& posSize, const std::string& name, const std::string& className, bool isTransparent = false);

  /**
   * Pass the adaptor back to the overlay. This allows the window to access Core's overlay.
   * @param[in] adaptor An initialized adaptor
   */
  void SetAdaptor(Dali::Adaptor& adaptor);

  /**
   * Get the window surface
   * @return The render surface
   */
  RenderSurface* GetSurface();

  /**
   * @copydoc Dali::Window::ShowIndicator()
   */
  void ShowIndicator( Dali::Window::IndicatorVisibleMode visibleMode );

  /**
   * @copydoc Dali::Window::SetIndicatorBgOpacity()
   */
  void SetIndicatorBgOpacity( Dali::Window::IndicatorBgOpacity opacity );

  /**
   * @copydoc Dali::Window::RotateIndicator()
   */
  void RotateIndicator( Dali::Window::WindowOrientation orientation );

  /**
   * @copydoc Dali::Window::SetClass()
   */
  void SetClass( std::string name, std::string klass );

  /**
   * @copydoc Dali::Window::Raise()
   */
  void Raise();

  /**
   * @copydoc Dali::Window::Lower()
   */
  void Lower();

  /**
   * @copydoc Dali::Window::Activate()
   */
  void Activate();

  /**
   * @copydoc Dali::Window::AddAvailableOrientation()
   */
  void AddAvailableOrientation(Dali::Window::WindowOrientation orientation);

  /**
   * @copydoc Dali::Window::RemoveAvailableOrientation()
   */
  void RemoveAvailableOrientation(Dali::Window::WindowOrientation orientation);

  /**
   * @copydoc Dali::Window::SetAvailableOrientations()
   */
  void SetAvailableOrientations(const std::vector<Dali::Window::WindowOrientation>& orientations);

  /**
   * @copydoc Dali::Window::GetAvailableOrientations()
   */
  const std::vector<Dali::Window::WindowOrientation>& GetAvailableOrientations();

  /**
   * @copydoc Dali::Window::SetPreferredOrientation()
   */
  void SetPreferredOrientation(Dali::Window::WindowOrientation orientation);

  /**
   * @copydoc Dali::Window::GetPreferredOrientation()
   */
  Dali::Window::WindowOrientation GetPreferredOrientation();

  /**
   * @copydoc Dali::Window::GetDragAndDropDetector() const
   */
  Dali::DragAndDropDetector GetDragAndDropDetector() const;

  /**
   * @copydoc Dali::Window::GetNativeHandle() const
   */
  Dali::Any GetNativeHandle() const;

  /**
   * Called from Orientation after the Change signal has been sent
   */
  void RotationDone( int orientation, int width, int height );

private:
  /**
   * Private constructor.
   * @sa Window::New()
   */
  Window();

  /**
   * Destructor
   */
  virtual ~Window();

  /**
   * Second stage initialization
   */
  void Initialize(const PositionSize& posSize, const std::string& name, const std::string& className);

  /**
   * Shows / hides the indicator bar.
   * Handles close/open if rotation changes whilst hidden
   */
  void DoShowIndicator( Dali::Window::WindowOrientation lastOrientation );

  /**
   * Close current indicator and open a connection onto the new indicator service.
   * Effect may not be synchronous if waiting for an indicator update on existing connection.
   */
  void DoRotateIndicator( Dali::Window::WindowOrientation orientation );

  /**
   * Change the indicator actor's rotation to match the current orientation
   */
  void SetIndicatorActorRotation();

  /**
   * Set the indicator properties on the window
   */
  void SetIndicatorProperties( bool isShown, Dali::Window::WindowOrientation lastOrientation );

private: // Indicator::Observer interface

  /**
   * @copydoc Dali::Internal::Adaptor::Indicator::Observer::IndicatorTypeChanged()
   */
  virtual void IndicatorTypeChanged( Indicator::Type type );

  /**
   * @copydoc Dali::Internal::Adaptor::Indicator::Observer::IndicatorClosed()
   */
  virtual void IndicatorClosed(Indicator* indicator);

  /**
   * @copydoc Dali::Internal::Adaptor::Indicator::Observer::IndicatorVisibilityChanged()
   */
  virtual void IndicatorVisibilityChanged( bool isVisible );

private: // Adaptor::Observer interface

  /**
   * @copydoc Dali::Internal::Adaptor::Adaptor::Observer::OnStart()
   */
  virtual void OnStart();

  /**
   * @copydoc Dali::Internal::Adaptor::Adaptor::Observer::OnPause()
   */
  virtual void OnPause();

  /**
   * @copydoc Dali::Internal::Adaptor::Adaptor::Observer::OnResume()
   */
  virtual void OnResume();

  /**
   * @copydoc Dali::Internal::Adaptor::Adaptor::Observer::OnStop()
   */
  virtual void OnStop();

  /**
   * @copydoc Dali::Internal::Adaptor::Adaptor::Observer::OnDestroy()
   */
  virtual void OnDestroy();

public: // Signals

  /**
   * The user should connect to this signal to get a timing when indicator was shown / hidden.
   */
  IndicatorSignalType& IndicatorVisibilityChangedSignal() { return mIndicatorVisibilityChangedSignal; }

  /**
   * This signal is emitted when the window is requesting to be deleted
   */
  SignalType& DeleteRequestSignal() { return mDeleteRequestSignal; }

private:

  typedef std::vector<Indicator*> DiscardedIndicators;

  RenderSurface*                   mSurface;
  Dali::Window::IndicatorVisibleMode mIndicatorVisible; ///< public state
  bool                             mIndicatorIsShown:1; ///< private state
  bool                             mShowRotatedIndicatorOnClose:1;
  bool                             mStarted:1;
  bool                             mIsTransparent:1;
  bool                             mWMRotationAppSet:1;
  bool                             mEcoreEventHander:1;
  Indicator*                       mIndicator;
  Dali::Window::WindowOrientation  mIndicatorOrientation;
  Dali::Window::WindowOrientation  mNextIndicatorOrientation;
  Dali::Window::IndicatorBgOpacity mIndicatorOpacityMode;
  Integration::SystemOverlay*      mOverlay;
  Adaptor*                         mAdaptor;
  Dali::DragAndDropDetector        mDragAndDropDetector;

  struct EventHandler;
  EventHandler*                    mEventHandler;

  OrientationPtr                               mOrientation;
  std::vector<Dali::Window::WindowOrientation> mAvailableOrientations;
  Dali::Window::WindowOrientation              mPreferredOrientation;

  // Signals
  IndicatorSignalType mIndicatorVisibilityChangedSignal;
  SignalType          mDeleteRequestSignal;
};

} // namespace Adaptor
} // namepsace Internal

// Helpers for public-api forwarding methods

inline Internal::Adaptor::Window& GetImplementation(Dali::Window& window)
{
  DALI_ASSERT_ALWAYS( window && "Window handle is empty" );
  BaseObject& object = window.GetBaseObject();
  return static_cast<Internal::Adaptor::Window&>(object);
}

inline const Internal::Adaptor::Window& GetImplementation(const Dali::Window& window)
{
  DALI_ASSERT_ALWAYS( window && "Window handle is empty" );
  const BaseObject& object = window.GetBaseObject();
  return static_cast<const Internal::Adaptor::Window&>(object);
}

} // namespace Dali


#endif // __DALI_INTERNAL_WINDOW_H__
