#ifndef __DALI_INTERNAL_BASE_LIFECYCLE_OBSERVER_H__
#define __DALI_INTERNAL_BASE_LIFECYCLE_OBSERVER_H__

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

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

/**
 * Adaptor life cycle observer
 */
class LifeCycleObserver
{
public:
  /**
   * Called when the adaptor has started.
   */
  virtual void OnStart() = 0;

  /**
   * Called when the adaptor is about to pause.
   */
  virtual void OnPause() = 0;

  /**
   * Called when the adaptor is about to resume.
   */
  virtual void OnResume() = 0;

  /**
   * Called when the adaptor is about to stop.
   */
  virtual void OnStop() = 0;

  /**
   * Called when the adaptor is about to be destroyed.
   */
  virtual void OnDestroy() = 0;

protected:

  /**
   * Constructor
   */
  LifeCycleObserver()
  {
  }

  /**
   * Virtual destructor
   */
  virtual ~LifeCycleObserver()
  {
  }

private:

  // Undefined copy constructor.
  LifeCycleObserver( const LifeCycleObserver& );

  // Undefined assignment operator.
  LifeCycleObserver& operator=( const LifeCycleObserver& );

};



} // namespace Internal

} // namespace Adaptor

} // namespace Dali

#endif // __DALI_INTERNAL_BASE_LIFECYCLE_OBSERVER_H__
