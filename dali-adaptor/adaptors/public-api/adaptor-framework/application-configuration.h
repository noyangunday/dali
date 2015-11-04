#ifndef __DALI_APPLICATION_CONFIGURATION_H__
#define __DALI_APPLICATION_CONFIGURATION_H__

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
 */

namespace Dali
{
/**
 * @addtogroup dali_adaptor_framework
 * @{
 */

namespace Configuration
{
enum ContextLoss
{
  APPLICATION_HANDLES_CONTEXT_LOSS,  ///< Application will tear down and recreate UI on context loss and context regained signals. Dali doesn't need to retain data.
  APPLICATION_DOES_NOT_HANDLE_CONTEXT_LOSS, ///< Application expects Dali to retain data ( increased memory footprint )
};

} // Configuration

/**
 * @}
 */
} // namespace Dali

#endif
