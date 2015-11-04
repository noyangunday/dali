#ifndef __DALI_CLIPBOARD_EVENT_NOTIFIER_H__
#define __DALI_CLIPBOARD_EVENT_NOTIFIER_H__

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

// EXTERNAL INCLUDES
#include <dali/public-api/object/base-handle.h>
#include <dali/public-api/signals/dali-signal.h>

namespace Dali
{

namespace Internal DALI_INTERNAL
{
namespace Adaptor
{
class ClipboardEventNotifier;
}
}

/**
 * @brief The ClipboardEventNotifier provides signals when clipboard events are received from the device.
 */
class DALI_IMPORT_API ClipboardEventNotifier : public BaseHandle
{
public:

  // Typedefs

  /**
   * @brief Clipboard event
   */
  typedef Signal< void ( ClipboardEventNotifier& ) > ClipboardEventSignalType;

  /**
   * @brief Create an uninitialized handle.
   *
   * This can be initialized by getting the notifier from Dali::Adaptor.
   */
  ClipboardEventNotifier();

  /**
   * @brief Retrieve a handle to the ClipboardEventNotifier instance.
   *
   * @return A handle to the ClipboardEventNotifier
   */
  static ClipboardEventNotifier Get();

  /**
   * @brief Destructor
   *
   * This is non-virtual since derived Handle types must not contain data or virtual methods.
   */
  ~ClipboardEventNotifier();

  /**
   * @brief Returns the selected content.
   * @return A reference to the string representing the selected content.
   */
  const std::string& GetContent() const;

  /**
   * @brief Sets the selected content.
   * @param[in] content  A string that represents the content that has been selected.
   */
  void SetContent( const std::string& content );

  /**
   * @brief Clears the stored content.
   */
  void ClearContent();

  /**
   * @brief Called when content is selected in the clipboard.
   */
  void EmitContentSelectedSignal();

public:  // Signals

  /**
   * @brief This is emitted when content is selected from the clipboard.
   *
   * A callback of the following type may be connected:
   * @code
   *   void YourCallback( ClipboardEventNotifier& notifier );
   * @endcode
   * @return The signal to connect to.
   */
  ClipboardEventSignalType& ContentSelectedSignal();

public: // Not intended for application developers

  /**
   * @brief This constructor is used by ClipboardEventNotifier::Get().
   *
   * @param[in] notifier A pointer to the drag and drop notifier.
   */
  explicit DALI_INTERNAL ClipboardEventNotifier( Internal::Adaptor::ClipboardEventNotifier* notifier );
};

} // namespace Dali

#endif // __DALI_CLIPBOARD_EVENT_NOTIFIER_H__
