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
#include "virtual-keyboard-impl.h"

// EXTERNAL INCLUDES
#include <Ecore.h>
#include <Ecore_Wayland.h>
#include <algorithm>

#include <dali/public-api/common/vector-wrapper.h>
#include <dali/integration-api/debug.h>

// INTERNAL INCLUDES
#include <adaptor.h>
#include <locale-utils.h>
#include <imf-manager-impl.h>


namespace Dali
{

namespace Internal
{

namespace Adaptor
{

namespace VirtualKeyboard
{

Dali::InputMethod::ActionButton gActionButtonFunction = Dali::InputMethod::ACTION_DEFAULT;

void RotateTo(int angle)
{
}

void SetReturnKeyType( const InputMethod::ActionButton type )
{
}

Dali::InputMethod::ActionButton GetReturnKeyType()
{
  return gActionButtonFunction;
}

} // namespace VirtualKeyboard

} // namespace Adaptor

} // namespace Internal

} // namespace Dali
