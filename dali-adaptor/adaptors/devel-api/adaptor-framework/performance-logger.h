#ifndef __DALI_PERFORMANCE_LOGGER_H__
#define __DALI_PERFORMANCE_LOGGER_H__

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

namespace Dali
{

namespace Internal DALI_INTERNAL
{
namespace Adaptor
{
class PerformanceLogger;
}
}

/**
 * @brief Performance logger class
 */
class DALI_IMPORT_API PerformanceLogger : public BaseHandle
{
public:

  /**
   * Enum for events that can be logged
   */
  enum Marker
  {
    START_EVENT,      ///< The start of timing
    END_EVENT         ///< The end of timing
  };

  /**
   * @brief Constructor, creates an uninitialized logger.
   *
   * Call New to fully construct a logger.
   */
  PerformanceLogger();

  /**
   * @brief Create a new logger
   *
   * @param[in] name The name of the logger. This needs to be a compile-time literal and alive for the whole lifetime of the performance logger.
   * @return a new logger
   */
  static PerformanceLogger New( const char* name );

  /**
   * @brief Copy constructor.
   *
   * @param[in] logger The handle to copy. The copied handle will point at the same implementation
   */
  PerformanceLogger( const PerformanceLogger& logger );

  /**
   * @brief Assignment operator.
   *
   * @param[in] logger The handle to copy. This handle will point at the same implementation
   * as the copied handle.
   * @return Reference to this logger handle
   */
  PerformanceLogger& operator=( const PerformanceLogger& logger );

  /**
   * @brief Destructor
   *
   * This is non-virtual since derived Handle types must not contain data or virtual methods.
   */
  ~PerformanceLogger();

  /**
   * @brief Downcast an Object handle to PerformanceLogger handle.
   *
   * If handle points to a PerformanceLogger object the downcast produces a valid
   * handle. If not the returned handle is left uninitialized.
   *
   * @param[in] handle to An object
   * @return handle to a PerformanceLogger object or an uninitialized handle
   */
  static PerformanceLogger DownCast( BaseHandle handle );

  /**
   * Add a performance marker
   *
   * @param markerType Performance marker type
   */
  void AddMarker( Marker markerType );

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

  // Not intended for application developers

  /**
   * Creates a new handle from the implementation.
   * @param[in] impl A pointer to the object.
   */
  explicit DALI_INTERNAL PerformanceLogger( Internal::Adaptor::PerformanceLogger* impl );

};

} // namespace Dali

#endif // __DALI_PERFORMANCE_LOGGER_H__
