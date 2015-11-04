#ifndef __DALI_FEEDBACK_PLAYER_H__
#define __DALI_FEEDBACK_PLAYER_H__

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
#include <dali/public-api/object/base-handle.h>

namespace Dali
{

namespace Internal DALI_INTERNAL
{
namespace Adaptor
{
class FeedbackPlayer;
}
}

/**
 * @brief Plays feedback effects.
 */
class DALI_IMPORT_API FeedbackPlayer : public BaseHandle
{
public:

  /**
   * @brief Create an uninitialized handle.
   *
   * This can be initialized by calling FeedbackPlayer::Get().
   */
  FeedbackPlayer();

  /**
   * @brief Create an initialized handle to the FeedbackPlayer.
   *
   * @return A handle to a newly allocated Dali resource.
   */
  static FeedbackPlayer Get();

  /**
   * @brief Destructor
   *
   * This is non-virtual since derived Handle types must not contain data or virtual methods.
   */
  ~FeedbackPlayer();

  /**
   * @brief Plays a monotone vibration.
   * @param[in]  duration  The duration of the vibration.
   */
  void PlayMonotone(unsigned int duration);

  /**
   * @brief Plays vibration in predefined patterns.
   * @param[in] filePath Path to the file containing the effect.
   */
  void PlayFile(const std::string filePath);

  /**
   * @brief Stops the currently playing vibration effects.
   */
  void Stop();

  /**
   * Plays a sound file.
   * @param[in] fileName Path to the sound file to play.
   * @return A handle which can be used to stop the sound playback.
   */
  int PlaySound( const std::string& fileName );

  /**
   * Stops a currently playing sound.
   * @param[in] handle A handle to the currently playing sound.
   */
  void StopSound( int handle );

  /**
   * Plays a feedback pattern.
   * @param[in] type The type of feedback.
   * @param[in] pattern The ID of the pattern to play.
   */
  void PlayFeedbackPattern( int type, int pattern );

  /*
   * Loads a file into data
   * @param[in] filename The filename.
   * @param[in] data The data in the file.
   * @return True if the file data could be loaded
   */
  bool LoadFile(const std::string& filename, std::string& data);

public: // Not intended for application developers

  /**
   * @brief This constructor is used by FeedbackPlayer::Get().
   * @param[in] feedbackPlayer A pointer to the feedback player.
   */
  explicit DALI_INTERNAL FeedbackPlayer( Internal::Adaptor::FeedbackPlayer* feedbackPlayer );
};

} // namespace Dali

#endif // __DALI_FEEDBACK_PLAYER_H__
