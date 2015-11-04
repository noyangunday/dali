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

#include <iostream>

#include <stdlib.h>
#include <stdint.h>
#include <dali/dali.h>
#include <dali-test-suite-utils.h>
#include <dali/internal/text-abstraction/font-client-helper.h>

using namespace Dali;

int UtcDaliFontClient(void)
{
  const int ORDERED_VALUES[] = { 50, 63, 75, 87, 100, 113, 125, 150, 200 };

  const unsigned int NUM_OF_ORDERED_VALUES = sizeof( ORDERED_VALUES ) / sizeof( int );

  TestApplication application;
  int preciseIndex = 0;
  int result=0;

  tet_infoline("UtcDaliFontClient center range");
  preciseIndex = 4;
  result = TextAbstraction::Internal::ValueToIndex( ORDERED_VALUES[preciseIndex], ORDERED_VALUES, NUM_OF_ORDERED_VALUES - 1u );
  DALI_TEST_EQUALS( preciseIndex, result, TEST_LOCATION );

  tet_infoline("UtcDaliFontClient start of range");
  preciseIndex = 0;
  result = TextAbstraction::Internal::ValueToIndex( ORDERED_VALUES[preciseIndex], ORDERED_VALUES, NUM_OF_ORDERED_VALUES - 1u );
  DALI_TEST_EQUALS( preciseIndex, result, TEST_LOCATION );

  tet_infoline("UtcDaliFontClient end of range");
  preciseIndex = 8;
  result = TextAbstraction::Internal::ValueToIndex( ORDERED_VALUES[preciseIndex], ORDERED_VALUES, NUM_OF_ORDERED_VALUES - 1u );
  DALI_TEST_EQUALS( preciseIndex, result, TEST_LOCATION );

  tet_infoline("UtcDaliFontClient below of range");
  result = TextAbstraction::Internal::ValueToIndex( 30, ORDERED_VALUES, NUM_OF_ORDERED_VALUES - 1u );
  DALI_TEST_EQUALS( 0, result, TEST_LOCATION );

  tet_infoline("UtcDaliFontClient below of range");
  result = TextAbstraction::Internal::ValueToIndex( 220, ORDERED_VALUES, NUM_OF_ORDERED_VALUES - 1u );
  DALI_TEST_EQUALS( 8, result, TEST_LOCATION );

  tet_infoline("UtcDaliFontClient zero ");
  result = TextAbstraction::Internal::ValueToIndex( 0, ORDERED_VALUES, NUM_OF_ORDERED_VALUES - 1u );
  DALI_TEST_EQUALS( 0, result, TEST_LOCATION );

  END_TEST;
}


