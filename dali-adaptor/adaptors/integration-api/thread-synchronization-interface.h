#ifndef __DALI_INTEGRATION_THREAD_SYNCHRONIZATION_INTERFACE_H__
#define __DALI_INTEGRATION_THREAD_SYNCHRONIZATION_INTERFACE_H__

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

/**
 * @brief Interface for the ThreadSyncrhonization handler.
 *
 * This handler ensure the threads are synchronized properly.
 */
class ThreadSynchronizationInterface
{
public:

  /////////////////////////////////////////////////////////////////////////////////////////////////
  // Called by the Event Thread if post-rendering is required
  /////////////////////////////////////////////////////////////////////////////////////////////////

  /**
   * @brief Surface informs that the post-render has been completed.
   *
   * @note Should only be called by the Event Thread if post-rendering is required.
   */
  virtual void PostRenderComplete() = 0;

  /////////////////////////////////////////////////////////////////////////////////////////////////
  // Called by the Render Thread if post-rendering is required
  /////////////////////////////////////////////////////////////////////////////////////////////////

  /**
   * @brief Called just before the post rendering is done by the surface.
   *
   * @note Should only be called by the Render Thread if post-rendering is required.
   */
  virtual void PostRenderStarted() = 0;

  /**
   * @brief Blocks the render thread until the post rendering has been completed by the surface.
   *
   * @note Should only be called by the Render Thread if post-rendering is required.
   */
  virtual void PostRenderWaitForCompletion() = 0;

protected:

  /**
   * @brief Protected constructor, no creation through this interface
   */
  ThreadSynchronizationInterface( ) { }

  /**
   * Virtual protected destructor, no deletion through this interface
   */
  virtual ~ThreadSynchronizationInterface() { }

private:

  // Undefined copy constructor.
  ThreadSynchronizationInterface( const ThreadSynchronizationInterface& );

  // Undefined assignment operator.
  ThreadSynchronizationInterface& operator=( const ThreadSynchronizationInterface& );
};

} // namespace Dali

#endif // __DALI_INTEGRATION_THREAD_SYNCHRONIZATION_INTERFACE_H__
