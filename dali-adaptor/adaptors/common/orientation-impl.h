#ifndef __DALI_INTERNAL_ORIENTATION_H__
#define __DALI_INTERNAL_ORIENTATION_H__

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
#include <cmath>
#include <dali/public-api/common/constants.h>
#include <dali/public-api/object/base-object.h>

// INTERNAL INCLUDES
#include <orientation.h>
#include <rotation-observer.h>

namespace Dali
{
class Adaptor;

namespace Internal
{

namespace Adaptor
{
class Window;
class Orientation;

typedef IntrusivePtr<Orientation> OrientationPtr;

class Orientation : public BaseObject, public RotationObserver
{
public:

  typedef Dali::Orientation::OrientationSignalType OrientationSignalType;

  static Orientation* New(Window* window);

  /**
   * Constructor
   */
  Orientation(Window* window);

protected:
  /**
   * Destructor
   */
  virtual ~Orientation();

public:
  /**
   * Set the adaptor for basic setup
   * @param[in] adaptor The adaptor
   */
  void SetAdaptor(Dali::Adaptor& adaptor);

  /**
   * Returns the actual orientation in degrees
   * @return The device's orientation
   */
  int GetDegrees() const;

  /**
   * Returns the actual orientation in radians
   * @return The device's orientation
   */
  float GetRadians() const;

public: // Signals

  /**
   * @copydoc Dali::Orientation::ChangedSignal()
   */
  OrientationSignalType& ChangedSignal();

private:
  /**
   * @copydoc Dali::Internal::Adaptor::RotationObserver::OnRotationPrepare()
   */
  virtual void OnRotationPrepare( const RotationEvent& rotation );

  /**
   * @copydoc Dali::Internal::Adaptor::RotationObserver::OnRotationRequest()
   */
  virtual void OnRotationRequest( );

  // Undefined
  Orientation(const Orientation&);
  Orientation& operator=(Orientation&);

private:
  /**
   * Signals and sends event of orientation change.
   */
  void EmitOrientationChange();

private:

  Window*                                  mWindow;

  OrientationSignalType mChangedSignal;

  int                                      mOrientation;
  int                                      mWindowWidth;
  int                                      mWindowHeight;
};

inline Orientation& GetImplementation (Dali::Orientation& orientation)
{
  DALI_ASSERT_ALWAYS(orientation && "Orientation handle is empty");

  BaseObject& handle = orientation.GetBaseObject();

  return static_cast<Orientation&>(handle);
}

inline const Orientation& GetImplementation(const Dali::Orientation& orientation)
{
  DALI_ASSERT_ALWAYS(orientation && "Orientation handle is empty");

  const BaseObject& handle = orientation.GetBaseObject();

  return static_cast<const Orientation&>(handle);
}

} // namespace Adaptor

} // namespace Internal

} // namespace Dali

#endif // __DALI_INTERNAL_ORIENTATION_H__
