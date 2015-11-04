#ifndef __DALI_ACCESSIBILITY_ADAPTOR_H__
#define __DALI_ACCESSIBILITY_ADAPTOR_H__

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
#include <dali/public-api/events/touch-event.h>
#include <dali/public-api/object/base-handle.h>

namespace Dali
{

namespace Internal DALI_INTERNAL
{
namespace Adaptor
{
class AccessibilityAdaptor;
}
}

class AccessibilityActionHandler;
class AccessibilityGestureHandler;
class TouchPoint;

/**
 * @brief The AccessibilityAdaptor provides communication to the indicator and the accessibility manager interface (implemented in toolkit).
 *
 */
class DALI_IMPORT_API AccessibilityAdaptor : public BaseHandle
{
public:

  /**
   * @brief Create an uninitialized handle.
   *
   * This can be initialized by calling getting the adaptor from Dali::Adaptor.
   */
  AccessibilityAdaptor();

  /**
   * @brief Retrieve a handle to the AccessibilityAdaptor.
   *
   * @return A handle to the AccessibilityAdaptor.
   */
  static AccessibilityAdaptor Get();

  /**
   * @brief Destructor
   *
   * This is non-virtual since derived Handle types must not contain data or virtual methods.
   */
  ~AccessibilityAdaptor();

  /**
   * @brief Returns the current position of the read action.
   * @return The current event position.
   */
  Vector2 GetReadPosition() const;

  /**
   * @brief Query whether the accessibility(screen-reader) is enabled.
   *
   * The accessibility will be enabled by system setting.
   * @return True if the accessibility(screen-reader) is enabled.
   */
  bool IsEnabled() const;

  /**
   * @brief Set the handler to handle accessibility actions.
   *
   * @param[in] handler The Accessibility action handler.
   * @note Handlers should remove themselves when they are destroyed.
   */
  void SetActionHandler(AccessibilityActionHandler& handler);

  /**
   * @brief Set the handler to handle accessibility gestures.
   *
   * @param[in] handler The Accessibility gesture handler.
   * @note Handlers should remove themselves when they are destroyed.
   */
  void SetGestureHandler(AccessibilityGestureHandler& handler);

  /**
   * @brief Handle the accessibility action to move focus to the next focusable actor
   * (by one finger flick down).
   *
   * @param allowEndFeedback true if end of list feedback should be played when the focus is alread reached to the end
   * @return Whether the action is performed successfully or not.
   */
  bool HandleActionNextEvent(bool allowEndFeedback = true);

  /**
   * @brief Handle the accessibility action to move focus to the previous focusable actor
   * (by one finger flick up).
   *
   * @param allowEndFeedback true if end of list feedback should be played when the focus is alread reached to the end
   * @return Whether the action is performed successfully or not.
   */
  bool HandleActionPreviousEvent(bool allowEndFeedback = true);

  /**
   * @brief Handle the accessibility action to activate the current focused actor (by one
   * finger )
   *
   * @return Whether the action is performed successfully or not.
   */
  bool HandleActionActivateEvent();

  /**
   * @brief Handle the accessibility action to focus and read the actor (by one finger tap or move).
   *
   * @param x x position of event
   * @param y y position of event
   * @param allowReadAgain true if the action read again the same object (i.e. read action)
   *                       false if the action just read when the focus object is changed (i.e. over action)
   * @return Whether the action is performed successfully or not.
   */
  bool HandleActionReadEvent(unsigned int x, unsigned int y, bool allowReadAgain);

  /**
   * @brief Handle the accessibility action to move focus to the next focusable actor
   * (by one finger flick right).
   *
   * @param allowEndFeedback true if end of list feedback should be played when the focus is alread reached to the end
   * @return Whether the action is performed successfully or not.
   */
  bool HandleActionReadNextEvent(bool allowEndFeedback = true);

  /**
   * @brief Handle the accessibility action to move focus to the previous focusable actor
   * (by one finger flick up).
   *
   * @param allowEndFeedback true if end of list feedback should be played when the focus is alread reached to the front
   * @return Whether the action is performed successfully or not.
   */
  bool HandleActionReadPreviousEvent(bool allowEndFeedback = true);

  /**
   * @brief Handle the accessibility action to change the value when the current focused
   * actor is a slider (by double finger down and move up and right).
   *
   * @return Whether the action is performed successfully or not.
   */
  bool HandleActionUpEvent();

  /**
   * @brief Handle the accessibility action to change the value when the current focused
   * actor is a slider (by double finger down and move down and left).
   *
   * @return Whether the action is performed successfully or not.
   */
  bool HandleActionDownEvent();

  /**
   * @brief Handle the accessibility action to clear the focus from the current focused
   * actor if any, so that no actor is focused in the focus chain.
   *
   * @return Whether the action is performed successfully or not.
   */
  bool HandleActionClearFocusEvent();

