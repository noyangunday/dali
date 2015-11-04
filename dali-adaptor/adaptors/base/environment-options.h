#ifndef __DALI_INTERNAL_ADAPTOR_ENVIRONMENT_OPTIONS_H__
#define __DALI_INTERNAL_ADAPTOR_ENVIRONMENT_OPTIONS_H__

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
#include <dali/integration-api/debug.h>

namespace Dali
{
namespace Internal
{
namespace Adaptor
{

/**
 * This class provides the environment options which define settings as well as
 * the ability to install a log function.
 *
 */
class EnvironmentOptions
{

public:

  /**
   * Constructor
   */
  EnvironmentOptions();

  /**
   * non-virtual destructor, not intended as a base class
   */
  ~EnvironmentOptions();

  /**
   * @param logFunction logging function
   */
  void SetLogFunction( const Dali::Integration::Log::LogFunction& logFunction );

  /**
   * Install the log function for the current thread.
   */
  void InstallLogFunction() const;

  /**
   * Un-install the log function for the current thread.
   */
  void UnInstallLogFunction() const;

  /**
   * @return whether network control is enabled or not ( 0 = off, 1 = on )
   */
  unsigned int GetNetworkControlMode() const;

  /**
   * @return frequency of how often FPS is logged out (e.g. 0 = off, 2 = every 2 seconds).
   */
  unsigned int GetFrameRateLoggingFrequency() const;

  /**
   * @return frequency of how often Update Status is logged out (e.g. 0 = off, 60 = log every 60 frames = 1 second @ 60FPS).
   */
  unsigned int GetUpdateStatusLoggingFrequency() const;

  /**
   * @return object profiler status interval ( 0 == off )
   */
  unsigned int GetObjectProfilerInterval() const;

  /**
   * @return performance statistics log level ( 0 == off )
   */
  unsigned int GetPerformanceStatsLoggingOptions() const;

  /**
   * @return performance statistics log frequency in seconds
   */
  unsigned int GetPerformanceStatsLoggingFrequency() const;

  /**
   * @return performance time stamp output ( 0 == off)
   */
  unsigned int GetPerformanceTimeStampOutput() const;

  /**
   * @return pan-gesture logging level ( 0 == off )
   */
  unsigned int GetPanGestureLoggingLevel() const;

  /**
   * @return pan-gesture prediction mode ( -1 means not set so no prediction, 0 = no prediction )
   */
  int GetPanGesturePredictionMode() const;

  /**
   * @return pan-gesture prediction amount
   */
  int GetPanGesturePredictionAmount() const;

  /**
   * @return maximum pan-gesture prediction amount
   */
  int GetPanGestureMaximumPredictionAmount() const;

  /**
   * @return minimum pan-gesture prediction amount
   */
  int GetPanGestureMinimumPredictionAmount() const;

  /**
   * @brief Gets the prediction amount to adjust when the pan velocity is changed.
   *
   * If the pan velocity is accelerating, the prediction amount will be increased
   * by the specified amount until it reaches the upper bound. If the pan velocity
   * is decelerating, the prediction amount will be decreased by the specified
   * amount until it reaches the lower bound.
   *
   * @return pan-gesture prediction amount adjustment
   */
  int GetPanGesturePredictionAmountAdjustment() const;

  /**
   * @return pan-gesture smoothing mode ( -1 means not set so no smoothing, 0 = no smoothing )
   */
  int GetPanGestureSmoothingMode() const;

  /**
   * @return pan-gesture smoothing amount
   */
  float GetPanGestureSmoothingAmount() const;

  /**
   * @return The minimum distance before a pan can be started (-1 means it's not set)
   */
  int GetMinimumPanDistance() const;

  /**
   * @return The minimum events before a pan can be started (-1 means it's not set)
   */
  int GetMinimumPanEvents() const;

  /**
   * @return The width of the window
   */
  unsigned int GetWindowWidth() const;

  /**
   * @return The height of the window
   */
  unsigned int GetWindowHeight() const;

  /**
   * @brief Get the graphics status time
   */
  int GetGlesCallTime() const;

  /**
   * @return true if performance server is required
   */
  bool PerformanceServerRequired() const;

  /**
   * @return Gets the window name.
   */
  const std::string& GetWindowName() const;

  /**
   * @return Gets the window class.
   */
  const std::string& GetWindowClassName() const;

private: // Internal

  /**
   * Parses the environment options.
   * Called from the constructor
   */
  void ParseEnvironmentOptions();

private: // Data

  std::string mWindowName;                        ///< name of the window
  std::string mWindowClassName;                   ///< name of the class the window belongs to
  unsigned int mNetworkControl;                   ///< whether network control is enabled
  unsigned int mFpsFrequency;                     ///< how often fps is logged out in seconds
  unsigned int mUpdateStatusFrequency;            ///< how often update status is logged out in frames
  unsigned int mObjectProfilerInterval;           ///< how often object counts are logged out in seconds
  unsigned int mPerformanceStatsLevel;            ///< performance statistics logging bitmask
  unsigned int mPerformanceStatsFrequency;        ///< performance statistics logging frequency (seconds)
  unsigned int mPerformanceTimeStampOutput;       ///< performance time stamp output ( bitmask)
  unsigned int mPanGestureLoggingLevel;           ///< pan-gesture log level
  int mPanGesturePredictionMode;                  ///< prediction mode for pan gestures
  int mPanGesturePredictionAmount;                ///< prediction amount for pan gestures
  int mPanGestureMaxPredictionAmount;             ///< maximum prediction amount for pan gestures
  int mPanGestureMinPredictionAmount;             ///< minimum prediction amount for pan gestures
  int mPanGesturePredictionAmountAdjustment;      ///< adjustment of prediction amount for pan gestures
  int mPanGestureSmoothingMode;                   ///< prediction mode for pan gestures
  float mPanGestureSmoothingAmount;               ///< prediction amount for pan gestures
  int mPanMinimumDistance;                        ///< minimum distance required before pan starts
  int mPanMinimumEvents;                          ///< minimum events required before pan starts
  int mGlesCallTime;                              ///< time in seconds between status updates
  unsigned int mWindowWidth;                      ///< width of the window
  unsigned int mWindowHeight;                     ///< height of the window

  Dali::Integration::Log::LogFunction mLogFunction;

  // Undefined copy constructor.
  EnvironmentOptions( const EnvironmentOptions& );

  // Undefined assignment operator.
  EnvironmentOptions& operator=( const EnvironmentOptions& );

};

} // Adaptor
} // Internal
} // Dali

#endif // __DALI_INTERNAL_ADAPTOR_ENVIRONMENT_OPTIONS_H__
