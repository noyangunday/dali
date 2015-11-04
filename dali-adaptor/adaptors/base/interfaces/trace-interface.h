#ifndef __DALI_INTERNAL_BASE_TRACE_INTERFACE_H__
#define __DALI_INTERNAL_BASE_TRACE_INTERFACE_H__

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

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

/**
 * Abstract Tracing Interface.
 * Used to log trace messages.
 * E.g. On Linux this may use ftrace
 *
 */
class TraceInterface
{

public:

  /**
   * Write a trace message
   * @param marker performance marker
   * @param traceMessage trace message
   */
  virtual void Trace( const PerformanceMarker& marker, const std::string& traceMessage ) = 0;

protected:

  /**
   * Constructor
   */
  TraceInterface()
  {
  }

  /**
   * virtual destructor
   */
  virtual ~TraceInterface()
  {
  }

  // Undefined copy constructor.
  TraceInterface( const TraceInterface& );

  // Undefined assignment operator.
  TraceInterface& operator=( const TraceInterface& );
};

} // namespace Internal

} // namespace Adaptor

} // namespace Dali

#endif
