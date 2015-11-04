#ifndef __DALI_INTERNAL_PERFORMANCE_LOGGER_H__
#define __DALI_INTERNAL_PERFORMANCE_LOGGER_H__

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
#include <dali/public-api/object/base-object.h>

// INTERNAL INCLUDES
#include <base/interfaces/performance-interface.h>
#include <performance-logger.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

class PerformanceLogger;

typedef IntrusivePtr<PerformanceLogger> PerformanceLoggerPtr;

/**
 * @brief Interface for the performance logger
 */
class PerformanceLogger : public BaseObject
{
public:

  /**
   * @brief Create a new logger
   *
   * @param[in] name The name of the logger. This needs to be a compile-time literal and alive for the whole lifetime of the performance logger.
   * @return a new logger
   */
  static PerformanceLoggerPtr New( const char* name );

  /**
   * Constructor
   * @param[in] name The name to assing to the logger
   */
  PerformanceLogger( const char* name );

  /**
   * Destructor.
   */
  virtual ~PerformanceLogger();

  /**
   * Add a performance marker
   *
   * @param markerType Performance marker type
   */
  void AddMarker( Dali::PerformanceLogger::Marker markerType );

  /**
   * Set the logging frequency
   *
   * @param logFrequency how often to log out in seconds
   */
  void SetLoggingFrequency( unsigned int logFrequency);

  /**
   * Set logging on or off for this logger
   *
   * @param[in] enable Enable logging or not
   */
  void EnableLogging( bool enable );

private: // Implementation

  // not implemented
  PerformanceLogger( const PerformanceLogger& );
  PerformanceLogger& operator=( const PerformanceLogger& );

private:

  PerformanceInterface::ContextId mContext;   ///< Context of this logger

};

// Helpers for public-api forwarding methods

inline static Internal::Adaptor::PerformanceLogger& GetImplementation( Dali::PerformanceLogger& logger )
{
  DALI_ASSERT_ALWAYS( logger && "PerformanceLogger handle is empty" );

  BaseObject& handle = logger.GetBaseObject();

  return static_cast< Internal::Adaptor::PerformanceLogger& >( handle );
}

inline static const  Internal::Adaptor::PerformanceLogger& GetImplementation( const Dali::PerformanceLogger& logger )
{
  DALI_ASSERT_ALWAYS( logger && "PerformanceLogger handle is empty" );

  const BaseObject& handle = logger.GetBaseObject();

  return static_cast< const Internal::Adaptor::PerformanceLogger& >( handle );
}

} // namespace Adaptor

} // namespace Internal

} // namespace Dali

#endif // __DALI_INTERNAL_PERFORMANCE_LOGGER_H__
