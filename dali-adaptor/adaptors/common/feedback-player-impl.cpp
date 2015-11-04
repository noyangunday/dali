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
#include <feedback-player-impl.h>

// EXTERNAL INCLUDES
#include <fstream>
#include <dali/public-api/object/type-registry.h>

// INTERNAL INCLUDES
#include <singleton-service-impl.h>

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
  return FeedbackPlayer::Get();
}

Dali::TypeRegistration FEEDBACK_PLAYER_TYPE( typeid(Dali::FeedbackPlayer), typeid(Dali::BaseHandle), Create );

} // unnamed namespace

Dali::FeedbackPlayer FeedbackPlayer::New()
{
  Dali::FeedbackPlayer player = Dali::FeedbackPlayer( new FeedbackPlayer() );
  return player;
}

Dali::FeedbackPlayer FeedbackPlayer::Get()
{
  Dali::FeedbackPlayer player;

  Dali::SingletonService service( SingletonService::Get() );
  if ( service )
  {
    // Check whether the singleton is already created
    Dali::BaseHandle handle = service.GetSingleton( typeid( Dali::FeedbackPlayer ) );
    if ( handle )
    {
      // If so, downcast the handle
      player = Dali::FeedbackPlayer( dynamic_cast< FeedbackPlayer* >( handle.GetObjectPtr() ) );
    }
    else
    {
      player = Dali::FeedbackPlayer( New() );
      service.Register( typeid( player ), player );
    }
  }

  return player;
}

void FeedbackPlayer::PlayMonotone( unsigned int duration )
{
  mPlugin.PlayHapticMonotone( duration );
}

void FeedbackPlayer::PlayFile( const std::string& filePath )
{
  mPlugin.PlayHaptic( filePath );
}

void FeedbackPlayer::Stop()
{
  mPlugin.StopHaptic();
}

int FeedbackPlayer::PlaySound( const std::string& filename )
{
  return mPlugin.PlaySound(filename);
}

void FeedbackPlayer::StopSound( int handle )
{
  mPlugin.StopSound(handle);
}

void FeedbackPlayer::PlayFeedbackPattern( int type, int pattern )
{
  mPlugin.PlayFeedbackPattern(type, pattern);
}

bool FeedbackPlayer::LoadFile(const std::string& filename, std::string& data)
{
  bool loaded = false;

  std::ifstream stream(filename.c_str());

  if( stream.is_open() )
  {
    data.assign((std::istreambuf_iterator<char>(stream)),
                std::istreambuf_iterator<char>());
    loaded = true;
  }

  return loaded;
}

FeedbackPlayer::FeedbackPlayer()
: mPlugin( FeedbackPluginProxy::DEFAULT_OBJECT_NAME )
{
}

FeedbackPlayer::~FeedbackPlayer()
{
}

} // namespace Adaptor

} // namespace Internal

} // namespace Dali
