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

// CLASS HEADER
#include "thread-controller.h"

// INTERNAL INCLUDES
#include <base/update-thread.h>
#include <base/render-thread.h>
#include <base/thread-synchronization.h>
#include <base/vsync-notifier.h>
#include <base/interfaces/adaptor-internal-services.h>
#include <base/environment-options.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

ThreadController::ThreadController( AdaptorInternalServices& adaptorInterfaces, const EnvironmentOptions& environmentOptions )
: mAdaptorInterfaces( adaptorInterfaces ),
  mUpdateThread( NULL ),
  mRenderThread( NULL ),
  mVSyncNotifier( NULL ),
  mThreadSync( NULL ),
  mNumberOfVSyncsPerRender( 1 )
{
  mThreadSync = new ThreadSynchronization( adaptorInterfaces, mNumberOfVSyncsPerRender );

  mUpdateThread = new UpdateThread( *mThreadSync, adaptorInterfaces, environmentOptions );

  mRenderThread = new RenderThread( *mThreadSync, adaptorInterfaces, environmentOptions );

  mVSyncNotifier = new VSyncNotifier( *mThreadSync, adaptorInterfaces, environmentOptions );

  // Set the thread-synchronization interface on the render-surface
  RenderSurface* currentSurface = mAdaptorInterfaces.GetRenderSurfaceInterface();
  if( currentSurface )
  {
    currentSurface->SetThreadSynchronization( *mThreadSync );
  }
}

ThreadController::~ThreadController()
{
  delete mVSyncNotifier;
  delete mRenderThread;
  delete mUpdateThread;
  delete mThreadSync;
}

void ThreadController::Initialize()
{
  // Notify the synchronization object before starting the threads
  mThreadSync->Initialise();

  // We want to the threads to be set up before they start
  mUpdateThread->Start();
  mRenderThread->Start();
  mVSyncNotifier->Start();
}

void ThreadController::Start()
{
  mThreadSync->Start();
}

void ThreadController::Pause()
{
  mThreadSync->Pause();
}

void ThreadController::Resume()
{
  mThreadSync->Resume();
}

void ThreadController::Stop()
{
  // Notify the synchronization object before stopping the threads
  mThreadSync->Stop();

  mVSyncNotifier->Stop();
  mUpdateThread->Stop();
  mRenderThread->Stop();
}

void ThreadController::RequestUpdate()
{
  mThreadSync->UpdateRequest();
}

void ThreadController::RequestUpdateOnce()
{
  // if we are paused, need to allow one update
  mThreadSync->UpdateOnce();
}

void ThreadController::ReplaceSurface( RenderSurface* newSurface )
{
  // Set the thread-syncronization on the new surface
  newSurface->SetThreadSynchronization( *mThreadSync );

  // tell render thread to start the replace. This call will block until the replace
  // has completed.
  RenderSurface* currentSurface = mAdaptorInterfaces.GetRenderSurfaceInterface();

  // Ensure the current surface releases any locks to prevent deadlock.
  currentSurface->StopRender();

  mThreadSync->ReplaceSurface( newSurface );
}

void ThreadController::SetRenderRefreshRate(unsigned int numberOfVSyncsPerRender )
{
  mNumberOfVSyncsPerRender = numberOfVSyncsPerRender;
  mThreadSync->SetRenderRefreshRate(numberOfVSyncsPerRender);
}

} // namespace Adaptor

} // namespace Internal

} // namespace Dali
