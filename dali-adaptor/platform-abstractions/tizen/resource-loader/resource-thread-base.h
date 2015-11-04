#ifndef __DALI_TIZEN_PLATFORM_RESOURCE_THREAD_BASE_H__
#define __DALI_TIZEN_PLATFORM_RESOURCE_THREAD_BASE_H__

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
#include <deque>
#include <dali/devel-api/threading/conditional-wait.h>

// INTERNAL INCLUDES
#include "resource-loader.h"
#include "resource-loading-client.h"

namespace Dali
{

namespace TizenPlatform
{

/**
 * Resource loader worker thread
 */
class ResourceThreadBase : public ResourceLoadingClient
{
public:
  // typedefs and enums

  /** Client threads send work to resource threads through Requests, for which
   *  this type identifies the action to be taken on the resource thread. */
  enum RequestType
  {
    /** Pull a resource out of the platform's file system. */
    RequestLoad,
    /** Pull a resource from the network. */
    RequestDownload,
    /** Pull a resource out of a memory buffer. */
    RequestDecode
  };

  typedef std::pair<Integration::ResourceRequest, RequestType>  RequestInfo;
  typedef std::deque<RequestInfo>                               RequestQueue;
  typedef RequestQueue::iterator                                RequestQueueIter;

public:
  // Constructor
  ResourceThreadBase(ResourceLoader& resourceLoader);

  // Destructor
  virtual ~ResourceThreadBase();

protected:
  void TerminateThread();

public:
  /**
   * Add a resource request to the back of the queue
   * @param[in] request The requested resource/file url and attributes
   * @param[in] type    Load or save flag
   */
  void AddRequest(const Integration::ResourceRequest& request, const RequestType type);

  /**
   * Cancel a resource request. Removes the request from the queue.
   * @param[in] resourceId ID of the resource to be canceled
   */
  void CancelRequest(Integration::ResourceId  resourceId);

  /**
   * Pause starting new work in the background, but keep that work queued.
   */
  void Pause();

  /**
   * Resume processing tasks on background thread.
   */
  void Resume();


protected:
  /**
   * Main control loop for the thread.
   * The thread is terminated when this function exits
   */
  void ThreadLoop();

  /**
   * Wait for an incoming resource request or termination
   */
  void WaitForRequests();

  /**
   * Process the resource request at the head of the queue
   */
  void ProcessNextRequest();

  /**
   * Install a logging function in to core for this thread.
   */
  void InstallLogging();

  /**
   * Uninstall a logging function.
   */
  void UninstallLogging();

  /**
   * Load a resource
   * @param[in] request  The requested resource/file url and attributes
   */
  virtual void Load(const Integration::ResourceRequest& request) = 0;

  /**
   * Download a resource
   * @param[in] request  The requested resource/file url and attributes
   */
  virtual void Download(const Integration::ResourceRequest& request);

  /**
   * Decode a resource exactly as if it were being loaded but source its data
   * from a memory buffer attached directly to the request object.
   * @param[in] request  The requested resource data and attributes
   */
  virtual void Decode(const Integration::ResourceRequest& request);

  /**
   * @brief Cancels current resource request if it matches the one latched to be cancelled.
   *
   * @copydoc ResourceLoadingClient::InterruptionPoint
   */
  virtual void InterruptionPoint() const;

private:
  /**
   * Helper for the thread calling the entry function
   * @param[in] This A pointer to the current UpdateThread object
   */
  static void* InternalThreadEntryFunc( void* This );

protected:
  ResourceLoader&                    mResourceLoader;
  pthread_t                          mThread;    ///< thread instance
  ConditionalWait                    mCondition; ///< condition variable
  RequestQueue                       mQueue;     ///< Request queue
private:
  Integration::ResourceId          mCurrentRequestId; ///< Current request, set by worker thread
  volatile Integration::ResourceId mCancelRequestId;  ///< Request to be cancelled on thread: written by external thread and read by worker.
  bool                             mPaused;           ///< Whether to process work in mQueue

private:

  // Undefined
  ResourceThreadBase( const ResourceThreadBase& resourceThreadBase );

  // Undefined
  ResourceThreadBase& operator=( const ResourceThreadBase& resourceThreadBase );

#if defined(DEBUG_ENABLED)
public:
  Integration::Log::Filter* mLogFilter;
#endif
}; // class ResourceThreadBase

} // namespace TizenPlatform

} // namespace Dali

#endif // __DALI_TIZEN_PLATFORM_RESOURCE_THREAD_BASE_H__
