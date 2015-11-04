#ifndef __DALI_INTERNAL_STYLE_MONITOR_H__
#define __DALI_INTERNAL_STYLE_MONITOR_H__

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
#include <dali/public-api/object/ref-object.h>
#include <dali/public-api/object/base-object.h>
#include <dali/integration-api/platform-abstraction.h>
#include <dali/devel-api/text-abstraction/font-client.h>

// INTERNAL INCLUDES
#include <style-monitor.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

/**
 * This holds the platform's style information.
 * It provides a signal when any aspect of the default style changes on the device.
 */
class StyleMonitor : public BaseObject
{
public:

  // Creation & Destruction

  /**
   * Constructor.
   * @param[in]  platformAbstraction  The platform abstraction.
   */
  StyleMonitor(Integration::PlatformAbstraction& platformAbstraction);

  /**
   * Retrieve the initialized instance of the StyleMonitor.
   * @return Handle to StyleMonitor.
   */
  static Dali::StyleMonitor Get();

  // Style Change Notifications

  /**
   * Informs the Style Monitor that the style has changed.
   * @param[in]  styleChange  The details of the change.
   */
  void StyleChanged( StyleChange::Type styleChange );

  // Style Information

  /**
   * @copydoc Dali::StyleMonitor::GetDefaultFontFamily() const
   */
  std::string GetDefaultFontFamily() const;

  /**
   * @copydoc Dali::StyleMonitor::GetDefaultFontStyle() const
   */
  std::string GetDefaultFontStyle() const;

  /**
   * @copydoc Dali::StyleMonitor::GetDefaultFontSize() const
   */
  int GetDefaultFontSize() const;

  /**
   * @copydoc Dali::StyleMonitor::GetTheme() const
   */
  const std::string& GetTheme() const;

  /**
   * @copydoc Dali::StyleMonitor::SetTheme()
   */
  void SetTheme(const std::string& themeFilePath);

  /**
   * @copydoc Dali::StyleMonitor::LoadThemeFile()
   */
  bool LoadThemeFile( const std::string& filename, std::string& output );

  // Signals

  /**
   * @copydoc Dali::StyleMonitor::StyleChangeSignal()
   */
  Dali::StyleMonitor::StyleChangeSignalType& StyleChangeSignal();

protected:

  /**
   * Virtual Destructor.
   */
  virtual ~StyleMonitor();

private:

  /**
   * Emit the style change signal.
   * @param[in]  styleChange  The details of the style change
   */
  inline void EmitStyleChangeSignal( StyleChange::Type styleChange );

private:

  Dali::StyleMonitor::StyleChangeSignalType mStyleChangeSignal; ///< Emitted when the style changes

  Integration::PlatformAbstraction& mPlatformAbstraction; ///< Reference to the PlatformAbstraction (for retrieving defaults)

  TextAbstraction::FontClient mfontClient;
  std::string mDefaultFontFamily;        ///< The system default font family
  std::string mDefaultFontStyle;         ///< The default font style
  std::string mUserDefinedThemeFilePath; ///< String containing the user defined theme file path
  int mDefaultFontSize;                  ///< The default accessibility font size e.g. 0 is smallest
};

} // namespace Adaptor

} // namespace Internal

// Additional Helpers for public-api forwarding methods

inline Internal::Adaptor::StyleMonitor& GetImplementation(Dali::StyleMonitor& monitor)
{
  DALI_ASSERT_ALWAYS(monitor && "Monitor handle is empty");
  BaseObject& handle = monitor.GetBaseObject();
  return static_cast<Internal::Adaptor::StyleMonitor&>(handle);
}

inline const Internal::Adaptor::StyleMonitor& GetImplementation(const Dali::StyleMonitor& monitor)
{
  DALI_ASSERT_ALWAYS(monitor && "Monitor handle is empty");
  const BaseObject& handle = monitor.GetBaseObject();
  return static_cast<const Internal::Adaptor::StyleMonitor&>(handle);
}

} // namespace Dali

#endif // __DALI_INTERNAL_STYLE_MONITOR_H__
