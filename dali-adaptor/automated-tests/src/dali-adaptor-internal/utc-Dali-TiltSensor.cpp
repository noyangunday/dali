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

#include <iostream>

#include <stdlib.h>
#include <stdint.h>
#include <Ecore.h>
#include <dali/dali.h>
#include <dali-test-suite-utils.h>
#include <tilt-sensor-impl.h>

using namespace Dali;

namespace
{
static const float ROTATION_EPSILON = 0.0001f;

/**
 * Helper to test whether timeout or tilt signal is received first
 */
struct SignalHelper : public ConnectionTracker
{
  SignalHelper()
  : mTiltSignalReceived( false ),
    mTimeoutOccurred( false )
  {
  }

  void OnTilted(const TiltSensor& sensor)
  {
    tet_printf("tilted signal received\n");

    mTiltSignalReceived = true;

    // quit the main loop to continue test
    //ecore_main_loop_quit();
  }

  bool OnTimeout()
  {
    tet_printf("timeout occurred\n");

    mTimeoutOccurred = true;

    // quit the main loop to continue test
    //ecore_main_loop_quit();

    return false;
  }

  bool mTiltSignalReceived; // True if tilted signal was received
  bool mTimeoutOccurred;    // True if timeout occured
};

TiltSensor GetTiltSensor()
{
  return Internal::Adaptor::TiltSensor::New();
}

bool ecore_timer_running = false;
Ecore_Task_Cb timer_callback_func=NULL;
const void* timer_callback_data=NULL;
intptr_t timerId = 8; // intptr_t has the same size as a pointer and is platform independent so this can be returned as a pointer in ecore_timer_add below without compilation warnings
}// anon namespace
extern "C"
{
Ecore_Timer* ecore_timer_add(double in,
                             Ecore_Task_Cb func,
                             const void   *data)
{
  ecore_timer_running = true;
  timer_callback_func = func;
  timer_callback_data = data;
  timerId+=8;
  return (Ecore_Timer*)timerId;
}

void* ecore_timer_del(Ecore_Timer *timer)
{
  ecore_timer_running = false;
  timer_callback_func = NULL;
  return NULL;
}

}



void tilt_sensor_startup(void)
{
}

void tilt_sensor_cleanup(void)
{
}


int UtcDaliTiltSensorEnable(void)
{
  TestApplication application;

  tet_infoline("UtcDaliTiltSensorEnable");

  TiltSensor sensor = GetTiltSensor();
  DALI_TEST_CHECK( sensor );

  sensor.Enable();
  DALI_TEST_CHECK( sensor.IsEnabled() );

  END_TEST;
}

int UtcDaliTiltSensorDisable(void)
{
  TestApplication application;

  tet_infoline("UtcDaliTiltSensorDisable");

  TiltSensor sensor = GetTiltSensor();
  DALI_TEST_CHECK( sensor );

  sensor.Enable();
  DALI_TEST_CHECK( sensor.IsEnabled() );

  sensor.Disable();
  DALI_TEST_CHECK( !sensor.IsEnabled() );
  END_TEST;
}

int UtcDaliTiltSensorIsEnabled(void)
{
  TestApplication application;

  tet_infoline("UtcDaliTiltSensorIsEnabled");

  TiltSensor sensor = GetTiltSensor();
  DALI_TEST_CHECK( sensor );

  // Should be disabled by default
  DALI_TEST_CHECK( !sensor.IsEnabled() );
  END_TEST;
}

int UtcDaliTiltSensorGetRoll(void)
{
  TestApplication application;

  tet_infoline("UtcDaliTiltSensorGetRoll");

  TiltSensor sensor = GetTiltSensor();
  DALI_TEST_CHECK( sensor );

  float roll = sensor.GetRoll();
  DALI_TEST_CHECK( roll <= 1.0f && roll >= -1.0f ); // range check
  END_TEST;
}

int UtcDaliTiltSensorGetPitch(void)
{
  TestApplication application;

  tet_infoline("UtcDaliTiltSensorGetPitch");

  TiltSensor sensor = GetTiltSensor();
  DALI_TEST_CHECK( sensor );

  float pitch = sensor.GetPitch();
  DALI_TEST_CHECK( pitch <= 1.0f && pitch >= -1.0f ); // range check
  END_TEST;
}

int UtcDaliTiltSensorGetRotation(void)
{
  TestApplication application;

  tet_infoline("UtcDaliTiltSensorGetRotation");

  TiltSensor sensor = GetTiltSensor();
  DALI_TEST_CHECK( sensor );

  Quaternion rotation = sensor.GetRotation();

  Radian roll( sensor.GetRoll() );
  Radian pitch( sensor.GetPitch() );

  Quaternion expectedRotation = Quaternion( roll  * Math::PI * -0.5f, Vector3::YAXIS ) *
                                Quaternion( pitch * Math::PI * -0.5f, Vector3::XAXIS );

  DALI_TEST_EQUALS( rotation, expectedRotation, ROTATION_EPSILON, TEST_LOCATION );
  END_TEST;
}


int UtcDaliTiltSensorSignalTilted(void)
{
  TestApplication application;

  tet_infoline("UtcDaliTiltSensorSignalTilted");

  TiltSensor sensor = GetTiltSensor();
  DALI_TEST_CHECK( sensor );
  sensor.Enable();

  Radian angle(Degree(-45));
  //Setting a negative threshold for testing purpose
  sensor.SetRotationThreshold( angle );

  END_TEST;
}

int UtcDaliTiltSensorSetUpdateFrequency(void)
{
  TestApplication application;

  tet_infoline("UtcDaliTiltSensorSetUpdateFrequency");

  TiltSensor sensor = GetTiltSensor();
  DALI_TEST_CHECK( sensor );
  sensor.SetUpdateFrequency( 1.0f/*hertz*/ );
  DALI_TEST_EQUALS( sensor.GetUpdateFrequency(), 1.0f, TEST_LOCATION );

  sensor.SetUpdateFrequency( 60.0f/*hertz*/ );
  DALI_TEST_EQUALS( sensor.GetUpdateFrequency(), 60.0f, TEST_LOCATION );

  END_TEST;
}


int UtcDaliTiltSensorSetRotationThreshold01(void)
{
  TestApplication application;

  tet_infoline("UtcDaliTiltSensorSetRotationThreshold01");

  TiltSensor sensor = GetTiltSensor();
  DALI_TEST_CHECK( sensor );
  sensor.Enable();

  Radian angle(Degree(-45));
  sensor.SetRotationThreshold( angle );
  DALI_TEST_EQUALS( sensor.GetRotationThreshold(), angle, TEST_LOCATION );

  angle = Degree(90);
  sensor.SetRotationThreshold( angle );
  DALI_TEST_EQUALS( sensor.GetRotationThreshold(), angle, TEST_LOCATION );
  END_TEST;
}
