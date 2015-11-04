#ifndef __DALI_INTERNAL_ADAPTOR_STAT_CONTEXT_MANAGER_H__
#define __DALI_INTERNAL_ADAPTOR_STAT_CONTEXT_MANAGER_H__

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

// EXTERNAL INCLUDES
#include <dali/devel-api/threading/mutex.h>

// INTERNAL INCLUDES
#include <base/performance-logging/performance-marker.h>
#include <base/performance-logging/statistics/stat-context.h>
#include <base/interfaces/performance-interface.h>


namespace Dali
{

namespace Internal
{

namespace Adaptor
{

/**
 * Class to manage StatContext objects.
 *
 * Contains 3 built in contexts for event, update, render.
 * The application developer can add more using the PerformanceLogger public API
 *
 * Example output of 4 contexts ( event, update, render and a custom one):
 *
 * Event, min 0.04 ms, max 5.27 ms, total (0.1 secs), avg 0.28 ms, std dev 0.73 ms
 * Update, min 0.29 ms, max 0.91 ms, total (0.5 secs), avg 0.68 ms, std dev 0.15 ms
 * Render, min 0.33 ms, max 0.97 ms, total (0.6 secs), avg 0.73 ms, std dev 0.17 ms
 * MyAppTask, min 76.55 ms, max 76.55 ms, total (0.1 secs), avg 76.55 ms, std dev 0.00 ms  (CUSTOM CONTEXT)
 *
 */
class StatContextManager
{

public:

    /**
     * @brief Constructor
     * @param[in] logInterface interface to log statistics to
     */
    StatContextManager( StatContextLogInterface& logInterface );

    /**
     * @brief destructor, not intended as a bass class
     */
    ~StatContextManager();

    /**
     * @brief Add a context
     * @param[in] name Name of the context to print in console
     * @param[in] type the type of events to filter ( e.g. event, update, render or custom)
     * @return The ID to give the context
     */
    PerformanceInterface::ContextId AddContext( const char* const name, PerformanceMarker::MarkerFilter type );

    /**
     * @brief Remove a context
     * @param[in] contextId id of the context to remove
     */
    void RemoveContext(PerformanceInterface::ContextId contextId );

    /**
     * @brief Add an internal marker (e.g. v-sync, update, render markers)
     * @param[in] marker the marker to add
     */
    void AddInternalMarker( const PerformanceMarker& marker );

    /**
     * @brief Add a custom marker defined by the application
     * @param[in] marker the marker to add
     * @param[in] contextId the context the custom marker is designed for
     */
    void AddCustomMarker( const PerformanceMarker& marker , PerformanceInterface::ContextId contextId );

    /**
     * @brief Get the nane of a context
     * @param[in] contextId id of the context to get the name
     * @return context name
     */
    const char* const GetContextName( PerformanceInterface::ContextId contextId ) const;

    /**
     * @brief Get the full description of a marker for this context
     * @param[in] type marker type, for a customer marker this will be either START or END
     * @param[in] contextId id of the context to get the name
     * @return marker description in relation to this context
     */
    const char* const GetMarkerDescription( PerformanceInterface::MarkerType type, PerformanceInterface::ContextId contextId ) const;


    /**
     * @brief enable / disable logging for a context
     * @param[in] enable whether to enable logging
     * @param[in] contextId the context to configure
     */
    void EnableLogging( bool enable, PerformanceInterface::ContextId contextId );

    /**
     * @brief set global logging level and frequency.
     * @param[in] statisticsLogOptions  log options
     * @param[in] logFrequency frequency in seconds
     */
    void SetLoggingLevel( unsigned int statisticsLogOptions, unsigned int logFrequency);

    /**
     * @brief Set the frequency of logging for an individual context
     * @param[in] logFrequency log frequency in seconds
     * @param[in] contextId the context to configure
     */
    void SetLoggingFrequency( unsigned int logFrequency, PerformanceInterface::ContextId contextId  );

  private:

    typedef Dali::Vector< StatContext* > StatContexts;

    /**
     * @brief helper
     * @param[in] contextId the context to get
     * @return context
     */
    StatContext* GetContext( PerformanceInterface::ContextId contextId ) const;

    Dali::Mutex mDataMutex;                            ///< mutex
    StatContexts mStatContexts;                        ///< The list of stat contexts
    StatContextLogInterface& mLogInterface;            ///< Log interface

    PerformanceInterface::ContextId mNextContextId;    ///< The next valid context ID

    // Some defaults contexts
    PerformanceInterface::ContextId mUpdateStats;    ///< update time statistics
    PerformanceInterface::ContextId mRenderStats;    ///< render time statistics
    PerformanceInterface::ContextId mEventStats;     ///< event time statistics

    unsigned int mStatisticsLogBitmask;              ///< statistics log bitmask
    unsigned int mLogFrequency;                      ///< log frequency
};


} // namespace Internal

} // namespace Adaptor

} // namespace Dali

#endif // __DALI_INTERNAL_ADAPTOR_STAT_CONTEXT_MANAGER_H__

