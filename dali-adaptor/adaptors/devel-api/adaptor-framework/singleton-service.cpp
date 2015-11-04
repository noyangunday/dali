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
#include <singleton-service.h>

// INTERNAL INCLUDES
#include <singleton-service-impl.h>

namespace Dali
{

SingletonService::SingletonService()
{
}

SingletonService SingletonService::New()
{
  return Internal::Adaptor::SingletonService::New();
}

SingletonService SingletonService::Get()
{
  return Internal::Adaptor::SingletonService::Get();
}

SingletonService::~SingletonService()
{
}

void SingletonService::Register( const std::type_info& info, BaseHandle singleton )
{
  GetImplementation( *this ).Register( info, singleton );
}

void SingletonService::UnregisterAll()
{
  GetImplementation( *this ).UnregisterAll();
}

BaseHandle SingletonService::GetSingleton( const std::type_info& info ) const
{
  return GetImplementation( *this ).GetSingleton( info );
}

SingletonService::SingletonService( Internal::Adaptor::SingletonService* singletonService )
: BaseHandle( singletonService )
{
}

} // namespace Dali
