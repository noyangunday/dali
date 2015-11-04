#ifndef __DALI_FEEDBACK_PLUGIN_H__
#define __DALI_FEEDBACK_PLUGIN_H__

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
#include <string>
#include <dali/public-api/common/dali-common.h>

namespace Dali
{

/**
 * FeedbackPlugin is an abstract interface, used by Dali-adaptor to access haptic and audio feedback.
 * A concrete implementation must be created for each platform and provided as a dynamic library which
 * will be loaded at run time by the adaptor.
 */
class FeedbackPlugin
{
public:

  typedef void (*SoundStopCallBack)( void* ptr );

  /**
   * Destructor.
   */
  virtual ~FeedbackPlugin()
  {
  }

  /**
   * Plays vibration in predefined patterns.
   * @param[in] filePath Path to the file containing the effect.
   */
  virtual void PlayHaptic( const std::string& filePath ) = 0;

  /**
   * Plays a monotone vibration.
   * @param[in]  duration  The duration of the vibration.
   */
  virtual void PlayHapticMonotone( unsigned int duration ) = 0;

  /**
   * Stops the currently playing vibration effects.
   */
  virtual void StopHaptic() = 0;

  /**
   * Plays a sound file.
   * @param[in] fileName Path to the sound file to play.
   * @return A handle which can be used to stop the sound playback.
   */
  virtual int PlaySound( const std::string& fileName ) = 0;

  /**
   * Stops a currently playing sound.
   * @param[in] handle A handle to the currently playing sound.
   */
  virtual void StopSound( int handle ) = 0;

  /**
   * Plays a feedback pattern.
   * @param[in] type The type of feedback.
   * @param[in] pattern The ID of the pattern to play.
   */
  virtual void PlayFeedbackPattern( int type, int pattern ) = 0;

  // Types for plugin factories

  /**
   * Function pointer called in adaptor to create a feedback plugin instance.
   * @param [in] pluginName name of the plugin to load.
   * @return Pointer to the newly created plugin object
   */
  typedef FeedbackPlugin* CreateFeedbackPlugin( void );

}; // class FeedbackPlugin

} // namespace Dali

#endif // __DALI_FEEDBACK_PLUGIN_H__
