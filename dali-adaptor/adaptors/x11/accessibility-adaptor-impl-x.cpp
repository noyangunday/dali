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
#include "accessibility-adaptor-impl.h"

// EXTERNAL INCLUDES
#include <vconf.h>
#include <Ecore_X.h>
#include <Elementary.h>

#include <dali/integration-api/debug.h>
#include <dali/integration-api/events/gesture-requests.h>

// INTERNAL INCLUDES
#include "system-settings.h"

#define MSG_DOMAIN_CONTROL_ACCESS (int)ECORE_X_ATOM_E_ILLUME_ACCESS_CONTROL

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

namespace {
#if defined(DEBUG_ENABLED)
Debug::Filter* gAccessibilityAdaptorLogFilter = Debug::Filter::New(Debug::NoLogging, false, "LOG_ACCESSIBILITY_ADAPTOR");
#endif
} // unnamed namespace

bool AccessibilityAdaptor::HandleActionNextEvent(bool allowEndFeedback)
{
  bool ret = false;

  if( mIndicator && mIndicatorFocused )
  {
    Elm_Access_Action_Info actionInfo;
    actionInfo.action_type = ELM_ACCESS_ACTION_HIGHLIGHT_NEXT;

    ret = mIndicator->SendMessage(MSG_DOMAIN_CONTROL_ACCESS, actionInfo.action_type, &actionInfo, sizeof(actionInfo));
  }
  else if( mActionHandler )
  {
    ret = mActionHandler->AccessibilityActionNext(allowEndFeedback);
  }

  DALI_LOG_INFO(gAccessibilityAdaptorLogFilter, Debug::General, "[%s:%d] %s\n", __FUNCTION__, __LINE__, ret?"TRUE":"FALSE");

  return ret;
}

bool AccessibilityAdaptor::HandleActionPreviousEvent(bool allowEndFeedback)
{
  bool ret = false;

  if( mIndicator && mIndicatorFocused )
  {
    Elm_Access_Action_Info actionInfo;
    actionInfo.action_type = ELM_ACCESS_ACTION_HIGHLIGHT_PREV;

    ret = mIndicator->SendMessage(MSG_DOMAIN_CONTROL_ACCESS, actionInfo.action_type, &actionInfo, sizeof(actionInfo));
  }
  else if( mActionHandler )
  {
    ret = mActionHandler->AccessibilityActionPrevious(allowEndFeedback);
  }

  DALI_LOG_INFO(gAccessibilityAdaptorLogFilter, Debug::General, "[%s:%d] %s\n", __FUNCTION__, __LINE__, ret?"TRUE":"FALSE");

  return ret;
}

bool AccessibilityAdaptor::HandleActionActivateEvent()
{
  bool ret = false;

  if( mIndicator && mIndicatorFocused )
  {
    Elm_Access_Action_Info actionInfo;
    actionInfo.action_type = ELM_ACCESS_ACTION_ACTIVATE;

    ret = mIndicator->SendMessage(MSG_DOMAIN_CONTROL_ACCESS, actionInfo.action_type, &actionInfo, sizeof(actionInfo));
  }
  else if( mActionHandler )
  {
    ret = mActionHandler->AccessibilityActionActivate();
  }

  DALI_LOG_INFO(gAccessibilityAdaptorLogFilter, Debug::General, "[%s:%d] %s\n", __FUNCTION__, __LINE__, ret?"TRUE":"FALSE");

  return ret;
}

