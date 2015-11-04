#ifndef __DALI_INTERNAL_FEEDBACK_PLAYER_H__
#define __DALI_INTERNAL_FEEDBACK_PLAYER_H__

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

// INTERNAL INCLUDES
#include <feedback-player.h>
#include <feedback/feedback-plugin-proxy.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

class FeedbackPluginProxy;

/**
 * Plays haptic effects.
 */
class FeedbackPlayer : public Dali::BaseObject
{

public:

  /**
   * Create a FeedbackPlayer.
   * This should only be called once by the Adaptor class.
   * @return A newly created FeedbackPlayer.
   */
  static Dali::FeedbackPlayer New();

  /**
   * Retrieve a handle to the FeedbackPlayer. This creates an instance if none has been created.
   * @return A handle to the FeedbackPlayer.
   */
  static Dali::FeedbackPlayer Get();

  /**
   * @copydoc Dali::FeedbackPlayer::PlayMonotone()
   */
  void PlayMonotone(unsigned int duration);

  /**
   * @copydoc Dali::FeedbackPlayer::PlayFile()
   */
  void PlayFile( const std::string& filePath );

  /**
   * @copydoc Dali::FeedbackPlayer::Stop()
   */
  void Stop();

  /**
   * @copydoc Dali::FeedbackPlayer::PlaySound()
   */
  int PlaySound( const std::string& fileName );

  /**
   * @copydoc Dali::FeedbackPlayer::StopSound()
   */
  void StopSound( int handle );

  /**
   * @copydoc Dali::FeedbackPlayer::PlayFeedbackPattern()
   */
  void PlayFeedbackPattern( int type, int pattern );

  /**
   * @copydoc Dali::FeedbackPlayer::LoadFile()
   */
  bool LoadFile(const std::string& filename, std::string& data);

private:

  /**
   * Private Constructor; see also FeedbackPlayer::New()
   */
  FeedbackPlayer();

  /**
   * Virtual Destructor
   */
  virtual ~FeedbackPlayer();

  // Undefined
  FeedbackPlayer(const FeedbackPlayer&);

  // Undefined
  FeedbackPlayer& operator=(FeedbackPlayer&);

private:

  FeedbackPluginProxy mPlugin;
};

} // namespace Adaptor

} // namespace Internal

// Helpers for public-api forwarding methods

inline Internal::Adaptor::FeedbackPlayer& GetImplementation(Dali::FeedbackPlayer& player)
{
  DALI_ASSERT_ALWAYS( player && "FeedbackPlayer handle is empty" );

  BaseObject& handle = player.GetBaseObject();

  return static_cast<Internal::Adaptor::FeedbackPlayer&>(handle);
}

inline const Internal::Adaptor::FeedbackPlayer& GetImplementation(const Dali::FeedbackPlayer& player)
{
  DALI_ASSERT_ALWAYS( player && "FeedbackPlayer handle is empty" );

  const BaseObject& handle = player.GetBaseObject();

  return static_cast<const Internal::Adaptor::FeedbackPlayer&>(handle);
}

} // namespace Dali

#endif // __DALI_INTERNAL_FEEDBACK_PLAYER_H__
