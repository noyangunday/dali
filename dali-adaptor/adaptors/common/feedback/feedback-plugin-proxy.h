#ifndef __DALI_INTERNAL_FEEDBACK_PLUGIN_PROXY_H__
#define __DALI_INTERNAL_FEEDBACK_PLUGIN_PROXY_H__

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

// INTERNAL INCLUDES
#include <feedback-plugin.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

typedef Dali::FeedbackPlugin::SoundStopCallBack SoundStopCallBack;
typedef Dali::FeedbackPlugin::CreateFeedbackPlugin CreateFeedbackPlugin;

/**
 * Proxy class to dynamically load, use and unload feedback plugin.
 */
class FeedbackPluginProxy
{
public:

  /**
   * The default feedback plugin proxy.
   */
  static const char * const DEFAULT_OBJECT_NAME;

public:

  /**
   * Constructor.
   */
  FeedbackPluginProxy( const std::string& sharedObjectName );

  /**
   * The destructor
   */
  ~FeedbackPluginProxy();

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

  /**
   * Dynamically loads the feedback plugin.
   */
  void Initialize();

private:

  bool mInitializeAttempted;
  void* mLibHandle;
  std::string mSharedObjectName;
  CreateFeedbackPlugin* mCreatePluginFunctionPtr;
  Dali::FeedbackPlugin* mFeedbackPlugin;
};

} // namespace Adaptor

} // namespace Internal

} // namespace Dali

#endif // __DALI_INTERNAL_FEEDBACK_PLUGIN_PROXY_H__
