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
#include "tilt-sensor-impl.h"

// EXTERNAL INCLUDES
#include <cmath>
#ifdef SENSOR_ENABLED
#include <sensor_internal.h>
#endif

#include <dali/public-api/object/type-registry.h>
#include <dali/integration-api/debug.h>

// INTERNAL INCLUDES
#include <singleton-service-impl.h>

namespace // unnamed namespace
{

const char* const SIGNAL_TILTED = "tilted";

const int NUMBER_OF_SAMPLES = 10;

const float MAX_ACCELEROMETER_XY_VALUE = 9.8f;

// Type Registration
Dali::BaseHandle GetInstance()
{
  return Dali::Internal::Adaptor::TiltSensor::Get();
}

Dali::TypeRegistration typeRegistration( typeid(Dali::TiltSensor), typeid(Dali::BaseHandle), GetInstance );

Dali::SignalConnectorType signalConnector1( typeRegistration, SIGNAL_TILTED, Dali::Internal::Adaptor::TiltSensor::DoConnectSignal );

} // unnamed namespace

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

Dali::TiltSensor TiltSensor::New()
{
  Dali::TiltSensor sensor = Dali::TiltSensor(new TiltSensor());

  return sensor;
}

Dali::TiltSensor TiltSensor::Get()
{
  Dali::TiltSensor sensor;

  Dali::SingletonService service( SingletonService::Get() );
  if ( service )
  {
    // Check whether the keyboard focus manager is already created
    Dali::BaseHandle handle = service.GetSingleton( typeid( Dali::TiltSensor ) );
    if(handle)
    {
      // If so, downcast the handle of singleton to keyboard focus manager
      sensor = Dali::TiltSensor( dynamic_cast< TiltSensor* >( handle.GetObjectPtr() ) );
    }
    else
    {
      // Create a singleton instance
      sensor = TiltSensor::New();
      service.Register( typeid( sensor ), sensor );
      handle = sensor;
    }
  }

  return sensor;
}

TiltSensor::~TiltSensor()
{
  Disable();
}

bool TiltSensor::Enable()
{
  // Make sure sensor API is responding
  bool success = Update();

  if ( success )
  {
    if ( !mTimer )
    {
      mTimer = Dali::Timer::New( 1000.0f / mFrequencyHertz );
      mTimer.TickSignal().Connect( mTimerSlot, &TiltSensor::Update );
    }

    if ( mTimer &&
         !mTimer.IsRunning() )
    {
      mTimer.Start();
    }
  }

  return success;
}

void TiltSensor::Disable()
{
  if ( mTimer )
  {
    mTimer.Stop();
    mTimer.Reset();
  }
}

bool TiltSensor::IsEnabled() const
{
  return ( mTimer && mTimer.IsRunning() );
}

float TiltSensor::GetRoll() const
{
  return mRoll;
}

float TiltSensor::GetPitch() const
{
  return mPitch;
}

Quaternion TiltSensor::GetRotation() const
{
  return mRotation;
}

TiltSensor::TiltedSignalType& TiltSensor::TiltedSignal()
{
  return mTiltedSignal;
}

void TiltSensor::SetUpdateFrequency( float frequencyHertz )
{
  DALI_ASSERT_ALWAYS( frequencyHertz > 0.0f && "Frequency must have a positive value" );

  if ( fabsf(mFrequencyHertz - frequencyHertz) >= GetRangedEpsilon(mFrequencyHertz, frequencyHertz) )
  {
    mFrequencyHertz = frequencyHertz;

    if ( mTimer )
    {
      mTimer.SetInterval( 1000.0f / mFrequencyHertz );
    }
  }
}

float TiltSensor::GetUpdateFrequency() const
{
  return mFrequencyHertz;
}

void TiltSensor::SetRotationThreshold(Radian rotationThreshold)
{
  mRotationThreshold = rotationThreshold;
}

Radian TiltSensor::GetRotationThreshold() const
{
  return mRotationThreshold;
}

