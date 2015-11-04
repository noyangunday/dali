#ifndef __DALI_INTERNAL_DRAG_AND_DROP_DETECTOR_H__
#define __DALI_INTERNAL_DRAG_AND_DROP_DETECTOR_H__

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

#include <dali/public-api/object/base-object.h>
#include <dali/public-api/math/vector2.h>

// INTERNAL INCLUDES
#include <drag-and-drop-detector.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

typedef IntrusivePtr< DragAndDropDetector > DragAndDropDetectorPtr;

/**
 * This class listens to Drag & Drop events.
 */
class DragAndDropDetector : public Dali::BaseObject
{
public:

  typedef Dali::DragAndDropDetector::DragAndDropSignal DragAndDropSignal;

  // Creation

  /**
   * Create a DragAndDropDetector.
   * This should only be called once by the Window class.
   * @return A newly allocated drag-and-drop-detector.
   */
  static Dali::DragAndDropDetector New();

  // Public API

  /**
   * @copydoc Dali::DragAndDropDetector::GetContent() const
   */
  const std::string& GetContent() const;

  /**
   * @copydoc Dali::DragAndDropDetector::GetCurrentScreenPosition() const
   */
  Vector2 GetCurrentScreenPosition() const;

  // Called by Drag & Drop Event Handler

  /**
   * Queries whether drag & drop behaviour is really required.
   * @return true if drag & drop required, false otherwise.
   */
  bool IsEnabled() const;

  /**
   * Sets the dragged content.
   * @param[in] content  A string that represents the content that has been dropped.
   */
  void SetContent( const std::string& content );

  /**
   * Clears the stored content.
   */
  void ClearContent();

  /**
   * Sets the position the drop occurred.
   */
  void SetPosition( Vector2 screenPosition );

  /**
   * Called when a draggable object enters our window.
   */
  void EmitEnteredSignal();

  /**
   * Called when a draggable object leaves our window.
   */
  void EmitExitedSignal();

  /**
   * Called when a draggable object leaves our window.
   */
  void EmitMovedSignal();

  /**
   * Is called when a drop actually occurs.
   */
  void EmitDroppedSignal();

public: // Signals

  /**
   * @copydoc Dali::DragAndDropDetector::EnteredSignal
   */
  DragAndDropSignal& EnteredSignal()
  {
    return mEnteredSignal;
  }

  /**
   * @copydoc Dali::DragAndDropDetector::ExitedSignal
   */
  DragAndDropSignal& ExitedSignal()
  {
    return mExitedSignal;
  }

  /**
   * @copydoc Dali::DragAndDropDetector::MovedSignal
   */
  DragAndDropSignal& MovedSignal()
  {
    return mMovedSignal;
  }

  /**
   * @copydoc Dali::DragAndDropDetector::DroppedSignal
   */
  DragAndDropSignal& DroppedSignal()
  {
    return mDroppedSignal;
  }

private:

  // Construction & Destruction

  /**
   * Constructor.
   */
  DragAndDropDetector();

  /**
   * Destructor.
   */
  virtual ~DragAndDropDetector();

  // Undefined
  DragAndDropDetector( const DragAndDropDetector& );
  DragAndDropDetector& operator=( DragAndDropDetector& );

private:

  std::string mContent;    ///< The current Drag & drop content.
  Vector2 mScreenPosition; ///< The screen position of the drop location.

  DragAndDropSignal mEnteredSignal;
  DragAndDropSignal mExitedSignal;
  DragAndDropSignal mMovedSignal;
  DragAndDropSignal mDroppedSignal;
};

} // namespace Adaptor

} // namespace Internal

// Helpers for public-api forwarding methods

inline Internal::Adaptor::DragAndDropDetector& GetImplementation(Dali::DragAndDropDetector& detector)
{
  DALI_ASSERT_ALWAYS( detector && "DragAndDropDetector handle is empty" );

  BaseObject& handle = detector.GetBaseObject();

  return static_cast<Internal::Adaptor::DragAndDropDetector&>(handle);
}

inline const Internal::Adaptor::DragAndDropDetector& GetImplementation(const Dali::DragAndDropDetector& detector)
{
  DALI_ASSERT_ALWAYS( detector && "DragAndDropDetector handle is empty" );

  const BaseObject& handle = detector.GetBaseObject();

  return static_cast<const Internal::Adaptor::DragAndDropDetector&>(handle);
}

} // namespace Dali

#endif // __DALI_INTERNAL_DRAG_AND_DROP_DETECTOR_H__
