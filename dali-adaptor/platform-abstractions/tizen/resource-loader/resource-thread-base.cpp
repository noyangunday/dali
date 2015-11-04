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
#include <memory>
#include <dali/integration-api/debug.h>

// INTERNAL INCLUDES
#include "resource-thread-base.h"
#include "tizen-logging.h"
#include "atomics.h"

using namespace Dali::Integration;

namespace Dali
{

// Initial values for the members tracking which resources have been cancelled.
// They start out with different values so that if the first load executed is
// synchronous, it won't be erroneously cancelled.
const Integration::ResourceId NO_REQUEST_IN_FLIGHT = Integration::ResourceId(0) - 1;
const Integration::ResourceId NO_REQUEST_CANCELLED = Integration::ResourceId(0) - 2;

namespace TizenPlatform
{

namespace
{
const char * const IDLE_PRIORITY_ENVIRONMENT_VARIABLE_NAME = "DALI_RESOURCE_THREAD_IDLE_PRIORITY";
} // unnamed namespace

/** Thrown by InterruptionPoint() to abort a request early. */
class CancelRequestException {};

ResourceThreadBase::ResourceThreadBase( ResourceLoader& resourceLoader ) :
  mResourceLoader( resourceLoader ),
  mThread( 0 ),
  mCurrentRequestId( NO_REQUEST_IN_FLIGHT ),
  mCancelRequestId( NO_REQUEST_CANCELLED ),
  mPaused( false )
{
#if defined(DEBUG_ENABLED)
  mLogFilter = Debug::Filter::New(Debug::Concise, false, "LOG_RESOURCE_THREAD_BASE");
#endif

  int error = pthread_create( &mThread, NULL, InternalThreadEntryFunc, this );
  DALI_ASSERT_ALWAYS( !error && "Error in pthread_create()" );
}

ResourceThreadBase::~ResourceThreadBase()
{
  TerminateThread();

#if defined(DEBUG_ENABLED)
  delete mLogFilter;
#endif
}

void ResourceThreadBase::TerminateThread()
{
  if (mThread)
  {
    // wake thread
    mCondition.Notify();

    // wait for thread to exit
    pthread_join( mThread, NULL );

    mThread = 0;
  }
}

void ResourceThreadBase::AddRequest(const ResourceRequest& request, const RequestType type)
{
  bool wasEmpty = false;
  bool wasPaused = false;

  {
    // Lock while adding to the request queue
    ConditionalWait::ScopedLock lock( mCondition );

    wasEmpty = mQueue.empty();
    wasPaused = mPaused;

    mQueue.push_back( std::make_pair(request, type) );
  }

  if( wasEmpty && !wasPaused )
  {
    // Wake-up the thread
    mCondition.Notify();
  }
}

// Called from outer thread.
void ResourceThreadBase::CancelRequest( const Integration::ResourceId resourceId )
{
  bool found = false;
  DALI_LOG_INFO( mLogFilter, Debug::Verbose, "%s: %u.\n", __FUNCTION__, unsigned(resourceId) );

  // Eliminate the cancelled request from the request queue if it is in there:
  {
    // Lock while searching and removing from the request queue:
    ConditionalWait::ScopedLock lock( mCondition );

    for( RequestQueueIter iterator = mQueue.begin();
         iterator != mQueue.end();
         ++iterator )
    {
      if( ((*iterator).first).GetId() == resourceId )
      {
        iterator = mQueue.erase( iterator );
        found = true;
        break;
      }
    }
  }

  // Remember the cancelled id for the worker thread to poll at one of its points
  // of interruption:
  if( !found )
  {
    Dali::Internal::AtomicWriteToCacheableAlignedAddress( &mCancelRequestId, resourceId );
    DALI_LOG_INFO( mLogFilter, Debug::Concise, "%s: Cancelling in-flight resource (%u).\n", __FUNCTION__, unsigned(resourceId) );
  }
}

// Called from worker thread.
void ResourceThreadBase::InterruptionPoint() const
{
  const Integration::ResourceId cancelled = Dali::Internal::AtomicReadFromCacheableAlignedAddress( &mCancelRequestId );
  const Integration::ResourceId current = mCurrentRequestId;

  if( current == cancelled )
  {
    DALI_LOG_INFO( mLogFilter, Debug::Concise, "%s: Cancelled in-flight resource (%u).\n", __FUNCTION__, unsigned(cancelled) );
    throw CancelRequestException();
  }
}

void* ResourceThreadBase::InternalThreadEntryFunc( void* This )
{
    ( static_cast<ResourceThreadBase*>( This ) )->ThreadLoop();
    return NULL;
}

void ResourceThreadBase::Pause()
{
  ConditionalWait::ScopedLock lock( mCondition );
  mPaused = true;
}

void ResourceThreadBase::Resume()
{
  // Clear the paused flag and if we weren't running already, also wake up the background thread:
  bool wasPaused = false;
  {
    ConditionalWait::ScopedLock lock( mCondition );
    wasPaused = mPaused;
    mPaused = false;
  }

  // If we were paused, wake up the background thread and give it a
  // chance to do some work:
  if( wasPaused )
  {
    mCondition.Notify();
  }
}

//----------------- Called from separate thread (mThread) -----------------

void ResourceThreadBase::ThreadLoop()
{
  // TODO: Use Environment Options
  const char* threadPriorityIdleRequired = std::getenv( IDLE_PRIORITY_ENVIRONMENT_VARIABLE_NAME );
  if( threadPriorityIdleRequired )
  {
    // if the parameter exists then set up an idle priority for this thread
    struct sched_param sp;
    sp.sched_priority = 0;
    sched_setscheduler(0, SCHED_IDLE, &sp);
    ///@ToDo: change to the corresponding Pthreads call, not this POSIX.1-2001 one with a Linux-specific argument (SCHED_IDLE): int pthread_setschedparam(pthread_t thread, int policy, const struct sched_param *param);, as specified in the docs for sched_setscheduler(): http://man7.org/linux/man-pages/man2/sched_setscheduler.2.html
  }

  InstallLogging();

  while( !mResourceLoader.IsTerminating() )
  {
    try
    {
      WaitForRequests();

      if ( !mResourceLoader.IsTerminating() )
      {
        ProcessNextRequest();
      }
    }

    catch( CancelRequestException& ex )
    {
      // No problem: a derived class deliberately threw to abort an in-flight request
      // that was cancelled.
      DALI_LOG_INFO( mLogFilter, Debug::Concise, "%s: Caught cancellation exception for resource (%u).\n", __FUNCTION__, unsigned(mCurrentRequestId) );
      CancelRequestException* disableUnusedVarWarning = &ex;
      ex = *disableUnusedVarWarning;
    }

    // Catch all exceptions to avoid killing the process, and log the error:
    catch( std::exception& ex )
    {
      const char * const what = ex.what();
      DALI_LOG_ERROR( "std::exception caught in resource thread. Aborting request with id %u because of std::exception with reason, \"%s\".\n", unsigned(mCurrentRequestId), what ? what : "null" );
    }
    catch( Dali::DaliException& ex )
    {
      // Probably a failed assert-always:
      DALI_LOG_ERROR( "DaliException caught in resource thread. Aborting request with id %u. Location: \"%s\". Condition: \"%s\".\n", unsigned(mCurrentRequestId), ex.location, ex.condition );
    }
    catch( ... )
    {
      DALI_LOG_ERROR( "Unknown exception caught in resource thread. Aborting request with id %u.\n", unsigned(mCurrentRequestId) );
    }
  }
}

void ResourceThreadBase::WaitForRequests()
{
  ConditionalWait::ScopedLock lock( mCondition );

  if( mQueue.empty() || mPaused == true )
  {
    // Waiting for a wake up from resource loader control thread
    // This will be to process a new request or terminate
    mCondition.Wait( lock );
  }
}

void ResourceThreadBase::ProcessNextRequest()
{
  ResourceRequest* request(NULL);
  RequestType type(RequestLoad);

  {
    // lock the queue and extract the next request
    ConditionalWait::ScopedLock lock( mCondition );

    if (!mQueue.empty())
    {
      const RequestInfo & front = mQueue.front();
      request = new ResourceRequest( front.first );
      type = front.second;
      mCurrentRequestId = front.first.GetId();
      mQueue.pop_front();
    }
  } // unlock the queue

  // process request outside of lock
  if ( NULL != request )
  {
    std::auto_ptr<ResourceRequest> deleter( request );
    switch( type )
    {
      case RequestLoad:
      {
        Load(*request);
      }
      break;

      case RequestDownload:
      {
        Download(*request);
      }
      break;

      case RequestDecode:
      {
        Decode(*request);
      }
      break;
    }
  }
}

void ResourceThreadBase::InstallLogging()
{
  // resource loading thread will send its logs to TIZEN Platform's LogMessage handler.
  Dali::Integration::Log::InstallLogFunction(Dali::TizenPlatform::LogMessage);
}

void ResourceThreadBase::UninstallLogging()
{
  // uninstall it on resource loading thread.
  Dali::Integration::Log::UninstallLogFunction();
}

void ResourceThreadBase::Download(const Integration::ResourceRequest& request)
{
  DALI_LOG_TRACE_METHOD(mLogFilter);
  DALI_LOG_WARNING("Resource Downloading from a remote server not supported for this type.");
  ///! If you need this for a subclassed thread, look to ResourceThreadImage::Download() for an example implementation.
}

void ResourceThreadBase::Decode(const Integration::ResourceRequest& request)
{
  DALI_LOG_TRACE_METHOD(mLogFilter);
  DALI_LOG_WARNING("Resource Decoding from a memory buffer not supported for this type.");
  ///! If you need this for a subclassed thread, look to ResourceThreadImage::Decode() for an example implementation.
}

} // namespace TizenPlatform

} // namespace Dali
