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
#include <common/color-controller-impl.h>

// EXTERNAL INCLUDES
#include <dali/integration-api/debug.h>
#include <dali/public-api/object/type-registry.h>

// INTERNAL INCLUDES
#include <singleton-service-impl.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

namespace
{

BaseHandle Create()
{
  return ColorController::Get();
}
Dali::TypeRegistration COLOR_CONTROLLER_TYPE( typeid(Dali::ColorController), typeid(Dali::BaseHandle), Create );

}

Dali::ColorController ColorController::Get()
{
  Dali::ColorController colorController;

  Dali::SingletonService service( SingletonService::Get() );
  if ( service )
  {
    // Check whether the singleton is already created
    Dali::BaseHandle handle = service.GetSingleton( typeid( Dali::ColorController ) );
    if(handle)
    {
      // If so, downcast the handle
      colorController = Dali::ColorController( dynamic_cast< ColorController* >( handle.GetObjectPtr() ) );
    }
    else
    {
      colorController = Dali::ColorController( new ColorController( ) );
      service.Register( typeid( colorController ), colorController );
    }
  }

  return colorController;

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
