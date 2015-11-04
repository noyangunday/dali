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

// CLASS HEADER
#include <accessibility-adaptor.h>

// INTERNAL INCLUDES
#include <accessibility-adaptor-impl.h>

namespace Dali
{

AccessibilityAdaptor::AccessibilityAdaptor()
{
}

AccessibilityAdaptor AccessibilityAdaptor::Get()
{
  return Internal::Adaptor::AccessibilityAdaptor::Get();
}

AccessibilityAdaptor::~AccessibilityAdaptor()
{
}

Vector2 AccessibilityAdaptor::GetReadPosition() const
{
  return Internal::Adaptor::AccessibilityAdaptor::GetImplementation(*this).GetReadPosition();
}

bool AccessibilityAdaptor::IsEnabled() const
{
  return Internal::Adaptor::AccessibilityAdaptor::GetImplementation(*this).IsEnabled();
}

void AccessibilityAdaptor::SetActionHandler(AccessibilityActionHandler& handler)
{
  Internal::Adaptor::AccessibilityAdaptor::GetImplementation(*this).SetActionHandler(handler);
}

void AccessibilityAdaptor::SetGestureHandler(AccessibilityGestureHandler& handler)
{
  Internal::Adaptor::AccessibilityAdaptor::GetImplementation(*this).SetGestureHandler(handler);
}

bool AccessibilityAdaptor::HandleActionNextEvent(bool allowEndFeedback)
{
  return Internal::Adaptor::AccessibilityAdaptor::GetImplementation(*this).HandleActionNextEvent(allowEndFeedback);
}

bool AccessibilityAdaptor::HandleActionPreviousEvent(bool allowEndFeedback)
{
  return Internal::Adaptor::AccessibilityAdaptor::GetImplementation(*this).HandleActionPreviousEvent(allowEndFeedback);
}

bool AccessibilityAdaptor::HandleActionActivateEvent()
{
  return Internal::Adaptor::AccessibilityAdaptor::GetImplementation(*this).HandleActionActivateEvent();
}

bool AccessibilityAdaptor::HandleActionReadEvent(unsigned int x, unsigned int y,  bool allowReadAgain)
{
  return Internal::Adaptor::AccessibilityAdaptor::GetImplementation(*this).HandleActionReadEvent(x, y, allowReadAgain);
}

bool AccessibilityAdaptor::HandleActionReadNextEvent(bool allowEndFeedback)
{
  return Internal::Adaptor::AccessibilityAdaptor::GetImplementation(*this).HandleActionReadNextEvent(allowEndFeedback);
}

bool AccessibilityAdaptor::HandleActionReadPreviousEvent(bool allowEndFeedback)
{
  return Internal::Adaptor::AccessibilityAdaptor::GetImplementation(*this).HandleActionReadPreviousEvent(allowEndFeedback);
}

bool AccessibilityAdaptor::HandleActionUpEvent()
{
  return Internal::Adaptor::AccessibilityAdaptor::GetImplementation(*this).HandleActionUpEvent();
}

bool AccessibilityAdaptor::HandleActionDownEvent()
{
  return Internal::Adaptor::AccessibilityAdaptor::GetImplementation(*this).HandleActionDownEvent();
}

bool AccessibilityAdaptor::HandleActionClearFocusEvent()
{
  return Internal::Adaptor::AccessibilityAdaptor::GetImplementation(*this).HandleActionClearFocusEvent();
}

bool AccessibilityAdaptor::HandleActionScrollEvent(const TouchPoint& point, unsigned long timeStamp)
{
  return Internal::Adaptor::AccessibilityAdaptor::GetImplementation(*this).HandleActionScrollEvent(point, timeStamp);
}

bool AccessibilityAdaptor::HandleActionTouchEvent(const TouchPoint& point, unsigned long timeStamp)
{
  return Internal::Adaptor::AccessibilityAdaptor::GetImplementation(*this).HandleActionTouchEvent(point, timeStamp);
}

bool AccessibilityAdaptor::HandleActionBackEvent()
{
  return Internal::Adaptor::AccessibilityAdaptor::GetImplementation(*this).HandleActionBackEvent();
}

void AccessibilityAdaptor::HandleActionEnableEvent()
{
  Internal::Adaptor::AccessibilityAdaptor::GetImplementation(*this).HandleActionEnableEvent();
}

void AccessibilityAdaptor::HandleActionDisableEvent()
{
  Internal::Adaptor::AccessibilityAdaptor::GetImplementation(*this).HandleActionDisableEvent();
}

bool AccessibilityAdaptor::HandleActionScrollUpEvent()
{
  return Internal::Adaptor::AccessibilityAdaptor::GetImplementation(*this).HandleActionScrollUpEvent();
}

bool AccessibilityAdaptor::HandleActionScrollDownEvent()
{
  return Internal::Adaptor::AccessibilityAdaptor::GetImplementation(*this).HandleActionScrollDownEvent();
}

bool AccessibilityAdaptor::HandleActionPageLeftEvent()
{
  return Internal::Adaptor::AccessibilityAdaptor::GetImplementation(*this).HandleActionPageLeftEvent();
}

bool AccessibilityAdaptor::HandleActionPageRightEvent()
{
  return Internal::Adaptor::AccessibilityAdaptor::GetImplementation(*this).HandleActionPageRightEvent();
}

bool AccessibilityAdaptor::HandleActionPageUpEvent()
{
  return Internal::Adaptor::AccessibilityAdaptor::GetImplementation(*this).HandleActionPageUpEvent();
}

bool AccessibilityAdaptor::HandleActionPageDownEvent()
{
  return Internal::Adaptor::AccessibilityAdaptor::GetImplementation(*this).HandleActionPageDownEvent();
}

bool AccessibilityAdaptor::HandleActionMoveToFirstEvent()
{
  return Internal::Adaptor::AccessibilityAdaptor::GetImplementation(*this).HandleActionMoveToFirstEvent();
}

bool AccessibilityAdaptor::HandleActionMoveToLastEvent()
{
  return Internal::Adaptor::AccessibilityAdaptor::GetImplementation(*this).HandleActionMoveToLastEvent();
}

bool AccessibilityAdaptor::HandleActionReadFromTopEvent()
{
  return Internal::Adaptor::AccessibilityAdaptor::GetImplementation(*this).HandleActionReadFromTopEvent();
}

bool AccessibilityAdaptor::HandleActionReadFromNextEvent()
{
  return Internal::Adaptor::AccessibilityAdaptor::GetImplementation(*this).HandleActionReadFromNextEvent();
}

bool AccessibilityAdaptor::HandleActionZoomEvent()
{
  return Internal::Adaptor::AccessibilityAdaptor::GetImplementation(*this).HandleActionZoomEvent();
}

bool AccessibilityAdaptor::HandleActionReadIndicatorInformationEvent()
{
  return Internal::Adaptor::AccessibilityAdaptor::GetImplementation(*this).HandleActionReadIndicatorInformationEvent();
}

bool AccessibilityAdaptor::HandleActionReadPauseResumeEvent()
{
  return Internal::Adaptor::AccessibilityAdaptor::GetImplementation(*this).HandleActionReadPauseResumeEvent();
}

bool AccessibilityAdaptor::HandleActionStartStopEvent()
{
  return Internal::Adaptor::AccessibilityAdaptor::GetImplementation(*this).HandleActionStartStopEvent();
}

AccessibilityAdaptor::AccessibilityAdaptor( Internal::Adaptor::AccessibilityAdaptor& manager )
: BaseHandle( &manager )
{
}

AccessibilityAdaptor::AccessibilityAdaptor( Internal::Adaptor::AccessibilityAdaptor* manager )
: BaseHandle( manager )
{
}

} // namespace Dali
