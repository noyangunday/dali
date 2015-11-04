#ifndef __DALI_INTERNAL_SOUND_PLAYER_H__
#define __DALI_INTERNAL_SOUND_PLAYER_H__

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
#include <sound-player.h>
#include <feedback/feedback-plugin-proxy.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

/**
 * Plays haptic effects.
 */
class SoundPlayer : public Dali::BaseObject
{

public:

  typedef Dali::SoundPlayer::SoundPlayFinishedSignalType SoundPlayFinishedSignalType;

  /**
   * Create a SoundPlayer.
   * @return A newly created SoundPlayer.
   */
  static Dali::SoundPlayer New();

  /**
   * Retrieve a handle to the SoundPlayer. This creates an instance if none has been created.
   * @return A handle to the SoundPlayer.
   */
  static Dali::SoundPlayer Get();

  /**
   * @copydoc Dali::SoundPlayer::PlaySound()
   */
  int PlaySound(const std::string fileName);

  /**
   * @copydoc Dali::SoundPlayer::Stop()
   */
  void Stop(int handle);

  /**
   * @copydoc Dali::SoundPlayer::SoundPlayFinishedSignal()
   */
  SoundPlayFinishedSignalType& SoundPlayFinishedSignal();

  /**
   * Connects a callback function with the object's signals.
   * @param[in] object The object providing the signal.
   * @param[in] tracker Used to disconnect the signal.
   * @param[in] signalName The signal to connect to.
   * @param[in] functor A newly allocated FunctorDelegate.
   * @return True if the signal was connected.
   * @post If a signal was connected, ownership of functor was passed to CallbackBase. Otherwise the caller is responsible for deleting the unused functor.
   */
  static bool DoConnectSignal( BaseObject* object, ConnectionTrackerInterface* tracker, const std::string& signalName, FunctorDelegate* functor );

private:

  /**
   * Private Constructor; see also soundPlayer::New()
   * @param[in]  soundPlayer  The public sound player class
   */
  SoundPlayer();

  /**
   * Destructor
   */
  virtual ~SoundPlayer();

  /**
   * Emits the SoundPlayFinished signal.
   */
  void EmitSoundPlayFinishedSignal();

  // Undefined
  SoundPlayer(const SoundPlayer&);

  // Undefined
  SoundPlayer& operator=(SoundPlayer&);

private:

  FeedbackPluginProxy mPlugin;
  SoundPlayFinishedSignalType mSoundPlayFinishedSignal;
};

} // namespace Adaptor

} // namespace Internal


// Helpers for public-api forwarding methods

inline Internal::Adaptor::SoundPlayer& GetImplementation(Dali::SoundPlayer& player)
{
  DALI_ASSERT_ALWAYS( player && "SoundPlayer handle is empty" );

  BaseObject& handle = player.GetBaseObject();

  return static_cast<Internal::Adaptor::SoundPlayer&>(handle);
}

inline const Internal::Adaptor::SoundPlayer& GetImplementation(const Dali::SoundPlayer& player)
{
  DALI_ASSERT_ALWAYS( player && "SoundPlayer handle is empty" );

  const BaseObject& handle = player.GetBaseObject();

  return static_cast<const Internal::Adaptor::SoundPlayer&>(handle);
}

} // namespace Dali

#endif // __DALI_INTERNAL_SOUND_PLAYER_H__
