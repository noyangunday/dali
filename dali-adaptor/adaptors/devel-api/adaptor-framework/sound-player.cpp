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
#include <sound-player.h>

// INTERNAL INCLUDES
#include <sound-player-impl.h>

namespace Dali
{

SoundPlayer::SoundPlayer()
{
}

SoundPlayer SoundPlayer::Get()
{
  return Internal::Adaptor::SoundPlayer::Get();
}

SoundPlayer::~SoundPlayer()
{
}

int SoundPlayer::PlaySound(const std::string fileName)
{
  return GetImplementation(*this).PlaySound(fileName);
}

void SoundPlayer::Stop(int handle)
{
  GetImplementation(*this).Stop(handle);
}

SoundPlayer::SoundPlayFinishedSignalType& SoundPlayer::SoundPlayFinishedSignal()
{
  return GetImplementation(*this).SoundPlayFinishedSignal();
}

SoundPlayer::SoundPlayer( Internal::Adaptor::SoundPlayer* player )
: BaseHandle( player )
{
}

} // namespace Dali