bool AccessibilityAdaptor::HandleActionReadEvent(unsigned int x, unsigned int y, bool allowReadAgain)
{
  bool ret = false;

  DALI_LOG_INFO(gAccessibilityAdaptorLogFilter, Debug::General, "[%s:%d] %d , %d\n", __FUNCTION__, __LINE__, x, y);

  mReadPosition.x = x;
  mReadPosition.y = y;

  Dali::AccessibilityAdaptor handle( this );

  bool indicatorFocused = false;

  // Check whether the Indicator is focused
  if( mIndicator && mIndicator->IsConnected() )
  {
    // Check the position and size of Indicator actor
    Dali::Actor indicatorActor = mIndicator->GetActor();
    Vector3 position = Vector3(0.0f, 0.0f, 0.0f);
    Vector3 size = indicatorActor.GetCurrentSize();

    if(mReadPosition.x >= position.x &&
       mReadPosition.x <= position.x + size.width &&
       mReadPosition.y >= position.y &&
       mReadPosition.y <= position.y + size.height)
    {
      indicatorFocused = true;
      DALI_LOG_INFO(gAccessibilityAdaptorLogFilter, Debug::General, "[%s:%d] Indicator area!!!!\n", __FUNCTION__, __LINE__);
    }
  }

  if( mIndicator )
  {
    if( !mIndicatorFocused && indicatorFocused )
    {
      // If Indicator is focused, the focus should be cleared in Dali focus chain.
      if( mActionHandler )
      {
        mActionHandler->ClearAccessibilityFocus();
      }
    }
    else if( mIndicatorFocused && !indicatorFocused )
    {
      Elm_Access_Action_Info actionInfo;
      actionInfo.action_type = ELM_ACCESS_ACTION_UNHIGHLIGHT;

      // Indicator should be unhighlighted
      ret = mIndicator->SendMessage(MSG_DOMAIN_CONTROL_ACCESS, actionInfo.action_type, &actionInfo, sizeof(actionInfo));
      DALI_LOG_INFO(gAccessibilityAdaptorLogFilter, Debug::General, "[%s:%d] Send unhighlight message to indicator!!!!\n", __FUNCTION__, __LINE__);
    }

    mIndicatorFocused = indicatorFocused;

    // Send accessibility READ action information to Indicator
    if( mIndicatorFocused )
    {
      Elm_Access_Action_Info actionInfo;
      actionInfo.x = mReadPosition.x;
      actionInfo.y = mReadPosition.y;

      if(allowReadAgain)
      {
        actionInfo.action_type = ELM_ACCESS_ACTION_READ;
      }
      else
      {
        actionInfo.action_type = static_cast<Elm_Access_Action_Type>( GetElmAccessActionOver() );
      }

      ret = mIndicator->SendMessage(MSG_DOMAIN_CONTROL_ACCESS, actionInfo.action_type, &actionInfo, sizeof(actionInfo));

      DALI_LOG_INFO(gAccessibilityAdaptorLogFilter, Debug::General, "[%s:%d] Send READ message to indicator!!!!\n", __FUNCTION__, __LINE__);
    }
  }

  if( mActionHandler && !mIndicatorFocused)
  {
    // If Indicator is not focused, the accessibility actions should be handled by the registered
    // accessibility action handler (e.g. focus manager)
    ret = mActionHandler->AccessibilityActionRead(allowReadAgain);
    DALI_LOG_INFO(gAccessibilityAdaptorLogFilter, Debug::General, "[%s:%d] %s\n", __FUNCTION__, __LINE__, ret?"TRUE":"FALSE");
  }

  return ret;
}

bool AccessibilityAdaptor::HandleActionReadNextEvent(bool allowEndFeedback)
{
  bool ret = false;

  if( mIndicator && mIndicatorFocused )
  {
    Elm_Access_Action_Info actionInfo;
    actionInfo.action_type = ELM_ACCESS_ACTION_HIGHLIGHT_NEXT;

    ret = mIndicator->SendMessage(MSG_DOMAIN_CONTROL_ACCESS, actionInfo.action_type, &actionInfo, sizeof(actionInfo));
  }
  else if( mActionHandler )
  {
    ret = mActionHandler->AccessibilityActionReadNext(allowEndFeedback);
  }

  DALI_LOG_INFO(gAccessibilityAdaptorLogFilter, Debug::General, "[%s:%d] %s\n", __FUNCTION__, __LINE__, ret?"TRUE":"FALSE");

  return ret;
}

bool AccessibilityAdaptor::HandleActionReadPreviousEvent(bool allowEndFeedback)
{
  bool ret = false;

  if( mIndicator && mIndicatorFocused )
  {
    Elm_Access_Action_Info actionInfo;
    actionInfo.action_type = ELM_ACCESS_ACTION_HIGHLIGHT_PREV;

    ret = mIndicator->SendMessage(MSG_DOMAIN_CONTROL_ACCESS, actionInfo.action_type, &actionInfo, sizeof(actionInfo));
  }
  else if( mActionHandler )
  {
    ret = mActionHandler->AccessibilityActionReadPrevious(allowEndFeedback);
  }

  DALI_LOG_INFO(gAccessibilityAdaptorLogFilter, Debug::General, "[%s:%d] %s\n", __FUNCTION__, __LINE__, ret?"TRUE":"FALSE");

  return ret;
}

