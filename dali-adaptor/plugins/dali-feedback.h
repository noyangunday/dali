#ifndef __FEEDBACK_PLUGIN_H__
#define __FEEDBACK_PLUGIN_H__

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
#if defined(DALI_PROFILE_MOBILE)
#include <dd-haptic.h>
#else
#include <haptic.h>
#endif

// INTERNAL INCLUDES
#include <feedback-plugin.h>

namespace Dali
{

namespace Plugin
{

/**
 * Plays feedback effects for Dali-Toolkit UI Controls.
 */
class DaliFeedback : public Dali::FeedbackPlugin
{

public: // Construction & Destruction

  /**
   * Constructor
   */
  DaliFeedback();

  /**
   * Destructor
   */
  virtual ~DaliFeedback();

public: // FeedbackPlugin overrides

  /**
   * @copydoc Dali::Integration::FeedbackPlugin::PlayHaptic()
   */
  void PlayHaptic( const std::string& filePath );

  /**
   * @copydoc Dali::FeedbackPlugin::PlayHapticMonotone()
   */
  void PlayHapticMonotone( unsigned int duration );

  /**
   * @copydoc Dali::FeedbackPlugin::StopHaptic()
   */
  void StopHaptic();

  /**
   * @copydoc Dali::FeedbackPlugin::PlaySound()
   */
  int PlaySound( const std::string& fileName );

  /**
   * @copydoc Dali::FeedbackPlugin::StopSound()
   */
  void StopSound( int handle );

  /**
   * @copydoc Dali::FeedbackPlugin::PlayFeedbackPattern()
   */
  void PlayFeedbackPattern( int type, int pattern );

private:

  haptic_device_h mDeviceHandle;

  bool mHapticInitialized;
};

}  // namespace Plugin

}  // namespace Dali

#endif // __FEEDBACK_PLUGIN_H__
