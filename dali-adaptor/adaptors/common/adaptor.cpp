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

// CLASS HEADER
#include <adaptor.h>

// EXTERNAL INCLUDES
#include <dali/integration-api/debug.h>

// INTERNAL INCLUDES
#include <accessibility-adaptor.h>
#include <imf-manager.h>
#include <style-monitor.h>
#include <render-surface.h>
#include <adaptor-impl.h>

namespace Dali
{

Adaptor& Adaptor::New( Window window )
{
  return New( window, Configuration::APPLICATION_DOES_NOT_HANDLE_CONTEXT_LOSS );
}

Adaptor& Adaptor::New( Window window, Configuration::ContextLoss configuration )
{
  Adaptor* adaptor = Internal::Adaptor::Adaptor::New( window, configuration, NULL );
  return *adaptor;
}

Adaptor& Adaptor::New( Any nativeWindow, const Dali::RenderSurface& surface )
{
  return New( nativeWindow, surface, Configuration::APPLICATION_DOES_NOT_HANDLE_CONTEXT_LOSS );
}

Adaptor& Adaptor::New( Any nativeWindow, const Dali::RenderSurface& surface, Configuration::ContextLoss configuration )
{
  Dali::RenderSurface* pSurface = const_cast<Dali::RenderSurface *>(&surface);
  Adaptor* adaptor = Internal::Adaptor::Adaptor::New( nativeWindow, pSurface, configuration, NULL );
  return *adaptor;
}

Adaptor::~Adaptor()
{
  delete mImpl;
}

void Adaptor::Start()
{
  mImpl->Start();
}

void Adaptor::Pause()
{
  mImpl->Pause();
}

void Adaptor::Resume()
{
  mImpl->Resume();
}

void Adaptor::Stop()
{
  mImpl->Stop();
}

bool Adaptor::AddIdle( CallbackBase* callback )
{
  return mImpl->AddIdle( callback );
}

void Adaptor::ReplaceSurface( Any nativeWindow, Dali::RenderSurface& surface )
{
  mImpl->ReplaceSurface(nativeWindow, surface);
}

Adaptor::AdaptorSignalType& Adaptor::ResizedSignal()
{
  return mImpl->ResizedSignal();
}

Adaptor::AdaptorSignalType& Adaptor::LanguageChangedSignal()
{
  return mImpl->LanguageChangedSignal();
}

RenderSurface& Adaptor::GetSurface()
{
  return mImpl->GetSurface();
}

void Adaptor::ReleaseSurfaceLock()
{
  mImpl->ReleaseSurfaceLock();
}

void Adaptor::SetRenderRefreshRate( unsigned int numberOfVSyncsPerRender )
{
  mImpl->SetRenderRefreshRate( numberOfVSyncsPerRender );
}

void Adaptor::SetUseHardwareVSync(bool useHardware)
{
  mImpl->SetUseHardwareVSync( useHardware );
}

Adaptor& Adaptor::Get()
{
  return Internal::Adaptor::Adaptor::Get();
}

bool Adaptor::IsAvailable()
{
  return Internal::Adaptor::Adaptor::IsAvailable();
}

void Adaptor::NotifySceneCreated()
{
  mImpl->NotifySceneCreated();
}

void Adaptor::NotifyLanguageChanged()
{
  mImpl->NotifyLanguageChanged();
}

void Adaptor::SetMinimumPinchDistance(float distance)
{
  mImpl->SetMinimumPinchDistance(distance);
}

void Adaptor::FeedTouchPoint( TouchPoint& point, int timeStamp )
{
  mImpl->FeedTouchPoint(point, timeStamp);
}

void Adaptor::FeedWheelEvent( WheelEvent& wheelEvent )
{
  mImpl->FeedWheelEvent(wheelEvent);
}

void Adaptor::FeedKeyEvent( KeyEvent& keyEvent )
{
  mImpl->FeedKeyEvent(keyEvent);
}

void Adaptor::SceneCreated()
{
  mImpl->SceneCreated();
}

void Adaptor::SetViewMode( ViewMode mode )
{
  mImpl->SetViewMode( mode );
}

void Adaptor::SetStereoBase(  float stereoBase )
{
  mImpl->SetStereoBase( stereoBase );
}


Adaptor::Adaptor()
: mImpl( NULL )
{
}

} // namespace Dali