bool TiltSensor::DoConnectSignal( BaseObject* object, ConnectionTrackerInterface* tracker, const std::string& signalName, FunctorDelegate* functor )
{
  bool connected( true );
  TiltSensor* sensor = dynamic_cast<TiltSensor*>( object );

  if( sensor && ( SIGNAL_TILTED == signalName ) )
  {
    sensor->TiltedSignal().Connect( tracker, functor );
  }
  else
  {
    // signalName does not match any signal
    connected = false;
  }

  return connected;
}

TiltSensor::TiltSensor()
: mFrequencyHertz( Dali::TiltSensor::DEFAULT_UPDATE_FREQUENCY ),
  mTimerSlot( this ),
  mSensorFrameworkHandle( -1 ),
  mRoll( 0.0f ),
  mPitch( 0.0f ),
  mRotation( Radian( 0.0f), Vector3::YAXIS ),
  mRotationThreshold( 0.0f )
{
  mRollValues.resize( NUMBER_OF_SAMPLES, 0.0f );
  mPitchValues.resize( NUMBER_OF_SAMPLES, 0.0f );
}

bool TiltSensor::Update()
{
  float newRoll = 0.0f;
  float newPitch = 0.0f;
  Quaternion newRotation;
#ifdef SENSOR_ENABLED

  // Read accelerometer data

  mSensorFrameworkHandle = sf_connect( ACCELEROMETER_SENSOR );
  if ( mSensorFrameworkHandle < 0 )
  {
    DALI_LOG_ERROR( "Failed to connect to sensor framework" );
    return false;
  }

  if ( sf_start(mSensorFrameworkHandle, 0) < 0 )
  {
    DALI_LOG_ERROR( "Failed to start sensor" );
    sf_disconnect(mSensorFrameworkHandle);
    return false;
  }

  sensor_data_t* base_data_values = (sensor_data_t*)malloc(sizeof(sensor_data_t));

  int dataErr = sf_get_data(mSensorFrameworkHandle, ACCELEROMETER_BASE_DATA_SET, base_data_values);
  if ( dataErr < 0 )
  {
    DALI_LOG_ERROR( "Failed to retrieve sensor data" );
    free(base_data_values);
    sf_stop(mSensorFrameworkHandle);
    sf_disconnect(mSensorFrameworkHandle);
    return false;
  }

  sf_stop(mSensorFrameworkHandle);
  sf_disconnect(mSensorFrameworkHandle);

  mRollValues.push_back( base_data_values->values[0] );
  mRollValues.pop_front();

  mPitchValues.push_back( base_data_values->values[1] );
  mPitchValues.pop_front();

  free(base_data_values);
  base_data_values = NULL;

  float averageRoll( 0.0f );
  for ( std::deque<float>::const_iterator iter = mRollValues.begin(); mRollValues.end() != iter; ++iter )
  {
    averageRoll += *iter;
  }
  averageRoll /= mRollValues.size();

  float averagePitch( 0.0f );
  for ( std::deque<float>::const_iterator iter = mPitchValues.begin(); mPitchValues.end() != iter; ++iter )
  {
    averagePitch += *iter;
  }
  averagePitch /= mPitchValues.size();

  newRoll  = Clamp( float(averageRoll  / MAX_ACCELEROMETER_XY_VALUE), -1.0f/*min*/, 1.0f/*max*/ );
  newPitch = Clamp( float(averagePitch / MAX_ACCELEROMETER_XY_VALUE), -1.0f/*min*/, 1.0f/*max*/ );

  newRotation = Quaternion( Radian( newRoll  * Math::PI * -0.5f ), Vector3::YAXIS ) *
                Quaternion( Radian( newPitch * Math::PI * -0.5f ), Vector3::XAXIS );
#endif // SENSOR_ENABLED

  Radian angle(Quaternion::AngleBetween(newRotation, mRotation));
  // If the change in value is more than the threshold then emit tilted signal.
  if( angle > mRotationThreshold )
  {
    mRoll = newRoll;
    mPitch = newPitch;
    mRotation = newRotation;

    if ( !mTiltedSignal.Empty() )
    {
      Dali::TiltSensor handle( this );
      mTiltedSignal.Emit( handle );
    }
  }

  return true;
}

} // namespace Adaptor

} // namespace Internal

} // namespace Dali
