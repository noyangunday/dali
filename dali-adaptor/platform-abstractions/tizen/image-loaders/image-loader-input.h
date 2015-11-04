#ifndef __DALI_TIZEN_PLATFORM_IMAGE_LOADER_INPUT_H__
#define __DALI_TIZEN_PLATFORM_IMAGE_LOADER_INPUT_H__

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
 */

// EXTERNAL INCLUDES
#include <cstdio>
#include <dali/public-api/images/image-operations.h>

// INTERNAL INCLUDES

namespace Dali
{
namespace TizenPlatform
{
namespace ImageLoader
{


/**
 * @brief A simple immutable struct to bundle together parameters for scaling an image.
 * @ToDo Move this to adaptor internal?
 * @ToDo Rename it after the move to ImageScalingParameters.
 */
class ScalingParameters
{
public:
  ScalingParameters( ImageDimensions dimensions = ImageDimensions(), FittingMode::Type fittingMode = FittingMode::DEFAULT, SamplingMode::Type samplingMode = SamplingMode::DEFAULT ) :
    dimensions(dimensions), scalingMode(fittingMode), samplingMode(samplingMode) {}
  const ImageDimensions dimensions;
  const FittingMode::Type scalingMode;
  const SamplingMode::Type samplingMode;
};

  /**
   * @brief Bundle-up the data pushed into an image loader.
   */
struct Input
{
  Input( FILE* file, ScalingParameters scalingParameters = ScalingParameters(), bool reorientationRequested = true ) :
    file(file), scalingParameters(scalingParameters), reorientationRequested(reorientationRequested) {}
  FILE* file;
  ScalingParameters scalingParameters;
  bool reorientationRequested;
};

} // ImageLoader
} // TizenPlatform
} // Dali

#endif // __DALI_TIZEN_PLATFORM_IMAGE_LOADER_INPUT_H__
