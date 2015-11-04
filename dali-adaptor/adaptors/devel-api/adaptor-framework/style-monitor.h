#ifndef __DALI_STYLE_MONITOR_H__
#define __DALI_STYLE_MONITOR_H__

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
#include <string>

#include <dali/public-api/object/base-handle.h>
#include <dali/public-api/signals/dali-signal.h>

// INTERNAL INCLUDES
#ifdef DALI_ADAPTOR_COMPILATION  // full path doesn't exist until adaptor is installed so we have to use relative
// @todo Make dali-adaptor code folder structure mirror the folder structure installed to dali-env
#include <style-change.h>
#else
#include <dali/public-api/adaptor-framework/style-change.h>
#endif

namespace Dali
{

namespace Internal DALI_INTERNAL
{
namespace Adaptor
{
class StyleMonitor;
}
}

/**
 * @brief Monitors the platform for style changes.
 *
 * This is a handle to the adaptor's style monitor which holds the platform's style information.
 * It provides a signal when any aspect of the default style changes on the device.
 * @see Adaptor::GetStyleMonitor
 */
class DALI_IMPORT_API StyleMonitor : public BaseHandle
{
public: // Typedefs

  typedef Signal< void ( StyleMonitor, StyleChange::Type ) > StyleChangeSignalType;   ///< StyleChange Signal type

public: // Creation & Destruction

  /**
   * @brief Create an uninitialized StyleMonitor handle.
   *
   * Tthis can be set by retrieving the style monitor from the Adaptor
   * or the Application classes.  Calling member functions when
   * uninitialized is not allowed.
   */
  StyleMonitor();

  /**
   * @brief Creates a copy of the handle.
   *
   * The copy will point to the same implementation as the original.
   * @param[in]  monitor  The Style Monitor to copy from.
   */
  StyleMonitor(const StyleMonitor& monitor);

  /**
   * @brief Retrieve the initialized instance of the StyleMonitor.
   * @return Handle to StyleMonitor.
   */
  static StyleMonitor Get();

  /**
   * @brief Destructor
   *
   * This is non-virtual since derived Handle types must not contain data or virtual methods.
   */
  ~StyleMonitor();

  /**
   * @brief Downcast an Object handle to StyleMonitor handle.
   *
   * If handle points to a StyleMonitor object the downcast produces a
   * valid handle. If not the returned handle is left uninitialized.
   *
   * @param[in] handle to An object @return handle to a Timer object
   * or an uninitialized handle
   */
  static StyleMonitor DownCast( BaseHandle handle );

public: // Style Information

  /**
   * @brief Retrieves the default font family.
   * @return The default font family.
   */
  std::string GetDefaultFontFamily() const;

  /**
   * @brief Retrieves the default font style.
   * @return The default font style.
   */
  std::string GetDefaultFontStyle() const;

  /**
   * @brief Retrieves the default font size.
   *
   * This is an logical size, which is mapped to a UI Control specific point-size in stylesheets.
   * For example if zero the smallest size, this could potentially map to TextLabel point-size 8.
   * @return The default font size, or a negative value if not set.
   */
  int GetDefaultFontSize() const;

  /**
   * @brief Retrieves the user defined Theme.
   * @return The user defined Theme.
   */
  const std::string& GetTheme() const;

  /**
   * @brief Sets an user defined Theme.
   * @param[in] themeFilePath Path of the user defined theme
   */
  void SetTheme(const std::string& themeFilePath);

  /**
   * @brief Utility to load a theme file
   * @param filename of the theme
   * @param output to write the contents to
   * @return true if the load is successful
   */
  bool LoadThemeFile( const std::string& filename, std::string& output );

public: // Signals

  /**
   * @brief This signal is emitted whenever the style changes on the device.
   *
   * A callback of the following type may be connected:
   * @code
   *   void YourCallbackName(StyleMonitor styleMonitor, StyleChange change);
   * @endcode
   * @return The signal to connect to.
   */
  StyleChangeSignalType& StyleChangeSignal();

public: // Operators

  /**
   * @brief Assignment operator.
   *
   * The handle points to the same implementation as the one being copied from.
   * @param[in]  monitor  The Style Monitor to copy from.
   * @return reference to this object
   */
  StyleMonitor& operator=(const StyleMonitor& monitor);

public: // Not intended for application developers
  /**
   * @brief This constructor is used internally to create a handle from an object pointer.
   * @param [in] styleMonitor A pointer the internal style monitor.
   */
  explicit DALI_INTERNAL StyleMonitor(Internal::Adaptor::StyleMonitor* styleMonitor);
};

} // namespace Dali

#endif // __DALI_STYLE_MONITOR_H__
