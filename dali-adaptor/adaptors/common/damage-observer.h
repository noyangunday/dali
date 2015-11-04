#ifndef __DALI_INTERNAL_DAMAGE_OBSERVER_H__
#define __DALI_INTERNAL_DAMAGE_OBSERVER_H__

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
#include <dali/public-api/common/vector-wrapper.h>
#include <dali/public-api/math/rect.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

typedef Rect<int> DamageArea;

/**
 * The DamageObserver can be overridden in order to listen to damage events.
 */
class DamageObserver
{
public:

  /**
   * Deriving classes should override this to be notified when we receive a damage event.
   * @param[in]  area  The area that has been damaged.
   */
  virtual void OnDamaged( const DamageArea& area ) = 0;

protected:

  /**
   * Protected Constructor.
   */
  DamageObserver()
  {
  }

  /**
   * Protected virtual destructor.
   */
  virtual ~DamageObserver()
  {
  }
};

} // namespace Adaptor

} // namespace Internal

} // namespace Dali

#endif // __DALI_INTERNAL_DAMAGE_OBSERVER_H__
