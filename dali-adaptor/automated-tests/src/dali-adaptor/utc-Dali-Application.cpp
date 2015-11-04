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

#include <dali/dali.h>
#include <dali-test-suite-utils.h>

using namespace Dali;

void utc_dali_application_startup(void)
{
  test_return_value = TET_UNDEF;
}

void utc_dali_application_cleanup(void)
{
  test_return_value = TET_PASS;
}

namespace
{

struct MyTestApp : public ConnectionTracker
{
  MyTestApp( Application& app)
  : initCalled( false ),
    application( app )
  {
    application.InitSignal().Connect( this, &MyTestApp::Create );
  }

  void Create(Application& app)
  {
    initCalled = true;
  }

  void Quit()
  {
    application.Quit();
  }

  // Data
  bool initCalled;
  Application& application;
};

void ApplicationSignalCallback( Application& app )
{
}

void ApplicationControlSignalCallback(Application&, void *)
{
}

} // unnamed namespace

int UtcDaliApplicationNew01(void)
{
  Application application = Application::New();

  MyTestApp testApp( application );

  DALI_TEST_CHECK( application );

  END_TEST;
}

int UtcDaliApplicationNew02(void)
{
  int argc( 1 );
  const char* argList[1] = { "program" };
  char** argv = const_cast<char**>(argList);

  Application application = Application::New( &argc, &argv );

  MyTestApp testApp( application );

  DALI_TEST_CHECK( application );

  END_TEST;
}

int UtcDaliApplicationNew03(void)
{
  int argc( 1 );
  const char* argList[1] = { "program" };
  char** argv = const_cast<char**>(argList);

  Application application = Application::New( &argc, &argv, "stylesheet" );

  MyTestApp testApp( application );

  DALI_TEST_CHECK( application );

  END_TEST;
}

int UtcDaliApplicationNew04(void)
{
  int argc( 1 );
  const char* argList[1] = { "program" };
  char** argv = const_cast<char**>(argList);

  Application application = Application::New( &argc, &argv, "stylesheet", Application::TRANSPARENT );

  MyTestApp testApp( application );

  DALI_TEST_CHECK( application );

  END_TEST;
}

int UtcDaliApplicationCopyAndAssignment(void)
{
  Application application = Application::New();
  Application copy( application );
  DALI_TEST_CHECK( copy == application );

  Application assigned;
  DALI_TEST_CHECK( !assigned );
  assigned = application;
  DALI_TEST_CHECK( copy == assigned );

  END_TEST;
}

