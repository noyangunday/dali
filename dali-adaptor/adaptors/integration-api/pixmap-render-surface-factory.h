#ifndef __DALI_INTEGRATION_PIXMAP_RENDER_SURFACE_FACTORY_H__
#define __DALI_INTEGRATION_PIXMAP_RENDER_SURFACE_FACTORY_H__

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
#include <dali/public-api/object/any.h>
#include <dali/public-api/common/dali-common.h>

// INTERNAL INCLUDES

namespace Dali
{

namespace ECore
{

class PixmapRenderSurface;

/**
 * Surface factory function for pixmap
 * A pixmap surface is created.
 *
 * @param [in] positionSize the position and size of the surface to create
 * @param [in] surface X Pixmap to use, or null for default.
 * @param [in] name Name of surface passed in
 * @param [in] isTransparent Whether the surface has an alpha channel
 */
PixmapRenderSurface* CreatePixmapSurface(PositionSize       positionSize,
                                         Any                surface,
                                         const std::string& name,
                                         bool               isTransparent);

} // namespace ECoreX

} // namespace Dali

#endif //  __DALI_INTEGRATION_PIXMAP_RENDER_SURFACE_FACTORY_H__
