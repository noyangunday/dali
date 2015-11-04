#ifndef __DALI_INTERNAL_ADAPTOR_STAT_CONTEXT_H__
#define __DALI_INTERNAL_ADAPTOR_STAT_CONTEXT_H__

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
#include <string>

// INTERNAL INCLUDES
#include <base/performance-logging/performance-marker.h>
#include <base/performance-logging/frame-time-stats.h>
#include <base/interfaces/performance-interface.h>
#include <base/performance-logging/statistics/stat-context-log-interface.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

/**
 * Stores and prints statistics for a particular logging context.
 *
 */
class StatContext
{

public:

    /**
     * @brief Constructor
     *
     * @param[in] id The ID to give the context
     * @param[in] contextName Name of the context to print in console
     * @param[in] contextType the type of events to filter ( e.g. event, update, render or custom)
     * @param[in] logFrequencySeconds frequency to log in seconds
     * @param[in] logInterface interface to log out to
     *
     */
    StatContext( unsigned int id,
                 const char* const contextName,
                 PerformanceMarker::MarkerFilter contextType,
                 unsigned int logFrequencySeconds,
                 StatContextLogInterface& logInterface );


    /**
     * @brief Non-virtual destructor, not intended as a base class
     */
    ~StatContext();

    /**
     * @return Return the context ID
     */
    unsigned int GetId() const;

    /**
     * @return the context name
     */
    const char* const GetName() const;

    /**
     *
     * For logging we want to output the name of the context with either
     * START / END appended to the end. E.g. MY_MARKER_START
     * @param[in] type marker type, for a customer marker this will be either START or END
     * @return the full description for a marker
     */
    const char* const GetMarkerDescription( PerformanceInterface::MarkerType type ) const;

    /**
     * @brief Set the frequency for logging
     *
     * @param[in] logFrequencySeconds The log frequency to set in seconds
     */
    void SetLogFrequency( unsigned int logFrequencySeconds );

    /**
     * @brief enable/disable logging
     *
     * @param[in] enableLogging Flag to spePerformancecify enabling/disabling
     */
    void EnableLogging( bool enableLogging );

    /**
     * @brief  Process a custom marker from the application
     *
     * @param[in] marker The marker to log
     */
    void ProcessCustomMarker( const PerformanceMarker& marker );

    /**
     * @brief Process a internal marker from DALi (V_SYNC/ UPDATE /RENDER/ EVENT )
     *
     * @param[in] marker The marker to log
     */
    void ProcessInternalMarker( const PerformanceMarker& marker );

  private:

    /**
     * @brief Record marker
     *
     * @param[in] marker to record
     */
    void RecordMarker( const PerformanceMarker& marker );

    /**
     * @brief Called when V-SYNC occurs to indicate a frame tick
     * @param[in] marker the marker containing a v-sync
     */
    void FrameTick( const PerformanceMarker& marker );

    /**
     * @brief Helper to print to console
     */
    void LogMarker();


  private:

    StatContext();                                ///< undefined default constructor

    StatContext( const StatContext& );            ///< undefined copy constructor

    StatContext& operator=( const StatContext& ); ///< undefined assignment operator

  private:

    PerformanceMarker mInitialMarker;             ///< Used to store initial time
    FrameTimeStats mStats;                        ///< Frame time stats to accumulate
    const char* const mName;                      ///< Name of the context
    char* mTempLogBuffer;                         ///< Temporary log buffer
    StatContextLogInterface& mLogInterface;       ///< Log interface
    const std::string mNamePlusStart;             ///< Name of the context + _START
    const std::string mNamePlusEnd;                ///< Name of the context + _END
    unsigned int mId;                             ///< The ID of the context
    unsigned int mLogFrequencyMicroseconds;       ///< if logging is enabled, what frequency to log out at in micro-seconds
    PerformanceMarker::MarkerFilter mFilterType;  ///< type of events the context is filtering
    bool mLoggingEnabled:1;                       ///< Whether to print the log for this context or not
    bool mInitialMarkerSet:1;                     ///< Whether the initial marker has been set

};


} // namespace Internal

} // namespace Adaptor

} // namespace Dali

#endif