bool AccessibilityAdaptor::HandleActionUpEvent()
{
  bool ret = false;

  if( mIndicator && mIndicatorFocused )
  {
    Elm_Access_Action_Info actionInfo;
    actionInfo.action_type = ELM_ACCESS_ACTION_UP;

    ret = mIndicator->SendMessage(MSG_DOMAIN_CONTROL_ACCESS, actionInfo.action_type, &actionInfo, sizeof(actionInfo));
  }
  else if( mActionHandler )
  {
    ret = mActionHandler->AccessibilityActionUp();
  }

  DALI_LOG_INFO(gAccessibilityAdaptorLogFilter, Debug::General, "[%s:%d] %s\n", __FUNCTION__, __LINE__, ret?"TRUE":"FALSE");

  return ret;
}

bool AccessibilityAdaptor::HandleActionDownEvent()
{
  bool ret = false;

  if( mIndicator && mIndicatorFocused )
  {
    Elm_Access_Action_Info actionInfo;
    actionInfo.action_type = ELM_ACCESS_ACTION_DOWN;

    ret = mIndicator->SendMessage(MSG_DOMAIN_CONTROL_ACCESS, actionInfo.action_type, &actionInfo, sizeof(actionInfo));
  }
  else if( mActionHandler )
  {
    ret = mActionHandler->AccessibilityActionDown();
  }

  DALI_LOG_INFO(gAccessibilityAdaptorLogFilter, Debug::General, "[%s:%d] %s\n", __FUNCTION__, __LINE__, ret?"TRUE":"FALSE");

  return ret;
}

bool AccessibilityAdaptor::HandleActionScrollUpEvent()
{
  bool ret = false;

  if( mIndicator && mIndicatorFocused )
  {
    // TODO: Send message to indicator with the correct action type
  }
  else if( mActionHandler )
  {
    ret = mActionHandler->AccessibilityActionScrollUp();
  }

  DALI_LOG_INFO(gAccessibilityAdaptorLogFilter, Debug::General, "[%s:%d] %s\n", __FUNCTION__, __LINE__, ret?"TRUE":"FALSE");

  return ret;
}


bool AccessibilityAdaptor::HandleActionScrollDownEvent()
{
  bool ret = false;

  if( mIndicator && mIndicatorFocused )
  {
    // TODO: Send message to indicator with the correct action type
  }
  else if( mActionHandler )
  {
    ret = mActionHandler->AccessibilityActionScrollDown();
  }

  DALI_LOG_INFO(gAccessibilityAdaptorLogFilter, Debug::General, "[%s:%d] %s\n", __FUNCTION__, __LINE__, ret?"TRUE":"FALSE");

  return ret;
}

bool AccessibilityAdaptor::HandleActionPageLeftEvent()
{
  bool ret = false;

  if( mIndicator && mIndicatorFocused )
  {
    // TODO: Send message to indicator with the correct action type
  }
  else if( mActionHandler )
  {
    ret = mActionHandler->AccessibilityActionPageLeft();
  }

  DALI_LOG_INFO(gAccessibilityAdaptorLogFilter, Debug::General, "[%s:%d] %s\n", __FUNCTION__, __LINE__, ret?"TRUE":"FALSE");

  return ret;
}

bool AccessibilityAdaptor::HandleActionPageRightEvent()
{
  bool ret = false;

  if( mIndicator && mIndicatorFocused )
  {
    // TODO: Send message to indicator with the correct action type
  }
  else if( mActionHandler )
  {
    ret = mActionHandler->AccessibilityActionPageRight();
  }

  DALI_LOG_INFO(gAccessibilityAdaptorLogFilter, Debug::General, "[%s:%d] %s\n", __FUNCTION__, __LINE__, ret?"TRUE":"FALSE");

  return ret;
}

bool AccessibilityAdaptor::HandleActionPageUpEvent()
{
  bool ret = false;

  if( mIndicator && mIndicatorFocused )
  {
    // TODO: Send message to indicator with the correct action type
  }
  else if( mActionHandler )
  {
    ret = mActionHandler->AccessibilityActionPageUp();
  }

  DALI_LOG_INFO(gAccessibilityAdaptorLogFilter, Debug::General, "[%s:%d] %s\n", __FUNCTION__, __LINE__, ret?"TRUE":"FALSE");

  return ret;
}

bool AccessibilityAdaptor::HandleActionPageDownEvent()
{
  bool ret = false;

  if( mIndicator && mIndicatorFocused )
  {
    // TODO: Send message to indicator with the correct action type
  }
  else if( mActionHandler )
  {
    ret = mActionHandler->AccessibilityActionPageDown();
  }

  DALI_LOG_INFO(gAccessibilityAdaptorLogFilter, Debug::General, "[%s:%d] %s\n", __FUNCTION__, __LINE__, ret?"TRUE":"FALSE");

  return ret;
}

