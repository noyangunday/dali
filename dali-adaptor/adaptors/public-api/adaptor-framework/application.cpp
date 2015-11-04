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
#include "application.h"

// EXTERNAL INCLUDES
#include <dali/integration-api/debug.h>

// INTERNAL INCLUDES
#include <application-impl.h>

namespace Dali
{

Application Application::New()
{
  return New( NULL, NULL );
}

Application Application::New( int* argc, char **argv[] )
{
  Internal::Adaptor::ApplicationPtr internal = Internal::Adaptor::Application::New( argc, argv, "", OPAQUE );
  return Application(internal.Get());
}

Application Application::New( int* argc, char **argv[], const std::string& stylesheet )
{
  Internal::Adaptor::ApplicationPtr internal = Internal::Adaptor::Application::New( argc, argv, stylesheet, OPAQUE );
  return Application(internal.Get());
}

Application Application::New( int* argc, char **argv[], const std::string& stylesheet, WINDOW_MODE windowMode )
{
  Internal::Adaptor::ApplicationPtr internal = Internal::Adaptor::Application::New( argc, argv, stylesheet, windowMode );
  return Application(internal.Get());
}

Application::~Application()
{
}

Application::Application()
{
}

Application::Application(const Application& application)
: BaseHandle(application)
{
}

Application& Application::operator=(const Application& application)
{
  if( *this != application )
  {
    BaseHandle::operator=( application );
  }
  return *this;
}

void Application::MainLoop()
{
  Internal::Adaptor::GetImplementation(*this).MainLoop(Configuration::APPLICATION_HANDLES_CONTEXT_LOSS);
}

void Application::MainLoop(Configuration::ContextLoss configuration)
{
  Internal::Adaptor::GetImplementation(*this).MainLoop(configuration);
}

void Application::Lower()
{
  Internal::Adaptor::GetImplementation(*this).Lower();
}

void Application::Quit()
{
  Internal::Adaptor::GetImplementation(*this).Quit();
}

bool Application::AddIdle( CallbackBase* callback )
{
  return Internal::Adaptor::GetImplementation(*this).AddIdle( callback );
}

Window Application::GetWindow()
{
  return Internal::Adaptor::GetImplementation(*this).GetWindow();
}

void Application::ReplaceWindow(PositionSize windowPosition, const std::string& name)
{
  Internal::Adaptor::GetImplementation(*this).ReplaceWindow(windowPosition, name);
}

void Application::SetViewMode( ViewMode viewMode )
{
  Internal::Adaptor::GetImplementation(*this).SetViewMode( viewMode );
}

ViewMode Application::GetViewMode() const
{
  return Internal::Adaptor::GetImplementation(*this).GetViewMode();
}

void Application::SetStereoBase( float stereoBase )
{
  Internal::Adaptor::GetImplementation(*this).SetStereoBase( stereoBase );
}

float Application::GetStereoBase() const
{
  return Internal::Adaptor::GetImplementation(*this).GetStereoBase();
}

Application::AppSignalType& Application::InitSignal()
{
  return Internal::Adaptor::GetImplementation(*this).InitSignal();
}

Application::AppSignalType& Application::TerminateSignal()
{
  return Internal::Adaptor::GetImplementation(*this).TerminateSignal();
}

Application::AppSignalType& Application::PauseSignal()
{
  return Internal::Adaptor::GetImplementation(*this).PauseSignal();
}

Application::AppSignalType& Application::ResumeSignal()
{
  return Internal::Adaptor::GetImplementation(*this).ResumeSignal();
}

Application::AppSignalType& Application::ResetSignal()
{
  return Internal::Adaptor::GetImplementation(*this).ResetSignal();
}

Application::AppSignalType& Application::ResizeSignal()
{
  return Internal::Adaptor::GetImplementation(*this).ResizeSignal();
}

Application::AppControlSignalType & Application::AppControlSignal()
{
  return Internal::Adaptor::GetImplementation(*this).AppControlSignal();
}

Application::AppSignalType& Application::LanguageChangedSignal()
{
  return Internal::Adaptor::GetImplementation(*this).LanguageChangedSignal();
}

Application::AppSignalType& Application::RegionChangedSignal()
{
  return Internal::Adaptor::GetImplementation(*this).RegionChangedSignal();
}

Application::AppSignalType& Application::BatteryLowSignal()
{
  return Internal::Adaptor::GetImplementation(*this).BatteryLowSignal();
}

Application::AppSignalType& Application::MemoryLowSignal()
{
  return Internal::Adaptor::GetImplementation(*this).MemoryLowSignal();
}

Application::Application(Internal::Adaptor::Application* application)
: BaseHandle(application)
{
}


} // namespace Dali