  /**
   * @brief Handle the accessibility action to scroll when there is a scroller on the touched position
   * (by 2 finger touch & move, 2 finger flick).
   *
   * @param[in]  point      The touch point information.
   * @param[in]  timeStamp  The time the touch occurred.
   * @return Whether the action is performed successfully or not.
   */
  bool HandleActionScrollEvent(const TouchPoint& point, unsigned long timeStamp);

  /**
   * @brief Handle the accessibility action to move for the current focused actor
   * (by 1 finger tap & hold and move).
   *
   * @param[in]  point      The touch point information.
   * @param[in]  timeStamp  The time the touch occurred.
   * @return Whether the action is performed successfully or not.
   */
  bool HandleActionTouchEvent(const TouchPoint& point, unsigned long timeStamp);

  /**
   * @brief Handle the accessibility action to navigate back (by two fingers circle draw).
   * @return Whether the action is performed successfully or not.
   */
  bool HandleActionBackEvent();

  /**
   * @brief Handle the accessibility action to enable the feature.
   */
  void HandleActionEnableEvent();

  /**
   * @brief Handle the accessibility action to disable the feature.
   */
  void HandleActionDisableEvent();

  /**
   * @brief Handle the accessibility action to scroll up the list and focus on
   * the first item on the list after the scrolling and read the item
   * (by two finger swipe up).
   *
   * @return Whether the action is performed successfully or not.
   */
  bool HandleActionScrollUpEvent();

  /**
   * @brief Handle the accessibility action to scroll down the list and focus on
   * the first item on the list after the scrolling and read the item
   * (by two finger swipe down).
   *
   * @return Whether the action is performed successfully or not.
   */
  bool HandleActionScrollDownEvent();

  /**
   * @brief Handle the accessibility action to scroll left to the previous page
   * (by two finger swipe left).
   *
   * @return Whether the action is performed successfully or not.
   */
  bool HandleActionPageLeftEvent();

  /**
   * @brief Handle the accessibility action to scroll right to the next page
   * (by two finger swipe right).
   *
   * @return Whether the action is performed successfully or not.
   */
  bool HandleActionPageRightEvent();

  /**
   * @brief Handle the accessibility action to scroll up to the previous page
   * (by one finger swipe left and right).
   *
   * @return Whether the action is performed successfully or not.
   */
  bool HandleActionPageUpEvent();

  /**
   * @brief Handle the accessibility action to scroll down to the next page
   * (by one finger swipe right and left).
   *
   * @return Whether the action is performed successfully or not.
   */
  bool HandleActionPageDownEvent();

  /**
   * @brief Handle the accessibility action to move the focus to the first item on the screen
   * (by one finger swipe up and down).
   *
   * @return Whether the action is performed successfully or not.
   */
  bool HandleActionMoveToFirstEvent();

  /**
   * @brief Handle the accessibility action to move the focus to the last item on the screen
   * (by one finger swipe down and up).
   *
   * @return Whether the action is performed successfully or not.
   */
  bool HandleActionMoveToLastEvent();

  /**
   * @brief Handle the accessibility action to move the focus to the first item on the top
   * and read from the top item continously (by three fingers single tap).
   *
   * @return Whether the action is performed successfully or not.
   */
  bool HandleActionReadFromTopEvent();

  /**
   * @brief Handle the accessibility action to move focus to and read from the next focusable
   * actor continously (by three fingers double tap).
   *
   * @return Whether the action is performed successfully or not.
   */
  bool HandleActionReadFromNextEvent();

  /**
   * @brief Handle the accessibility action to do the zooming
   * (by one finger triple tap).
   *
   * @return Whether the action is performed successfully or not.
   */
  bool HandleActionZoomEvent();

  /**
   * @brief Handle the accessibility action to read the information in the indicator
   * (by two fingers triple tap).
   *
   * @return Whether the action is performed successfully or not.
   */
  bool HandleActionReadIndicatorInformationEvent();

  /**
   * @brief Handle the accessibility action to pause/resume the current speech
   * (by two fingers single tap).
   *
   * @return Whether the action is performed successfully or not.
   */
  bool HandleActionReadPauseResumeEvent();

  /**
   * @brief Handle the accessibility action to start/stop the current action
   * (by two fingers double tap).
   *
   * @return Whether the action is performed successfully or not.
   */
  bool HandleActionStartStopEvent();

public: // Not intended for application developers

  /**
   * @brief Creates a handle using the Adaptor::Internal implementation.
   *
   * @param[in] adaptor The AccessibilityAdaptor implementation.
   */
  DALI_INTERNAL AccessibilityAdaptor( Internal::Adaptor::AccessibilityAdaptor& adaptor );

  /**
   * @brief This constructor is used by AccessibilityAdaptor::Get().
   *
   * @param[in] adaptor A pointer to the accessibility adaptor.
   */
  explicit DALI_INTERNAL AccessibilityAdaptor( Internal::Adaptor::AccessibilityAdaptor* adaptor );
};

} // namespace Dali

#endif // __DALI_ACCESSIBILITY_ADAPTOR_H__
