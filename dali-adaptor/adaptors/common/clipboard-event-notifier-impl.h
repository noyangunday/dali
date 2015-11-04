#ifndef __DALI_INTERNAL_CLIPBOARD_EVENT_NOTIFIER_H__
#define __DALI_INTERNAL_CLIPBOARD_EVENT_NOTIFIER_H__

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
#include <string>
#include <dali/public-api/object/base-object.h>
#include <dali/public-api/math/vector2.h>

// INTERNAL INCLUDES
#include <clipboard-event-notifier.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

/**
 * This class listens to Clipboard events.
 */
class ClipboardEventNotifier : public Dali::BaseObject
{
public:

  typedef Dali::ClipboardEventNotifier::ClipboardEventSignalType ClipboardEventSignalType;

  // Creation

  /**
   * Create a ClipboardEventNotifier.
   * @return A newly allocated clipboard-event-notifier.
   */
  static Dali::ClipboardEventNotifier New();

  /**
   * @copydoc Dali::ClipboardEventNotifier::Get()
   */
  static Dali::ClipboardEventNotifier Get();

  // Public API

  /**
   * @copydoc Dali::ClipboardEventNotifier::GetContent() const
   */
  const std::string& GetContent() const;

  /**
   * Sets the selected content.
   * @param[in] content  A string that represents the content that has been selected.
   */
  void SetContent( const std::string& content );

  /**
   * Clears the stored content.
   */
  void ClearContent();

  /**
   * Called when content is selected in the clipboard.
   */
  void EmitContentSelectedSignal();

public: // Signals

  /**
   * @copydoc Dali::ClipboardEventNotifier::ContentSelectedSignal
   */
  ClipboardEventSignalType& ContentSelectedSignal()
  {
    return mContentSelectedSignal;
  }

private:

  // Construction & Destruction

  /**
   * Constructor.
   */
  ClipboardEventNotifier();

  /**
   * Destructor.
   */
  virtual ~ClipboardEventNotifier();

  // Undefined
  ClipboardEventNotifier( const ClipboardEventNotifier& );
  ClipboardEventNotifier& operator=( ClipboardEventNotifier& );

private:

  std::string mContent;    ///< The current selected content.

  ClipboardEventSignalType mContentSelectedSignal;

public:

  // Helpers for public-api forwarding methods

  inline static Internal::Adaptor::ClipboardEventNotifier& GetImplementation(Dali::ClipboardEventNotifier& detector)
  {
    DALI_ASSERT_ALWAYS( detector && "ClipboardEventNotifier handle is empty" );

    BaseObject& handle = detector.GetBaseObject();

    return static_cast<Internal::Adaptor::ClipboardEventNotifier&>(handle);
  }

  inline static const Internal::Adaptor::ClipboardEventNotifier& GetImplementation(const Dali::ClipboardEventNotifier& detector)
  {
    DALI_ASSERT_ALWAYS( detector && "ClipboardEventNotifier handle is empty" );

    const BaseObject& handle = detector.GetBaseObject();

    return static_cast<const Internal::Adaptor::ClipboardEventNotifier&>(handle);
  }

};

} // namespace Adaptor

} // namespace Internal

} // namespace Dali

#endif // __DALI_INTERNAL_CLIPBOARD_EVENT_NOTIFIER_H__
