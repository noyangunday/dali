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

#include "utc-image-loading-common.h"

/** Live platform abstraction recreated for each test case. */
Integration::PlatformAbstraction * gAbstraction = 0;

/** A variety of parameters to reach different code paths in image loading code. */
std::vector<ImageParameters> gCancelAttributes;

void utc_dali_loading_startup(void)
{
  test_return_value = TET_UNDEF;
  gAbstraction = CreatePlatformAbstraction();

  // Setup some loading parameters to engage post-processing stages:

  ImageParameters scaleToFillAttributes;
  scaleToFillAttributes.second.first = FittingMode::SCALE_TO_FILL;
  scaleToFillAttributes.first = ImageDimensions( 160, 120 );
  gCancelAttributes.push_back( scaleToFillAttributes );

  // Hit the derived dimensions code:
  ImageParameters scaleToFillAttributesDeriveWidth = scaleToFillAttributes;
  scaleToFillAttributesDeriveWidth.first = ImageDimensions( 0, 120 );
  gCancelAttributes.push_back( scaleToFillAttributesDeriveWidth );

  ImageParameters scaleToFillAttributesDeriveHeight = scaleToFillAttributes;
  scaleToFillAttributesDeriveHeight.first = ImageDimensions( 160, 0 );
  gCancelAttributes.push_back( scaleToFillAttributesDeriveHeight );

  // Try to push a tall crop:
  ImageParameters scaleToFillAttributesTall = scaleToFillAttributes;
  scaleToFillAttributesTall.first = ImageDimensions( 160, 480 );
  ImageParameters scaleToFillAttributesTall2 = scaleToFillAttributes;
  scaleToFillAttributesTall2.first = ImageDimensions( 160, 509 );
  ImageParameters scaleToFillAttributesTall3 = scaleToFillAttributes;
  scaleToFillAttributesTall3.first = ImageDimensions( 37, 251 );
  gCancelAttributes.push_back( scaleToFillAttributesTall );
  gCancelAttributes.push_back( scaleToFillAttributesTall2 );
  gCancelAttributes.push_back( scaleToFillAttributesTall3 );

  // Try to push a wide crop:
  ImageParameters scaleToFillAttributesWide = scaleToFillAttributes;
  scaleToFillAttributesWide.first = ImageDimensions( 320, 60 );
  ImageParameters scaleToFillAttributesWide2 = scaleToFillAttributes;
  scaleToFillAttributesWide2.first = ImageDimensions( 317, 60 );
  ImageParameters scaleToFillAttributesWide3 = scaleToFillAttributes;
  scaleToFillAttributesWide3.first = ImageDimensions( 317, 53 );
  gCancelAttributes.push_back( scaleToFillAttributesWide );
  gCancelAttributes.push_back( scaleToFillAttributesWide2 );
  gCancelAttributes.push_back( scaleToFillAttributesWide3 );

  ImageParameters shrinkToFitAttributes = scaleToFillAttributes;
  shrinkToFitAttributes.second.first = FittingMode::SHRINK_TO_FIT;
  gCancelAttributes.push_back( shrinkToFitAttributes );

  ImageParameters fitWidthAttributes = scaleToFillAttributes;
  fitWidthAttributes.second.first = FittingMode::FIT_WIDTH;
  gCancelAttributes.push_back( fitWidthAttributes );

  ImageParameters fitHeightAttributes = scaleToFillAttributes;
  fitHeightAttributes.second.first = FittingMode::FIT_HEIGHT;
  gCancelAttributes.push_back( fitHeightAttributes );

  ///@ToDo: Add attribute variants for all scale modes.
  ///@ToDo: Add attribute variants for all filter modes.

  // Pad the array to a prime number to mitigate any accidental periodic
  // patterns in which image file has which attributes applied to its load:
  srand48( 104729 );
  const float lastUniques = gCancelAttributes.size() - 0.001f;
  while( gCancelAttributes.size() < 61u )
  {
    gCancelAttributes.push_back( gCancelAttributes[unsigned(drand48() * lastUniques)] );
  }
}

void utc_dali_loading_cleanup(void)
{
  delete gAbstraction;
  gAbstraction = 0;

  test_return_value = TET_PASS;
}
