#ifndef __DALI_INTERNAL_PHYSICAL_KEYBOARD_H__
#define __DALI_INTERNAL_PHYSICAL_KEYBOARD_H__

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

// INTERNAL INCLUDES
#include <physical-keyboard.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

class PhysicalKeyboard : public BaseObject
{
public:

  /**
   * Creates a new instance of the PhysicalKeyboard.
   */
  static Dali::PhysicalKeyboard New();

  /**
   * Gets the singleton instance of the Physical Keyboard.
   */
  static Dali::PhysicalKeyboard Get();

  /**
   * @copydoc Dali::PhysicalKeyboard::IsAttached()
   */
  bool IsAttached() const;

  /**
   * Should be called by the EventHandler when a key is received. If it's received from a physical
   * keyboard then the parameter should be true.
   * @param[in]  fromPhysicalKeyboard  true if received from a physical keyboard, false otherwise.
   */
  void KeyReceived( bool fromPhysicalKeyboard );

  // Signals

  /**
   * @copydoc Dali::PhysicalKeyboard::StatusChangedSignal()
   */
  Dali::PhysicalKeyboard::PhysicalKeyboardSignalType& StatusChangedSignal() { return mStatusChangedSignal; }

protected:

  /**
   * A reference counted object may only be deleted by calling Unreference()
   */
  virtual ~PhysicalKeyboard();

private:

  // Undefined
  PhysicalKeyboard( const PhysicalKeyboard& );
  PhysicalKeyboard& operator=( PhysicalKeyboard& );

  /**
   * Constructor
   */
  PhysicalKeyboard();

private:

  Dali::PhysicalKeyboard::PhysicalKeyboardSignalType mStatusChangedSignal; ///< Status changed signal
  bool mAttached; ///< true if the physical keyboard is attached, false otherwise
};

} // namespace Adaptor

} // namespace Internal

// Helpers for public-api forwarding methods

inline static Internal::Adaptor::PhysicalKeyboard& GetImplementation( PhysicalKeyboard& keyboard )
{
  DALI_ASSERT_ALWAYS( keyboard && "PhysicalKeyboard handle is empty" );

  BaseObject& handle = keyboard.GetBaseObject();

  return static_cast< Internal::Adaptor::PhysicalKeyboard& >( handle );
}

inline static const  Internal::Adaptor::PhysicalKeyboard& GetImplementation( const PhysicalKeyboard& keyboard )
{
  DALI_ASSERT_ALWAYS( keyboard && "PhysicalKeyboard handle is empty" );

  const BaseObject& handle = keyboard.GetBaseObject();

  return static_cast< const Internal::Adaptor::PhysicalKeyboard& >( handle );
}

} // namespace Dali

#endif // __DALI_INTERNAL_PHYSICAL_KEYBOARD_H__
