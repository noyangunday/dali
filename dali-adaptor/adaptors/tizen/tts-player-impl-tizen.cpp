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

// CLASS HEADER
#include "tts-player-impl.h"

// EXTERNAL INCLUDES
#include <tts.h>
#include <stdio.h>

#include <dali/public-api/object/type-registry.h>


namespace Dali
{

namespace Internal
{

namespace Adaptor
{

namespace // unnamed namespace
{
// Type Registration
Dali::BaseHandle Create()
{
  return Dali::TtsPlayer::Get() ;
}

Dali::TypeRegistration mType( typeid(Dali::TtsPlayer), typeid(Dali::BaseHandle), Create );

/**
 * Helper function to convert Tizen-specific TTS state to external state.
 * @param state The Tizen TTS state.
 * @return The external TTS state.
 */
Dali::TtsPlayer::State InternalToExternalState( tts_state_e state )
{
  switch( state )
  {
    case TTS_STATE_CREATED:
    {
      return Dali::TtsPlayer::UNAVAILABLE;
    }
    case TTS_STATE_READY:
    {
      return Dali::TtsPlayer::READY;
    }
    case TTS_STATE_PLAYING:
    {
      return Dali::TtsPlayer::PLAYING;
    }
    case TTS_STATE_PAUSED:
    {
      return Dali::TtsPlayer::PAUSED;
    }
  }

  return Dali::TtsPlayer::UNAVAILABLE;
}

} // unnamed namespace

#if defined(DEBUG_ENABLED)
Debug::Filter* TtsPlayer::gLogFilter = Debug::Filter::New(Debug::Concise, false, "LOG_TTS_PLAYER");
#endif

Dali::TtsPlayer TtsPlayer::New(Dali::TtsPlayer::Mode mode)
{
  Dali::TtsPlayer player = Dali::TtsPlayer(new TtsPlayer(mode));

  return player;
}

TtsPlayer::TtsPlayer(Dali::TtsPlayer::Mode mode)
: mInitialized(false),
  mUnplayedString(""),
  mUtteranceId(0),
  mTtsMode(mode)
{
  Initialize();
}

TtsPlayer::~TtsPlayer()
{
  // If it is playing, stop it
  Stop();

  // Unset the callback funtion for TTS state change
  int retVal = tts_unset_state_changed_cb(mTtsHandle);
  if( retVal != TTS_ERROR_NONE )
  {
    LogErrorCode(static_cast<tts_error_e>(retVal));
  }

  // Destroy the TTS handle and disconnects the daemon
  retVal = tts_destroy(mTtsHandle);
  if( retVal != TTS_ERROR_NONE )
  {
    LogErrorCode(static_cast<tts_error_e>(retVal));
  }
}

void TtsPlayer::Initialize()
{
  // Create the TTS handle
  int retVal = tts_create(&mTtsHandle);

  if( retVal != TTS_ERROR_NONE )
  {
    LogErrorCode(static_cast<tts_error_e>(retVal));
  }
  else
  {
    // Set the callback funtion for TTS state change
    retVal = tts_set_state_changed_cb(mTtsHandle, &StateChangedCallback, this);
    if( retVal != TTS_ERROR_NONE )
    {
      LogErrorCode(static_cast<tts_error_e>(retVal));
    }

    // Check tts mode
    tts_mode_e ttsMode = TTS_MODE_DEFAULT;
    switch (mTtsMode)
    {
      case Dali::TtsPlayer::DEFAULT:
        ttsMode = TTS_MODE_DEFAULT;
      break;
      case Dali::TtsPlayer::NOTIFICATION:
        ttsMode = TTS_MODE_NOTIFICATION;
      break;
      case Dali::TtsPlayer::SCREEN_READER:
        ttsMode = TTS_MODE_SCREEN_READER;
      break;
      default:
      break;
    }

    // Set mode
    retVal = tts_set_mode(mTtsHandle, ttsMode);
    if(retVal != TTS_ERROR_NONE)
    {
      LogErrorCode(static_cast<tts_error_e>(retVal));
    }

    // Connect the TTS daemon asynchronously
    retVal = tts_prepare(mTtsHandle);
    if(retVal != TTS_ERROR_NONE)
    {
      LogErrorCode(static_cast<tts_error_e>(retVal));
    }
  }
}

void TtsPlayer::Play(const std::string& text)
{
  if(mInitialized)
  {
    Stop();

    // Add text to the queue, and use normal speed, default language and default voice set by the user
    int retVal = tts_add_text(mTtsHandle, text.c_str(), NULL, TTS_VOICE_TYPE_AUTO, TTS_SPEED_AUTO, &mUtteranceId);
    if(retVal != TTS_ERROR_NONE)
    {
      LogErrorCode(static_cast<tts_error_e>(retVal));
    }
    else
    {
      // Start synthesizing voice from text in the queue and play synthesized audio data
      retVal = tts_play(mTtsHandle);
      if(retVal != TTS_ERROR_NONE)
      {
        LogErrorCode(static_cast<tts_error_e>(retVal));
      }
    }
  }
  else
  {
    mUnplayedString = text;
  }
}

void TtsPlayer::Stop()
{
  if(mInitialized)
  {
    // Check the current TTS state
    tts_state_e state;
    int retVal = tts_get_state(mTtsHandle, &state);
    if(retVal != TTS_ERROR_NONE)
    {
      LogErrorCode(static_cast<tts_error_e>(retVal));
    }
    else if(state == TTS_STATE_PLAYING || state == TTS_STATE_PAUSED)
    {
      // If it is playing or paused, stop playing and clear the queue
      retVal = tts_stop(mTtsHandle);
      if( retVal != TTS_ERROR_NONE )
      {
        LogErrorCode(static_cast<tts_error_e>(retVal));
      }
    }
  }
}

void TtsPlayer::Pause()
{
  if(mInitialized)
  {
    // Check the current TTS state
    tts_state_e state;
    int retVal = tts_get_state(mTtsHandle, &state);
    if(retVal != TTS_ERROR_NONE)
    {
      LogErrorCode(static_cast<tts_error_e>(retVal));
    }
    else if(state == TTS_STATE_PLAYING)
    {
      // If the player is playing, pause it.
      retVal = tts_pause(mTtsHandle);
      if( retVal != TTS_ERROR_NONE )
      {
        LogErrorCode(static_cast<tts_error_e>(retVal));
      }
    }
  }
}

void TtsPlayer::Resume()
{
  if(mInitialized)
  {
    // Check the current TTS state
    tts_state_e state;
    int retVal = tts_get_state(mTtsHandle, &state);
    if(retVal != TTS_ERROR_NONE)
    {
      LogErrorCode(static_cast<tts_error_e>(retVal));
    }
    else if(state == TTS_STATE_PAUSED)
    {
      // If the player is paused, resume it.
      retVal = tts_play(mTtsHandle);
      if( retVal != TTS_ERROR_NONE )
      {
        LogErrorCode(static_cast<tts_error_e>(retVal));
      }
    }
  }
}

Dali::TtsPlayer::State TtsPlayer::GetState()
{
  Dali::TtsPlayer::State ttsState = Dali::TtsPlayer::UNAVAILABLE;

  if(mInitialized)
  {
    // Check the current TTS state
    tts_state_e state;
    int retVal = tts_get_state(mTtsHandle, &state);
    if(retVal != TTS_ERROR_NONE)
    {
      LogErrorCode(static_cast<tts_error_e>(retVal));
    }
    else
    {
      ttsState = InternalToExternalState( state );
    }
  }

  return ttsState;
}

Dali::TtsPlayer::StateChangedSignalType& TtsPlayer::StateChangedSignal()
{
  return mStateChangedSignal;
}

void TtsPlayer::EmitStateChangedSignal( tts_state_e previous, tts_state_e current )
{
  // Convert the previous and current states to external states and emit them as a signal.
  if( !mStateChangedSignal.Empty() )
  {
    mStateChangedSignal.Emit( InternalToExternalState( previous ), InternalToExternalState( current ) );
  }
}

void TtsPlayer::StateChangedCallback(tts_h tts, tts_state_e previous, tts_state_e current, void *userData)
{
  // Get the implementation (this is a static function).
  TtsPlayer* obj = static_cast<TtsPlayer*>(userData);

  // Emit the signal.
  obj->EmitStateChangedSignal( previous, current );

  if(!obj->mInitialized && current == TTS_STATE_READY)
  {
    obj->mInitialized = true;

    // if there is queued text before initialization, play it
    if(obj->mUnplayedString != "")
    {
      obj->Play(obj->mUnplayedString);
      obj->mUnplayedString = "";
    }
  }
}

void TtsPlayer::LogErrorCode(tts_error_e reason)
{
  std::string error_string;

  switch (reason)
  {
    case TTS_ERROR_NONE:
    {
      break;
    }
    case TTS_ERROR_OUT_OF_MEMORY:
    {
      error_string = "TTS: Out of Memory\n";
      break;
    }
    case TTS_ERROR_IO_ERROR:
    {
      error_string = "TTS: I/O error\n";
      break;
    }
    case TTS_ERROR_INVALID_PARAMETER:
    {
      error_string = "TTS: Invalid parameter\n";
      break;
    }
    case TTS_ERROR_OUT_OF_NETWORK:
    {
      error_string = "TTS: Out of network\n";
      break;
    }
    case TTS_ERROR_INVALID_STATE:
    {
      error_string = "TTS: Invalid state\n";
      break;
    }
    case TTS_ERROR_INVALID_VOICE:
    {
      error_string = "TTS: Invalid voice\n";
      break;
    }
    case TTS_ERROR_ENGINE_NOT_FOUND:
    {
      error_string = "TTS: No available engine\n";
      break;
    }
    case TTS_ERROR_TIMED_OUT:
    {
      error_string = "TTS: No answer from the daemon\n";
      break;
    }
    case TTS_ERROR_OPERATION_FAILED:
    {
      error_string = "TTS: Operation failed\n";
      break;
    }
    default:
    {
      error_string = "Invalid TTS error code\n";
      break;
    }
  }

  if(reason != TTS_ERROR_NONE)
  {
    DALI_LOG_WARNING("[%s:%d] tts error : %s\n", __FUNCTION__, __LINE__, error_string.c_str());
  }
}

} // namespace Adaptor

} // namespace Internal

} // namespace Dali
