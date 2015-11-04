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
#include <sound-player-impl.h>

// EXTERNAL INCLUDES
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

const char* const SIGNAL_SOUND_PLAY_FINISHED = "sound-play-finished";

// Type Registration
Dali::BaseHandle GetInstance()
{
  return SoundPlayer::Get();
}

Dali::TypeRegistration SOUND_PLAYER_TYPE( typeid(Dali::SoundPlayer), typeid(Dali::BaseHandle), GetInstance );

Dali::SignalConnectorType SIGNAL_CONNECTOR_1( SOUND_PLAYER_TYPE, SIGNAL_SOUND_PLAY_FINISHED, Dali::Internal::Adaptor::SoundPlayer::DoConnectSignal );

} // unnamed namespace

Dali::SoundPlayer SoundPlayer::New()
{
  Dali::SoundPlayer player = Dali::SoundPlayer( new SoundPlayer() );
  return player;
}

Dali::SoundPlayer SoundPlayer::Get()
{
  Dali::SoundPlayer player;

  Dali::SingletonService service( SingletonService::Get() );
  if ( service )
  {
    // Check whether the singleton is already created
    Dali::BaseHandle handle = service.GetSingleton( typeid( Dali::SoundPlayer ) );
    if ( handle )
    {
      // If so, downcast the handle
      player = Dali::SoundPlayer( dynamic_cast< SoundPlayer* >( handle.GetObjectPtr() ) );
    }
    else
    {
      player = Dali::SoundPlayer( New() );
      service.Register( typeid( player ), player );
    }
  }

  return player;
}

int SoundPlayer::PlaySound( const std::string fileName )
{
  return mPlugin.PlaySound( fileName );
}

void SoundPlayer::Stop( int handle )
{
  mPlugin.StopSound( handle );
}

SoundPlayer::SoundPlayFinishedSignalType& SoundPlayer::SoundPlayFinishedSignal()
{
  return mSoundPlayFinishedSignal;
}

bool SoundPlayer::DoConnectSignal( BaseObject* object, ConnectionTrackerInterface* tracker, const std::string& signalName, FunctorDelegate* functor )
{
  bool connected( true );
  SoundPlayer* player = dynamic_cast<SoundPlayer*>( object );

  if( player && ( SIGNAL_SOUND_PLAY_FINISHED == signalName ) )
  {
    player->SoundPlayFinishedSignal().Connect( tracker, functor );
  }
  else
  {
    // signalName does not match any signal
    connected = false;
  }

  return connected;
}

SoundPlayer::SoundPlayer()
: mPlugin( FeedbackPluginProxy::DEFAULT_OBJECT_NAME )
{
}

SoundPlayer::~SoundPlayer()
{
}

void SoundPlayer::EmitSoundPlayFinishedSignal()
{
  // Emit SoundPlayFinished signal

  if ( !mSoundPlayFinishedSignal.Empty() )
  {
    Dali::SoundPlayer handle( this );
    mSoundPlayFinishedSignal.Emit( handle );
  }
}

} // namespace Adaptor

} // namespace Internal

} // namespace Dali
