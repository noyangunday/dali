#ifndef __DALI_ACCESSIBILITY_ACTION_HANDLER_H__
#define __DALI_ACCESSIBILITY_ACTION_HANDLER_H__

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
#include <dali/public-api/common/dali-common.h>
#include <dali/public-api/events/touch-event.h>

namespace Dali
{

/**
 * AccessibilityActionHandler is an abstract interface, used by Dali to handle accessibility actions
 * passed by the accessibility manager.
 */
class AccessibilityActionHandler
{
public:

  /**
   * Change the accessibility status when Accessibility feature(screen-reader) turned on or off.
   * @return whether the status is changed or not.
   */
  virtual bool ChangeAccessibilityStatus() = 0;

  /**
   * Clear the accessibility focus from the current focused actor.
   * @return whether the focus is cleared or not.
   */
  virtual bool ClearAccessibilityFocus() = 0;

  /**
   * Perform the accessibility action associated with a scroll event.
   * @param touchEvent The touch point (and time) of the event.
   * @return whether the focus is cleared or not.
   */
  virtual bool AccessibilityActionScroll( Dali::TouchEvent& touchEvent ) = 0;

  /**
   * Perform the accessibility action to move focus to the previous focusable actor (by one finger flick up).
   * @param allowEndFeedback true if end of list feedback should be played when the focus is alread reached to the end
   * @return whether the accessibility action is performed or not.
   */
  virtual bool AccessibilityActionPrevious(bool allowEndFeedback) = 0;

  /**
   * Perform the accessibility action to move focus to the next focusable actor (by one finger flick down).
   * @param allowEndFeedback true if end of list feedback should be played when the focus is alread reached to the end
   * @return whether the accessibility action is performed or not.
   */
  virtual bool AccessibilityActionNext(bool allowEndFeedback) = 0;

  /**
   * Perform the accessibility action to move focus to the previous focusable actor (by one finger flick left).
   * @param allowEndFeedback true if end of list feedback should be played when the focus is alread reached to the end
   * @return whether the accessibility action is performed or not.
   */
  virtual bool AccessibilityActionReadPrevious(bool allowEndFeedback) = 0;

  /**
   * Perform the accessibility action to move focus to the next focusable actor (by one finger flick right).
   * @param allowEndFeedback true if end of list feedback should be played when the focus is alread reached to the end
   * @return whether the accessibility action is performed or not.
   */
  virtual bool AccessibilityActionReadNext(bool allowEndFeedback) = 0;

  /**
   * Perform the accessibility action to focus and read the actor (by one finger tap or move).
   * @param allowReadAgain true if the action read again the same object (i.e. read action)
   *                       false if the action just read when the focus object is changed (i.e. over action)
   * @return whether the accessibility action is performed or not.
   */
  virtual bool AccessibilityActionRead(bool allowReadAgain) = 0;

  /**
   * Perform the accessibility action to activate the current focused actor (by one finger double tap).
   * @return whether the accessibility action is performed or not.
   */
  virtual bool AccessibilityActionActivate() = 0;

  /**
   * Perform the accessibility action to change the value when the current focused actor is a slider
   * (by double finger down and move up and right).
   * @return whether the accessibility action is performed or not.
   */
  virtual bool AccessibilityActionUp() = 0;

  /**
   * Perform the accessibility action to change the value when the current focused actor is a slider
   * (by double finger down and move down and left).
   * @return whether the accessibility action is performed or not.
   */
  virtual bool AccessibilityActionDown() = 0;

  /**
   * Perform the accessibility action to navigate back (by two fingers circle draw).
   * @return whether the accessibility action is performed or not.
   */
  virtual bool AccessibilityActionBack() = 0;

  /**
   * Perform the accessibility action to scroll up the list and focus on the first item on the list
   * after the scrolling and read the item (by two finger swipe up).
   * @return whether the accessibility action is performed or not.
   */
  virtual bool AccessibilityActionScrollUp() = 0;

  /**
   * Perform the accessibility action to scroll down the list and focus on the first item on the list
   * after the scrolling and read the item (by two finger swipe down).
   * @return whether the accessibility action is performed or not.
   */
  virtual bool AccessibilityActionScrollDown() = 0;

  /**
   * Perform the accessibility action to scroll left to the previous page (by two finger swipe left).
   * @return whether the accessibility action is performed or not.
   */
  virtual bool AccessibilityActionPageLeft() = 0;

  /**
   * Perform the accessibility action to scroll right to the next page (by two finger swipe right).
   * @return whether the accessibility action is performed or not.
   */
  virtual bool AccessibilityActionPageRight() = 0;

  /**
   * Perform the accessibility action to scroll up to the previous page (by one finger swipe left and right).
   * @return whether the accessibility action is performed or not.
   */
  virtual bool AccessibilityActionPageUp() = 0;

  /**
   * Perform the accessibility action to scroll down to the next page (by one finger swipe right and left).
   * @return whether the accessibility action is performed or not.
   */
  virtual bool AccessibilityActionPageDown() = 0;

  /**
   * Perform the accessibility action to move the focus to the first item on the screen
   * (by one finger swipe up and down).
   * @return whether the accessibility action is performed or not.
   */
  virtual bool AccessibilityActionMoveToFirst() = 0;

  /**
   * Perform the accessibility action to move the focus to the last item on the screen
   * (by one finger swipe down and up).
   * @return whether the accessibility action is performed or not.
   */
  virtual bool AccessibilityActionMoveToLast() = 0;

  /**
   * Perform the accessibility action to move the focus to the first item on the top
   * and read from the top item continously (by three fingers single tap).
   * @return whether the accessibility action is performed or not.
   */
  virtual bool AccessibilityActionReadFromTop() = 0;

  /**
   * Perform the accessibility action to move the focus to and read from the next item
   * continously (by three fingers double tap).
   * @return whether the accessibility action is performed or not.
   */
  virtual bool AccessibilityActionReadFromNext() = 0;

  /**
   * Perform the accessibility action to move the focus to do the zooming (by one finger triple tap).
   * @return whether the accessibility action is performed or not.
   */
  virtual bool AccessibilityActionZoom() = 0;

  /**
   * Perform the accessibility action to read the information in the indicator (by two fingers triple tap).
   * @return whether the accessibility action is performed or not.
   */
  virtual bool AccessibilityActionReadIndicatorInformation() = 0;

  /**
   * Perform the accessibility action to pause/resume the current read out (by two fingers single tap).
   * @return whether the accessibility action is performed or not.
   */
  virtual bool AccessibilityActionReadPauseResume() = 0;

  /**
   * Perform the accessibility action to start/stop the current action (by two fingers double tap).
   * @return whether the accessibility action is performed or not.
   */
  virtual bool AccessibilityActionStartStop() = 0;

  /**
   * Perform the accessibility action to mouse move (by one finger tap & hold and move).
   * @param touchEvent touch event structure
   * @return whether the accessibility action is performed or not.
   */
  virtual bool AccessibilityActionTouch(const Dali::TouchEvent& touchEvent) = 0;

}; // class AccessibilityActionHandler

} // namespace Dali

#endif // __DALI_ACCESSIBILITY_ACTION_HANDLER_H__
