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
#include "color-controller-impl.h"

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

Dali::ColorController ColorController::Get()
{
  return Dali::ColorController( new ColorController() );
}

ColorController::ColorController()
{
}

ColorController::~ColorController()
{
}

bool ColorController::RetrieveColor( const std::string& colorCode, Vector4& colorValue )
{
  return false;
}

bool ColorController::RetrieveColor( const std::string& colorCode , Vector4& textColor, Vector4& textOutlineColor, Vector4& textShadowColor)
{
  return false;
}

} // namespace Adaptor

} // namespace Internal

} // namespace Dali