bool AccessibilityAdaptor::HandleActionMoveToFirstEvent()
{
  bool ret = false;

  if( mIndicator && mIndicatorFocused )
  {
    // TODO: Send message to indicator with the correct action type
  }
  else if( mActionHandler )
  {
    ret = mActionHandler->AccessibilityActionMoveToFirst();
  }

  DALI_LOG_INFO(gAccessibilityAdaptorLogFilter, Debug::General, "[%s:%d] %s\n", __FUNCTION__, __LINE__, ret?"TRUE":"FALSE");

  return ret;
}

bool AccessibilityAdaptor::HandleActionMoveToLastEvent()
{
  bool ret = false;

  if( mIndicator && mIndicatorFocused )
  {
    // TODO: Send message to indicator with the correct action type
  }
  else if( mActionHandler )
  {
    ret = mActionHandler->AccessibilityActionMoveToLast();
  }

  DALI_LOG_INFO(gAccessibilityAdaptorLogFilter, Debug::General, "[%s:%d] %s\n", __FUNCTION__, __LINE__, ret?"TRUE":"FALSE");

  return ret;
}

bool AccessibilityAdaptor::HandleActionReadFromTopEvent()
{
  bool ret = false;

  if( mIndicator && mIndicatorFocused )
  {
    // TODO: Send message to indicator with the correct action type
  }
  else if( mActionHandler )
  {
    ret = mActionHandler->AccessibilityActionReadFromTop();
  }

  DALI_LOG_INFO(gAccessibilityAdaptorLogFilter, Debug::General, "[%s:%d] %s\n", __FUNCTION__, __LINE__, ret?"TRUE":"FALSE");

  return ret;
}

bool AccessibilityAdaptor::HandleActionReadFromNextEvent()
{
  bool ret = false;

  if( mIndicator && mIndicatorFocused )
  {
    // TODO: Send message to indicator with the correct action type
  }
  else if( mActionHandler )
  {
    ret = mActionHandler->AccessibilityActionReadFromNext();
  }

  DALI_LOG_INFO(gAccessibilityAdaptorLogFilter, Debug::General, "[%s:%d] %s\n", __FUNCTION__, __LINE__, ret?"TRUE":"FALSE");

  return ret;
}

bool AccessibilityAdaptor::HandleActionZoomEvent()
{
  bool ret = false;

  if( mIndicator && mIndicatorFocused )
  {
    // TODO: Send message to indicator with the correct action type
  }
  else if( mActionHandler )
  {
    ret = mActionHandler->AccessibilityActionZoom();
  }

  DALI_LOG_INFO(gAccessibilityAdaptorLogFilter, Debug::General, "[%s:%d] %s\n", __FUNCTION__, __LINE__, ret?"TRUE":"FALSE");

  return ret;
}

bool AccessibilityAdaptor::HandleActionReadIndicatorInformationEvent()
{
  bool ret = false;

  if( mIndicator && mIndicatorFocused )
  {
    // TODO: Send message to indicator with the correct action type
  }
  else if( mActionHandler )
  {
    ret = mActionHandler->AccessibilityActionReadIndicatorInformation();
  }

  DALI_LOG_INFO(gAccessibilityAdaptorLogFilter, Debug::General, "[%s:%d] %s\n", __FUNCTION__, __LINE__, ret?"TRUE":"FALSE");

  return ret;
}

bool AccessibilityAdaptor::HandleActionReadPauseResumeEvent()
{
  bool ret = false;

  if( mIndicator && mIndicatorFocused )
  {
    // TODO: Send message to indicator with the correct action type
  }
  else if( mActionHandler )
  {
    ret = mActionHandler->AccessibilityActionReadPauseResume();
  }

  DALI_LOG_INFO(gAccessibilityAdaptorLogFilter, Debug::General, "[%s:%d] %s\n", __FUNCTION__, __LINE__, ret?"TRUE":"FALSE");

  return ret;
}

bool AccessibilityAdaptor::HandleActionStartStopEvent()
{
  bool ret = false;

  if( mIndicator && mIndicatorFocused )
  {
    // TODO: Send message to indicator with the correct action type
  }
  else if( mActionHandler )
  {
    ret = mActionHandler->AccessibilityActionStartStop();
  }

  DALI_LOG_INFO(gAccessibilityAdaptorLogFilter, Debug::General, "[%s:%d] %s\n", __FUNCTION__, __LINE__, ret?"TRUE":"FALSE");

  return ret;
}

} // namespace Adaptor

} // namespace Internal

} // namespace Dali
