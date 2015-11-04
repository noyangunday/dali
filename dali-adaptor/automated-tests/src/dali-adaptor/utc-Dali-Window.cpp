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
#include <Ecore_X.h>
#include <devel-api/adaptor-framework/drag-and-drop-detector.h>
#include <dali-test-suite-utils.h>

using namespace Dali;

void utc_dali_window_startup(void)
{
  test_return_value = TET_UNDEF;
}

void utc_dali_window_cleanup(void)
{
  test_return_value = TET_PASS;
}

namespace
{

intptr_t screenId = 0; // intptr_t has the same size as a pointer and is platform independent so this can be returned as a pointer in ecore_x_default_screen_get below without compilation warnings

} // unnamed namespace

extern "C"
{

Ecore_X_Screen* ecore_x_default_screen_get(void)
{
  screenId += 8;
  return (Ecore_X_Screen*)screenId;
}

void ecore_x_screen_size_get(const Ecore_X_Screen *screen, int *w, int *h)
{
 *w = 100;
 *h = 100;
}

Ecore_X_Window ecore_x_window_argb_new(Ecore_X_Window parent, int x, int y, int w, int h)
{
  return 0;
}

}

int UtcDaliWindowConstructorP(void)
{
  Dali::Window window;
  DALI_TEST_CHECK( !window );
  END_TEST;
}

int UtcDaliWindowCopyConstructorP(void)
{
  Dali::Window window;
  Dali::Window copy( window );
  DALI_TEST_CHECK( copy == window );

  END_TEST;
}

int UtcDaliWindowConstructorFromInternalPointerN(void)
{
  Internal::Adaptor::Window* internalWindow = NULL;
  Dali::Window window(internalWindow);
  DALI_TEST_CHECK( !window ); // Should not reach here!

  END_TEST;
}

int UtcDaliWindowAssignmentOperatorP(void)
{
  const Dali::Window window;
  Dali::Window copy;
  DALI_TEST_CHECK( ! copy );
  copy = window;
  DALI_TEST_CHECK( copy == window );

  END_TEST;
}

int UtcDaliWindowDestructorP(void)
{
  Dali::Window* window = new Dali::Window();
  delete window;

  DALI_TEST_CHECK( true );
  END_TEST;
}

int UtcDaliWindowNewN(void)
{
  // Attempt to create a new window
  try
  {
    PositionSize windowPosition(0, 0, 0, 0);
    Dali::Window window = Dali::Window::New( windowPosition, "test-window", true );

    tet_result( TET_FAIL );
  }
  catch ( DaliException& e )
  {
    DALI_TEST_ASSERT( e, "Failed to create X window", TEST_LOCATION );
  }

  // Attempt to create a new window
  try
  {
    PositionSize windowPosition(0, 0, 0, 0);
    Dali::Window window = Dali::Window::New( windowPosition, "test-window", "test-window-class", true );

    tet_result( TET_FAIL );
  }
  catch ( DaliException& e )
  {
    DALI_TEST_ASSERT( e, "Failed to create X window", TEST_LOCATION );
  }

  END_TEST;
}

int UtcDaliWindowShowIndicatorN(void)
{
  Dali::Window window;
  try
  {
    window.ShowIndicator(Dali::Window::VISIBLE);
    DALI_TEST_CHECK( false ); // Should not reach here!
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliWindowSetIndicatorBgOpacityN(void)
{
  Dali::Window window;
  try
  {
    window.SetIndicatorBgOpacity(Dali::Window::OPAQUE);
    DALI_TEST_CHECK( false ); // Should not reach here!
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliWindowRotateIndicatorN(void)
{
  Dali::Window window;
  try
  {
    window.RotateIndicator(Dali::Window::PORTRAIT);
    DALI_TEST_CHECK( false ); // Should not reach here!
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliWindowSetClassN(void)
{
  Dali::Window window;
  try
  {
    window.SetClass("window-name", "window-class");
    DALI_TEST_CHECK( false ); // Should not reach here!
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliWindowRaiseN(void)
{
  Dali::Window window;
  try
  {
    window.Raise();
    DALI_TEST_CHECK( false ); // Should not reach here!
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliWindowLowerN(void)
{
  Dali::Window window;
  try
  {
    window.Lower();
    DALI_TEST_CHECK( false ); // Should not reach here!
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliWindowActivateN(void)
{
  Dali::Window window;
  try
  {
    window.Activate();
    DALI_TEST_CHECK( false ); // Should not reach here!
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliWindowAddAvailableOrientationN(void)
{
  Dali::Window window;
  try
  {
    window.AddAvailableOrientation(Dali::Window::PORTRAIT);
    DALI_TEST_CHECK( false ); // Should not reach here!
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliWindowRemoveAvailableOrientationN(void)
{
  Dali::Window window;
  try
  {
    window.RemoveAvailableOrientation(Dali::Window::PORTRAIT);
    DALI_TEST_CHECK( false ); // Should not reach here!
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliWindowSetPreferredOrientationN(void)
{
  Dali::Window window;
  try
  {
    window.SetPreferredOrientation(Dali::Window::PORTRAIT);
    DALI_TEST_CHECK( false ); // Should not reach here!
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliWindowGetPreferredOrientationN(void)
{
  Dali::Window window;
  try
  {
    Dali::Window::WindowOrientation orientation = window.GetPreferredOrientation();
    DALI_TEST_CHECK( orientation == Dali::Window::PORTRAIT ); // Should not reach here!
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliWindowGetDragAndDropDetectorN(void)
{
  Dali::Window window;
  try
  {
    DragAndDropDetector detector = window.GetDragAndDropDetector();
    DALI_TEST_CHECK( !detector ); // Should not reach here!
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliWindowGetNativeHandleN(void)
{
  Dali::Window window;
  try
  {
    Dali::Any handle = window.GetNativeHandle();
    DALI_TEST_CHECK( false ); // Should not reach here!
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliWindowIndicatorVisibilityChangedSignalN(void)
{
  Dali::Window window;
  try
  {
    window.IndicatorVisibilityChangedSignal();
    DALI_TEST_CHECK( false ); // Should not reach here!
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}




