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

Dali::TypeRegistration mType( typeid(Dali::TtsPlayer), typeid(Dali::BaseHandle), Create ) ;
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
{
  DALI_LOG_ERROR("TTS is not implemented in UBUNTU profile.");
}

TtsPlayer::~TtsPlayer()
{
}

void TtsPlayer::Play(const std::string& text)
{
}

void TtsPlayer::Stop()
{
}

void TtsPlayer::Pause()
{
}

void TtsPlayer::Resume()
{
}

Dali::TtsPlayer::State TtsPlayer::GetState()
{
  return Dali::TtsPlayer::UNAVAILABLE;
}

Dali::TtsPlayer::StateChangedSignalType& TtsPlayer::StateChangedSignal()
{
  return mStateChangedSignal;
}

} // namespace Adaptor

} // namespace Internal

} // namespace Dali

