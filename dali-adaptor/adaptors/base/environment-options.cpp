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
#include "environment-options.h"

// EXTERNAL INCLUDES
#include <cstdlib>
#include <dali/integration-api/render-controller.h>
#include <dali/public-api/math/math-utils.h>

// INTERNAL INCLUDES
#include <base/environment-variables.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

namespace
{
const unsigned int DEFAULT_STATISTICS_LOG_FREQUENCY = 2;

unsigned int GetIntegerEnvironmentVariable( const char* variable, unsigned int defaultValue )
{
  const char* variableParameter = std::getenv(variable);

  // if the parameter exists convert it to an integer, else return the default value
  unsigned int intValue = variableParameter ? std::atoi(variableParameter) : defaultValue;
  return intValue;
}

bool GetIntegerEnvironmentVariable( const char* variable, int& intValue )
{
  const char* variableParameter = std::getenv(variable);

  if( !variableParameter )
  {
    return false;
  }
  // if the parameter exists convert it to an integer, else return the default value
  intValue = std::atoi(variableParameter);
  return true;
}

bool GetFloatEnvironmentVariable( const char* variable, float& floatValue )
{
  const char* variableParameter = std::getenv(variable);

  if( !variableParameter )
  {
    return false;
  }
  // if the parameter exists convert it to an integer, else return the default value
  floatValue = std::atof(variableParameter);
  return true;
}

const char * GetCharEnvironmentVariable( const char * variable )
{
  return std::getenv( variable );
}

} // unnamed namespace

EnvironmentOptions::EnvironmentOptions()
: mWindowName(),
  mWindowClassName(),
  mNetworkControl(0),
  mFpsFrequency(0),
  mUpdateStatusFrequency(0),
  mObjectProfilerInterval( 0 ),
  mPerformanceStatsLevel(0),
  mPerformanceStatsFrequency( DEFAULT_STATISTICS_LOG_FREQUENCY ),
  mPerformanceTimeStampOutput(0),
  mPanGestureLoggingLevel(0),
  mPanGesturePredictionMode(-1),
  mPanGesturePredictionAmount(-1), ///< only sets value in pan gesture if greater than 0
  mPanGestureMaxPredictionAmount(-1),
  mPanGestureMinPredictionAmount(-1),
  mPanGesturePredictionAmountAdjustment(-1),
  mPanGestureSmoothingMode(-1),
  mPanGestureSmoothingAmount(-1.0f),
  mPanMinimumDistance(-1),
  mPanMinimumEvents(-1),
  mGlesCallTime(0),
  mWindowWidth( 0 ),
  mWindowHeight( 0 )
{
  ParseEnvironmentOptions();
}

EnvironmentOptions::~EnvironmentOptions()
{
}

void EnvironmentOptions::SetLogFunction( const Dali::Integration::Log::LogFunction& logFunction )
{
  mLogFunction = logFunction;
}

void EnvironmentOptions::InstallLogFunction() const
{
  Dali::Integration::Log::InstallLogFunction( mLogFunction );
}

void EnvironmentOptions::UnInstallLogFunction() const
{
  Dali::Integration::Log::UninstallLogFunction();
}

unsigned int EnvironmentOptions::GetNetworkControlMode() const
{
  return mNetworkControl;
}
unsigned int EnvironmentOptions::GetFrameRateLoggingFrequency() const
{
  return mFpsFrequency;
}

unsigned int EnvironmentOptions::GetUpdateStatusLoggingFrequency() const
{
  return mUpdateStatusFrequency;
}

unsigned int EnvironmentOptions::GetObjectProfilerInterval() const
{
  return mObjectProfilerInterval;
}

unsigned int EnvironmentOptions::GetPerformanceStatsLoggingOptions() const
{
  return mPerformanceStatsLevel;
}
unsigned int EnvironmentOptions::GetPerformanceStatsLoggingFrequency() const
{
  return mPerformanceStatsFrequency;
}
unsigned int EnvironmentOptions::GetPerformanceTimeStampOutput() const
{
  return mPerformanceTimeStampOutput;
}

unsigned int EnvironmentOptions::GetPanGestureLoggingLevel() const
{
  return mPanGestureLoggingLevel;
}

int EnvironmentOptions::GetPanGesturePredictionMode() const
{
  return mPanGesturePredictionMode;
}

int EnvironmentOptions::GetPanGesturePredictionAmount() const
{
  return mPanGesturePredictionAmount;
}

int EnvironmentOptions::GetPanGestureMaximumPredictionAmount() const
{
  return mPanGestureMaxPredictionAmount;
}

int EnvironmentOptions::GetPanGestureMinimumPredictionAmount() const
{
  return mPanGestureMinPredictionAmount;
}

int EnvironmentOptions::GetPanGesturePredictionAmountAdjustment() const
{
  return mPanGesturePredictionAmountAdjustment;
}

int EnvironmentOptions::GetPanGestureSmoothingMode() const
{
  return mPanGestureSmoothingMode;
}

float EnvironmentOptions::GetPanGestureSmoothingAmount() const
{
  return mPanGestureSmoothingAmount;
}

int EnvironmentOptions::GetMinimumPanDistance() const
{
  return mPanMinimumDistance;
}

int EnvironmentOptions::GetMinimumPanEvents() const
{
  return mPanMinimumEvents;
}

unsigned int EnvironmentOptions::GetWindowWidth() const
{
  return mWindowWidth;
}

unsigned int EnvironmentOptions::GetWindowHeight() const
{
  return mWindowHeight;
}

int EnvironmentOptions::GetGlesCallTime() const
{
  return mGlesCallTime;
}

