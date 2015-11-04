#ifndef __DALI_TILT_SENSOR_H__
#define __DALI_TILT_SENSOR_H__

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

namespace Internal DALI_INTERNAL
{
namespace Adaptor
{
class TiltSensor;
}
}

/**
 * TiltSensor provides pitch & roll values when the device is tilted.
 * The basic usage is shown below:
 *
 * @code
 *
 *  void Example()
 *  {
 *    TiltSensor sensor = TiltSensor::Get();
 *
 *    // Try to enable the sensor
 *    if ( sensor.Enable() )
 *    {
 *      // Query the current values
 *      std::cout << "Roll = " << sensor.GetRoll() << ", Pitch = " << sensor.GetPitch() << std::endl;
 *
 *      // Get notifications when the device is tilted
 *      sensor.SignalTilted().Connect( &OnTilted );
 *    }
 *  }
 *
 *  void OnTilted()
 *  {
 *    // Query the new values
 *    std::cout << "Roll = " << sensor.GetRoll() << ", Pitch = " << sensor.GetPitch() << std::endl;
 *  }
 *
 * @endcode
 *
 * While the tilt sensor is enabled, it will periodically poll for the latest pitch & roll values.
 * For performance & power-saving, applications should disable this polling when no longer needed:
 *
 * @code
 *
 *  void EndExample()
 *  {
 *    // Disable the sensor when no longer needed
 *    TiltSensor::Get().Disable();
 *  }
 *
 * @endcode
 */
class DALI_IMPORT_API TiltSensor : public BaseHandle
{
public:

  typedef Signal< void (const TiltSensor&) > TiltedSignalType;

  static const float DEFAULT_UPDATE_FREQUENCY; // 60 hertz

  /**
   * Create an uninitialized handle.
   * This can be initialized by calling TiltSensor::Get().
   */
  TiltSensor();

  /**
   * Create an initialized handle to the TiltSensor.
   * @return A handle to a newly allocated Dali resource.
   */
  static TiltSensor Get();

  /**
   * @brief Destructor
   *
   * This is non-virtual since derived Handle types must not contain data or virtual methods.
   */
  ~TiltSensor();

  /**
   * Attempt to enable the tilt-sensor. This will fail if the underlying sensor hardware is powered-down,
   * typically this occurs when the device is set to "sleep" mode.
   * @return True if the tilt-sensor is enabled.
   */
  bool Enable();

  /**
   * Disable the tilt-sensor.
   */
  void Disable();

  /**
   * Query whether the tilt-sensor is disabled.
   * The sensor may be disabled automatically; typically this occurs when the device is set to "sleep" mode.
   * @return True if the tilt-sensor is enabled.
   */
  bool IsEnabled() const;

  /**
   * Query the roll value. This is in the range -1 to 1.
   * When the device is lying face-up on a flat surface, this method will return a value close to zero.
   * A value close to 1 indicates that the right-side of the device is pointing upwards.
   * A value close to -1 indicates that the right-side of the device is pointing downwards.
   * @pre The tilt-sensor is enabled.
   * @return The roll value.
   */
  float GetRoll() const;

  /**
   * Query the pitch value. This is in the range -1 to 1.
   * When the device is lying face-up on a flat surface, this method will return a value close to zero.
   * A value close to 1 indicates that the top of the device is pointing upwards.
   * A value close to -1 indicates that the top of the device is pointing downwards.
   * @pre The tilt-sensor is enabled.
   * @return The pitch value.
   */
  float GetPitch() const;

  /**
   * Retrieve the rotation of the device.
   * When the device is lying face-up on a flat surface, the rotation angle will be approximately zero.
   * The roll & pitch of the device is considered to be a rotation around the Y and X axes respectively.
   * @pre The tilt-sensor is enabled.
   * @return The rotation in quaternion format.
   */
  Quaternion GetRotation() const;

  /**
   * This signal will be emitted when the device is tilted, if the tilt-sensor is enabled.
   * The frequency of the signals can be controlled using SetUpdateFrequency().
   * @return The signal to connect to.
   *
   * @note The signal name is "tilted" if using BaseHandle::ConnectSignal()
   */
  TiltedSignalType& TiltedSignal();

  /**
   * Set the sensor update frequency.
   * The default is TiltSensor::DEFAULT_UPDATE_FREQUENCY.
   * @param[in] frequencyHertz The frequency in hertz.
   */
  void SetUpdateFrequency( float frequencyHertz );

  /**
   * Query the sensor update frequency.
   * @return The frequency in hertz.
   */
  float GetUpdateFrequency() const;

  /**
   * Set the threshold value for rotation in Radians, above which TiltedSignal should be emitted.
   * The default is 0.0f in Radians (i.e) it will be emitted always at the frequency set.
   * Example tiltSensor.SetRotationThreshold( Radian(Degree(10) ) // A rotation threshold of 10 degrees
   * @param[in] rotationThreshold The threshold value for rotation.
   */
  void SetRotationThreshold( Radian rotationThreshold );

  /**
   * Query the rotation threshold above which TiltedSignal will be emitted.
   * @return The rotation degree threshold in Radians.
   */
  Radian GetRotationThreshold() const;

public: // Not intended for application developers

  /**
   * This constructor is used by TiltSensor::Get().
   * @param[in] sensor A pointer to the tilt sensor.
   */
  explicit DALI_INTERNAL TiltSensor( Internal::Adaptor::TiltSensor* sensor );
};

} // namespace Dali

#endif // __DALI_TILT_SENSOR_H__
