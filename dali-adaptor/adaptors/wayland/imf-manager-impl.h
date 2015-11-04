#ifndef __DALI_INTERNAL_IMF_MANAGER_H
#define __DALI_INTERNAL_IMF_MANAGER_H

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
#include <Ecore_IMF.h>
#include <Ecore.h>
#include <Ecore_Wayland.h>

#include <dali/public-api/object/base-object.h>
#include <dali/integration-api/events/key-event-integ.h>

// INTERNAL INCLUDES
#include <imf-manager.h>

namespace Dali
{

class RenderSurface;

namespace Internal
{

namespace Adaptor
{

class ImfManager : public Dali::BaseObject
{
public:
  typedef Dali::ImfManager::ImfManagerSignalType ImfManagerSignalType;
  typedef Dali::ImfManager::ImfEventSignalType ImfEventSignalType;

public:

  /**
   * Check whether the ImfManager is available.
   * @return true if available, false otherwise
   */
  static bool IsAvailable();

  /**
   * Get the IMF manager instance, it creates the instance if it has not already been created.
   * Internally, a check should be made using IsAvailable() before this is called as we do not want
   * to create an instance if not needed by applications.
   * @see IsAvailable()
   */
  static Dali::ImfManager Get();

  /**
   * Connect Callbacks required for IMF.
   * If you don't connect imf callbacks, you can't get the key events.
   * The events are PreeditChanged, Commit and DeleteSurrounding.
   */
  void ConnectCallbacks();

  /**
   * Disconnect Callbacks attached to imf context.
   */
  void DisconnectCallbacks();

  /**
   * @copydoc Dali::ImfManager::Activate()
   */
  void Activate();

  /**
   * @copydoc Dali::ImfManager::Deactivate()
   */
  void Deactivate();

  /**
   * @copydoc Dali::ImfManager::Reset()
   */
  void Reset();

  /**
   * @copydoc Dali::ImfManager::GetContext()
   */
  Ecore_IMF_Context* GetContext();

  /**
   * @copydoc Dali::ImfManager::RestoreAfterFocusLost()
   */
  bool RestoreAfterFocusLost() const;

  /**
   * @copydoc Dali::ImfManager::SetRestoreAfterFocusLost()
   */
  void SetRestoreAfterFocusLost( bool toggle );

  /**
   * @copydoc Dali::ImfManager::PreEditChanged()
   */
  void PreEditChanged( void *data, Ecore_IMF_Context *imfContext, void *event_info );

  /**
   * @copydoc Dali::ImfManager::NotifyCursorPosition()
   */
  void CommitReceived( void *data, Ecore_IMF_Context *imfContext, void *event_info );

  /**
   * @copydoc Dali::ImfManager::NotifyCursorPosition()
   */
  Eina_Bool RetrieveSurrounding( void *data, Ecore_IMF_Context *imfContext, char** text, int* cursorPosition );

  /**
   * @copydoc Dali::ImfManager::DeleteSurrounding()
   */
  void DeleteSurrounding( void *data, Ecore_IMF_Context *imfContext, void *event_info );

  // Cursor related
  /**
   * @copydoc Dali::ImfManager::NotifyCursorPosition()
   */
  void NotifyCursorPosition();

  /**
   * @copydoc Dali::ImfManager::SetCursorPosition()
   */
  void SetCursorPosition( unsigned int cursorPosition );

  /**
   * @copydoc Dali::ImfManager::GetCursorPosition()
   */
  unsigned int GetCursorPosition() const;

  /**
   * @copydoc Dali::ImfManager::SetSurroundingText()
   */
  void SetSurroundingText( const std::string& text );

  /**
   * @copydoc Dali::ImfManager::GetSurroundingText()
   */
  const std::string& GetSurroundingText() const;

public:  // Signals

  /**
   * @copydoc Dali::ImfManager::ActivatedSignal()
   */
  ImfManagerSignalType& ActivatedSignal() { return mActivatedSignal; }

  /**
   * @copydoc Dali::ImfManager::EventReceivedSignal()
   */
  ImfEventSignalType& EventReceivedSignal() { return mEventSignal; }

protected:

  /**
   * Destructor.
   */
  virtual ~ImfManager();

private:
  /**
   * @copydoc Dali::ImfManager::DeleteContext()
   */
  void DeleteContext();

private:
  // Undefined
  ImfManager( const ImfManager& );
  ImfManager& operator=( ImfManager& );

private:
  Ecore_IMF_Context* mIMFContext;
  int mIMFCursorPosition;
  std::string mSurroundingText;

  bool mRestoreAfterFocusLost:1;             ///< Whether the keyboard needs to be restored (activated ) after focus regained.
  bool mIdleCallbackConnected:1;             ///< Whether the idle callback is already connected.

  std::vector<Dali::Integration::KeyEvent> mKeyEvents; ///< Stores key events to be sent from idle call-back.

  ImfManagerSignalType      mActivatedSignal;
  ImfEventSignalType        mEventSignal;

public:

inline static Internal::Adaptor::ImfManager& GetImplementation(Dali::ImfManager& imfManager)
{
  DALI_ASSERT_ALWAYS( imfManager && "ImfManager handle is empty" );

  BaseObject& handle = imfManager.GetBaseObject();

  return static_cast<Internal::Adaptor::ImfManager&>(handle);
}

inline static const  Internal::Adaptor::ImfManager& GetImplementation(const Dali::ImfManager& imfManager)
{
  DALI_ASSERT_ALWAYS( imfManager && "ImfManager handle is empty" );

  const BaseObject& handle = imfManager.GetBaseObject();

  return static_cast<const Internal::Adaptor::ImfManager&>(handle);
}

};


} // namespace Adaptor

} // namespace Internal

} // namespace Dali

#endif // __DALI_INTERNAL_IMF_MANAGER_H