int UtcDaliApplicationMainLoop01N(void)
{
  Application application;

  try
  {
    application.MainLoop();
    DALI_TEST_CHECK( false ); // Should not get here
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliApplicationMainLoop02N(void)
{
  Application application;

  try
  {
    application.MainLoop( Configuration::APPLICATION_DOES_NOT_HANDLE_CONTEXT_LOSS );
    DALI_TEST_CHECK( false ); // Should not get here
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliApplicationLowerN(void)
{
  Application application;

  try
  {
    application.Lower();
    DALI_TEST_CHECK( false ); // Should not get here
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliApplicationQuitN(void)
{
  Application application;

  try
  {
    application.Quit();
    DALI_TEST_CHECK( false ); // Should not get here
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliApplicationAddIdleN(void)
{
  Application application;

  try
  {
    CallbackBase* callback = NULL;
    application.AddIdle( callback );
    DALI_TEST_CHECK( false ); // Should not get here
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliApplicationGetWindowN(void)
{
  Application application;

  try
  {
    (void) application.GetWindow();
    DALI_TEST_CHECK( false ); // Should not get here
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliApplicationReplaceWindowN(void)
{
  Application application;

  try
  {
    application.ReplaceWindow( PositionSize(), "window" );
    DALI_TEST_CHECK( false ); // Should not get here
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliApplicationSetViewModeN(void)
{
  Application application;

  try
  {
    application.SetViewMode( STEREO_VERTICAL );
    DALI_TEST_CHECK( false ); // Should not get here
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliApplicationGetViewModeN(void)
{
  Application application;

  try
  {
    (void) application.GetViewMode();
    DALI_TEST_CHECK( false ); // Should not get here
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliApplicationSetStereoBaseN(void)
{
  Application application;

  try
  {
    application.SetStereoBase( 1.0f );
    DALI_TEST_CHECK( false ); // Should not get here
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliApplicationGetStereoBaseN(void)
{
  Application application;

  try
  {
    (void) application.GetStereoBase();
    DALI_TEST_CHECK( false ); // Should not get here
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliApplicationInitSignalP(void)
{
  Application application = Application::New();
  application.InitSignal().Connect( &ApplicationSignalCallback );
  DALI_TEST_CHECK( application );

  END_TEST;
}

int UtcDaliApplicationInitSignalN(void)
{
  Application application;

  try
  {
    application.InitSignal().Connect( &ApplicationSignalCallback );
    DALI_TEST_CHECK( false ); // Should not get here
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliApplicationTerminateSignalP(void)
{
  Application application = Application::New();
  application.TerminateSignal().Connect( &ApplicationSignalCallback );
  DALI_TEST_CHECK( application );

  END_TEST;
}

int UtcDaliApplicationTerminateSignalN(void)
{
  Application application;

  try
  {
    application.TerminateSignal().Connect( &ApplicationSignalCallback );
    DALI_TEST_CHECK( false ); // Should not get here
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliApplicationPauseSignalP(void)
{
  Application application = Application::New();
  application.PauseSignal().Connect( &ApplicationSignalCallback );
  DALI_TEST_CHECK( application );

  END_TEST;
}

int UtcDaliApplicationPauseSignalN(void)
{
  Application application;

  try
  {
    application.PauseSignal().Connect( &ApplicationSignalCallback );
    DALI_TEST_CHECK( false ); // Should not get here
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliApplicationResumeSignalP(void)
{
  Application application = Application::New();
  application.ResumeSignal().Connect( &ApplicationSignalCallback );
  DALI_TEST_CHECK( application );

  END_TEST;
}

int UtcDaliApplicationResumeSignalN(void)
{
  Application application;

  try
  {
    application.ResumeSignal().Connect( &ApplicationSignalCallback );
    DALI_TEST_CHECK( false ); // Should not get here
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliApplicationResetSignalP(void)
{
  Application application = Application::New();
  application.ResetSignal().Connect( &ApplicationSignalCallback );
  DALI_TEST_CHECK( application );

  END_TEST;
}

int UtcDaliApplicationResetSignalN(void)
{
  Application application;

  try
  {
    application.ResetSignal().Connect( &ApplicationSignalCallback );
    DALI_TEST_CHECK( false ); // Should not get here
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliApplicationResizeSignalP(void)
{
  Application application = Application::New();
  application.ResizeSignal().Connect( &ApplicationSignalCallback );
  DALI_TEST_CHECK( application );

  END_TEST;
}

int UtcDaliApplicationResizeSignalN(void)
{
  Application application;

  try
  {
    application.ResizeSignal().Connect( &ApplicationSignalCallback );
    DALI_TEST_CHECK( false ); // Should not get here
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliApplicationlControlSignalP(void)
{
  Application application = Application::New();
  application.AppControlSignal().Connect( &ApplicationControlSignalCallback );
  DALI_TEST_CHECK( application );

  END_TEST;
}

int UtcDaliApplicationlControlSignalN(void)
{
  Application application;

  try
  {
    application.AppControlSignal().Connect( &ApplicationControlSignalCallback );
    DALI_TEST_CHECK( false ); // Should not get here
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliApplicationLanguageChangedSignalP(void)
{
  Application application = Application::New();
  application.LanguageChangedSignal().Connect( &ApplicationSignalCallback );
  DALI_TEST_CHECK( application );

  END_TEST;
}

int UtcDaliApplicationLanguageChangedSignalN(void)
{
  Application application;

  try
  {
    application.LanguageChangedSignal().Connect( &ApplicationSignalCallback );
    DALI_TEST_CHECK( false ); // Should not get here
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliApplicationRegionChangedSignalP(void)
{
  Application application = Application::New();
  application.RegionChangedSignal().Connect( &ApplicationSignalCallback );
  DALI_TEST_CHECK( application );

  END_TEST;
}

int UtcDaliApplicationRegionChangedSignalN(void)
{
  Application application;

  try
  {
    application.RegionChangedSignal().Connect( &ApplicationSignalCallback );
    DALI_TEST_CHECK( false ); // Should not get here
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliApplicationBatteryLowSignalP(void)
{
  Application application = Application::New();
  application.BatteryLowSignal().Connect( &ApplicationSignalCallback );
  DALI_TEST_CHECK( application );

  END_TEST;
}

int UtcDaliApplicationBatteryLowSignalN(void)
{
  Application application;

  try
  {
    application.BatteryLowSignal().Connect( &ApplicationSignalCallback );
    DALI_TEST_CHECK( false ); // Should not get here
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliApplicationMemoryLowSignalP(void)
{
  Application application = Application::New();
  application.MemoryLowSignal().Connect( &ApplicationSignalCallback );
  DALI_TEST_CHECK( application );

  END_TEST;
}

int UtcDaliApplicationMemoryLowSignalN(void)
{
  Application application;

  try
  {
    application.MemoryLowSignal().Connect( &ApplicationSignalCallback );
    DALI_TEST_CHECK( false ); // Should not get here
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}
