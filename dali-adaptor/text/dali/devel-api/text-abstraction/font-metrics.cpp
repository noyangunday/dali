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
#include <dali/devel-api/text-abstraction/font-metrics.h>

namespace Dali
{

namespace TextAbstraction
{

FontMetrics::FontMetrics()
: ascender( 0.f ),
  descender( 0.f ),
  height( 0.f ),
  underlinePosition( 0.f ),
  underlineThickness( 0.f )
{
}

FontMetrics::FontMetrics( float ascenderPixels,
                          float descenderPixels,
                          float heightPixels,
                          float underlinePositionPixels,
                          float underlineThicknessPixels )
: ascender( ascenderPixels ),
  descender( descenderPixels ),
  height( heightPixels ),
  underlinePosition( underlinePositionPixels ),
  underlineThickness( underlineThicknessPixels )
{
}

} // namespace TextAbstraction

} // namespace Dali
