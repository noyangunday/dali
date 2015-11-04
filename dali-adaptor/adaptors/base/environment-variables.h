#ifndef __DALI_INTERNAL_ADAPTOR_ENVIRONMENT_VARIABLES_H__
#define __DALI_INTERNAL_ADAPTOR_ENVIRONMENT_VARIABLES_H__

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

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

/**
 * What performance statistics are logged out to dlog
 * see StatisticsLogOptions in performance-interface.h for values
 */
#define DALI_ENV_LOG_PERFORMANCE_STATS "DALI_LOG_PERFORMANCE_STATS"

/**
 * How frequent in seconds to log out performance statistics
 */
#define DALI_ENV_LOG_PERFORMANCE_STATS_FREQUENCY "DALI_LOG_PERFORMANCE_STATS_FREQ"

/**
 * Where timestamped events for update/render/event and custom events
 * are output.
 * see TimeStampOutput in performance-interface.h for values
 */
#define DALI_ENV_PERFORMANCE_TIMESTAMP_OUTPUT "DALI_PERFORMANCE_TIMESTAMP_OUTPUT"

/**
 * Allow control and monitoring of DALi via the network
 */
#define DALI_ENV_NETWORK_CONTROL "DALI_NETWORK_CONTROL"

// environment variable for enabling/disabling fps tracking
#define DALI_ENV_FPS_TRACKING "DALI_FPS_TRACKING"

#define DALI_ENV_UPDATE_STATUS_INTERVAL "DALI_UPDATE_STATUS_INTERVAL"

#define DALI_ENV_OBJECT_PROFILER_INTERVAL "DALI_OBJECT_PROFILER_INTERVAL"

#define DALI_ENV_LOG_PAN_GESTURE "DALI_LOG_PAN_GESTURE"

#define DALI_ENV_PAN_PREDICTION_MODE "DALI_PAN_PREDICTION_MODE"

#define DALI_ENV_PAN_PREDICTION_AMOUNT "DALI_PAN_PREDICTION_AMOUNT"

#define DALI_ENV_PAN_MAX_PREDICTION_AMOUNT "DALI_PAN_MAX_PREDICTION_AMOUNT"

#define DALI_ENV_PAN_MIN_PREDICTION_AMOUNT "DALI_PAN_MIN_PREDICTION_AMOUNT"

#define DALI_ENV_PAN_PREDICTION_AMOUNT_ADJUSTMENT "DALI_PAN_PREDICTION_AMOUNT_ADJUSTMENT"

#define DALI_ENV_PAN_SMOOTHING_MODE "DALI_PAN_SMOOTHING_MODE"

#define DALI_ENV_PAN_SMOOTHING_AMOUNT "DALI_PAN_SMOOTHING_AMOUNT"

#define DALI_ENV_PAN_MINIMUM_DISTANCE "DALI_PAN_MINIMUM_DISTANCE"

#define DALI_ENV_PAN_MINIMUM_EVENTS "DALI_PAN_MINIMUM_EVENTS"

#define DALI_GLES_CALL_TIME "DALI_GLES_CALL_TIME"

#define DALI_WINDOW_WIDTH "DALI_WINDOW_WIDTH"

#define DALI_WINDOW_HEIGHT "DALI_WINDOW_HEIGHT"

#define DALI_WINDOW_NAME "DALI_WINDOW_NAME"

#define DALI_WINDOW_CLASS_NAME "DALI_WINDOW_CLASS_NAME"

} // namespace Adaptor

} // namespace Internal

} // namespace Dali

#endif // __DALI_INTERNAL_ADAPTOR_ENVIRONMENT_VARIABLES_H__