const std::string& EnvironmentOptions::GetWindowName() const
{
  return mWindowName;
}

const std::string& EnvironmentOptions::GetWindowClassName() const
{
  return mWindowClassName;
}

bool EnvironmentOptions::PerformanceServerRequired() const
{
  return ( ( GetPerformanceStatsLoggingOptions() > 0) ||
           ( GetPerformanceTimeStampOutput() > 0 ) ||
           ( GetNetworkControlMode() > 0) );
}

void EnvironmentOptions::ParseEnvironmentOptions()
{
  // get logging options
  mFpsFrequency = GetIntegerEnvironmentVariable( DALI_ENV_FPS_TRACKING, 0 );
  mUpdateStatusFrequency = GetIntegerEnvironmentVariable( DALI_ENV_UPDATE_STATUS_INTERVAL, 0 );
  mObjectProfilerInterval = GetIntegerEnvironmentVariable( DALI_ENV_OBJECT_PROFILER_INTERVAL, 0 );
  mPerformanceStatsLevel = GetIntegerEnvironmentVariable( DALI_ENV_LOG_PERFORMANCE_STATS, 0 );
  mPerformanceStatsFrequency = GetIntegerEnvironmentVariable( DALI_ENV_LOG_PERFORMANCE_STATS_FREQUENCY, 0 );
  mPerformanceTimeStampOutput = GetIntegerEnvironmentVariable( DALI_ENV_PERFORMANCE_TIMESTAMP_OUTPUT, 0 );
  mNetworkControl = GetIntegerEnvironmentVariable( DALI_ENV_NETWORK_CONTROL, 0 );
  mPanGestureLoggingLevel = GetIntegerEnvironmentVariable( DALI_ENV_LOG_PAN_GESTURE, 0 );

  int predictionMode;
  if( GetIntegerEnvironmentVariable(DALI_ENV_PAN_PREDICTION_MODE, predictionMode) )
  {
    mPanGesturePredictionMode = predictionMode;
  }
  int predictionAmount(-1);
  if( GetIntegerEnvironmentVariable(DALI_ENV_PAN_PREDICTION_AMOUNT, predictionAmount) )
  {
    if( predictionAmount < 0 )
    {
      // do not support times in the past
      predictionAmount = 0;
    }
    mPanGesturePredictionAmount = predictionAmount;
  }
  int minPredictionAmount(-1);
  if( GetIntegerEnvironmentVariable(DALI_ENV_PAN_MIN_PREDICTION_AMOUNT, minPredictionAmount) )
  {
    if( minPredictionAmount < 0 )
    {
      // do not support times in the past
      minPredictionAmount = 0;
    }
    mPanGestureMinPredictionAmount = minPredictionAmount;
  }
  int maxPredictionAmount(-1);
  if( GetIntegerEnvironmentVariable(DALI_ENV_PAN_MAX_PREDICTION_AMOUNT, maxPredictionAmount) )
  {
    if( minPredictionAmount > -1 && maxPredictionAmount < minPredictionAmount )
    {
      // maximum amount should not be smaller than minimum amount
      maxPredictionAmount = minPredictionAmount;
    }
    mPanGestureMaxPredictionAmount = maxPredictionAmount;
  }
  int predictionAmountAdjustment(-1);
  if( GetIntegerEnvironmentVariable(DALI_ENV_PAN_PREDICTION_AMOUNT_ADJUSTMENT, predictionAmountAdjustment) )
  {
    if( predictionAmountAdjustment < 0 )
    {
      // negative amount doesn't make sense
      predictionAmountAdjustment = 0;
    }
    mPanGesturePredictionAmountAdjustment = predictionAmountAdjustment;
  }
  int smoothingMode;
  if( GetIntegerEnvironmentVariable(DALI_ENV_PAN_SMOOTHING_MODE, smoothingMode) )
  {
    mPanGestureSmoothingMode = smoothingMode;
  }
  float smoothingAmount = 1.0f;
  if( GetFloatEnvironmentVariable(DALI_ENV_PAN_SMOOTHING_AMOUNT, smoothingAmount) )
  {
    smoothingAmount = Clamp(smoothingAmount, 0.0f, 1.0f);
    mPanGestureSmoothingAmount = smoothingAmount;
  }

  int minimumDistance(-1);
  if ( GetIntegerEnvironmentVariable(DALI_ENV_PAN_MINIMUM_DISTANCE, minimumDistance ))
  {
    mPanMinimumDistance = minimumDistance;
  }

  int minimumEvents(-1);
  if ( GetIntegerEnvironmentVariable(DALI_ENV_PAN_MINIMUM_EVENTS, minimumEvents ))
  {
    mPanMinimumEvents = minimumEvents;
  }

  int glesCallTime(0);
  if ( GetIntegerEnvironmentVariable(DALI_GLES_CALL_TIME, glesCallTime ))
  {
    mGlesCallTime = glesCallTime;
  }

  int windowWidth(0), windowHeight(0);
  if ( GetIntegerEnvironmentVariable( DALI_WINDOW_WIDTH, windowWidth ) && GetIntegerEnvironmentVariable( DALI_WINDOW_HEIGHT, windowHeight ) )
  {
    mWindowWidth = windowWidth;
    mWindowHeight = windowHeight;
  }

  const char * windowName = GetCharEnvironmentVariable( DALI_WINDOW_NAME );
  if ( windowName )
  {
    mWindowName = windowName;
  }

  const char * windowClassName = GetCharEnvironmentVariable( DALI_WINDOW_CLASS_NAME );
  if ( windowClassName )
  {
    mWindowClassName = windowClassName;
  }
}

} // Adaptor

} // Internal

} // Dali
