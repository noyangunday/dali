#ifndef __DALI_INTERNAL_SINGLETON_SERVICE_H__
#define __DALI_INTERNAL_SINGLETON_SERVICE_H__

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

// EXTERNAL INCLUDES
#include <dali/public-api/object/base-object.h>
#include <dali/devel-api/common/map-wrapper.h>

// INTERNAL INCLUDES
#include <singleton-service.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

class SingletonService : public Dali::BaseObject
{
public:

  /**
   * Create a SingletonService.
   * This should only be called once by the Application class.
   * @return A newly created SingletonService.
   */
  static Dali::SingletonService New();

  /**
   * @copydoc Dali::SingletonService::Get()
   */
  static Dali::SingletonService Get();

  /**
   * @copydoc Dali::SingletonService::Register()
   */
  void Register( const std::type_info& info, BaseHandle singleton );

  /**
   * @copydoc Dali::SingletonService::UnregisterAll()
   */
  void UnregisterAll();

  /**
   * @copydoc Dali::SingletonService::GetSingleton()
   */
  BaseHandle GetSingleton( const std::type_info& info ) const;

private:

  /**
   * Private Constructor
   * @see SingletonService::New()
   */
  SingletonService();

  /**
   * Virtual Destructor
   */
  virtual ~SingletonService();

  // Undefined
  SingletonService( const SingletonService& );
  SingletonService& operator=( SingletonService& );

private:

  typedef std::pair<std::string, BaseHandle> SingletonPair;
  typedef std::map<std::string, BaseHandle>  SingletonContainer;
  typedef SingletonContainer::const_iterator SingletonConstIter;

  SingletonContainer mSingletonContainer; ///< The container to look up singleton by its type name
};

} // namespace Adaptor

} // namespace Internal

// Helpers for public-api forwarding methods

inline Internal::Adaptor::SingletonService& GetImplementation(Dali::SingletonService& player)
{
  DALI_ASSERT_ALWAYS( player && "SingletonService handle is empty" );

  BaseObject& handle = player.GetBaseObject();

  return static_cast<Internal::Adaptor::SingletonService&>(handle);
}

inline const Internal::Adaptor::SingletonService& GetImplementation(const Dali::SingletonService& player)
{
  DALI_ASSERT_ALWAYS( player && "SingletonService handle is empty" );

  const BaseObject& handle = player.GetBaseObject();

  return static_cast<const Internal::Adaptor::SingletonService&>(handle);
}

} // namespace Dali

#endif // __DALI_INTERNAL_SINGLETON_SERVICE_H__
