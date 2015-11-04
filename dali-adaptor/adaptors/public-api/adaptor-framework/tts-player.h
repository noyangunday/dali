#ifndef __DALI_TTS_PLAYER_H__
#define __DALI_TTS_PLAYER_H__

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
#include <dali/public-api/signals/dali-signal.h>

namespace Dali
{
/**
 * @addtogroup dali_adaptor_framework
 * @{
 */

namespace Internal DALI_INTERNAL
{
namespace Adaptor
{
class TtsPlayer;
}
}

/**
 * @brief The Text-to-speech Player.
 */
class DALI_IMPORT_API TtsPlayer : public BaseHandle
{
public: // ENUMs

  /**
   * @brief Enumeration of TTS mode.
   */
  enum Mode
  {
    DEFAULT = 0,  ///< Default mode for normal application
    NOTIFICATION, ///< Notification mode
    SCREEN_READER, ///< Screen reader mode
    MODE_NUM
  };

  /**
   * @brief Enumeration of TTS state.
   */
  enum State
  {
    UNAVAILABLE = 0,    ///< Player is not available
    READY,              ///< Player is ready to play
    PLAYING,            ///< Player is playing
    PAUSED              ///< Player is paused
  };

public: // Typedefs

  /**
   * @brief Type of signal emitted when the TTS state changes.
   */
  typedef Signal< void ( const Dali::TtsPlayer::State, const Dali::TtsPlayer::State ) > StateChangedSignalType;

public: // API

  /**
   * @brief Create an uninitialized handle.
   *
   * This can be initialized by calling TtsPlayer::Get().
   */
  TtsPlayer();

  /**
   * @brief Gets the singleton of the TtsPlayer for the given mode.
   *
   * @param mode the mode of tts-player
   * @return A handle of the Ttsplayer for the given mode.
   */
  static TtsPlayer Get(Dali::TtsPlayer::Mode mode = Dali::TtsPlayer::DEFAULT);

  /**
   * @brief Destructor
   *
   * This is non-virtual since derived Handle types must not contain data or virtual methods.
   */
  ~TtsPlayer();

  /**
   * @brief This copy constructor is required for (smart) pointer semantics.
   *
   * @param [in] handle A reference to the copied handle
   */
  TtsPlayer(const TtsPlayer& handle);

  /**
   * @brief This assignment operator is required for (smart) pointer semantics.
   *
   * @param [in] rhs  A reference to the copied handle
   * @return A reference to this
   */
  TtsPlayer& operator=(const TtsPlayer& rhs);

  /**
   * @brief Start playing the audio data synthesized from the specified text.
   *
   * @pre The TtsPlayer needs to be initialized.
   * @param[in] text to play.
   */
  void Play(const std::string& text);

  /**
   * @brief Stops playing the utterance.
   * @pre The TtsPlayer needs to be initialized.
   */
  void Stop();

  /**
   * @brief Pauses the currently playing utterance.
   * @pre The TtsPlayer needs to be initialized.
   */
  void Pause();

  /**
   * @brief Resumes the previously paused utterance.
   * @pre The TtsPlayer needs to be initialized.
   */
  void Resume();

  /**
   * @brief Gets the current state of the player.
   * @pre The TtsPlayer needs to be initialized.
   */
  State GetState();

  /**
   * @brief Allows connection TTS state change signal.
   * Note: Only supported by some adaptor types.
   * @return A reference to the signal for connection.
   */
  Dali::TtsPlayer::StateChangedSignalType& StateChangedSignal();

public: // Not intended for application developers

  /**
   * @brief This constructor is used by TtsPlayer::Get().
   * @param[in] ttsPlayer A pointer to the TTS player.
   */
  explicit DALI_INTERNAL TtsPlayer( Internal::Adaptor::TtsPlayer* ttsPlayer );
};

/**
 * @}
 */
} // namespace Dali

#endif // __DALI_TTS_PLAYER_H__
