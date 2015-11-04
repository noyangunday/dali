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
#include <feedback-player.h>

// INTERNAL INCLUDES
#include <feedback-player-impl.h>

namespace Dali
{

FeedbackPlayer::FeedbackPlayer()
{
}

FeedbackPlayer FeedbackPlayer::Get()
{
  return Internal::Adaptor::FeedbackPlayer::Get();
}

FeedbackPlayer::~FeedbackPlayer()
{
}

void FeedbackPlayer::PlayMonotone(unsigned int duration)
{
  GetImplementation(*this).PlayMonotone(duration);
}

void FeedbackPlayer::PlayFile(const std::string filePath)
{
  GetImplementation(*this).PlayFile(filePath);
}

void FeedbackPlayer::Stop()
{
  GetImplementation(*this).Stop();
}

int FeedbackPlayer::PlaySound( const std::string& fileName )
{
  return GetImplementation(*this).PlaySound(fileName);
}

void FeedbackPlayer::StopSound( int handle )
{
  GetImplementation(*this).StopSound(handle);
}

void FeedbackPlayer::PlayFeedbackPattern( int type, int pattern )
{
  GetImplementation(*this).PlayFeedbackPattern(type, pattern);
}

bool FeedbackPlayer::LoadFile(const std::string& filename, std::string& data)
{
  return GetImplementation(*this).LoadFile(filename, data);
}

FeedbackPlayer::FeedbackPlayer( Internal::Adaptor::FeedbackPlayer* player )
: BaseHandle( player )
{
}

} // namespace Dali
