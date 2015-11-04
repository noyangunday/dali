#ifndef __DALI_INTERNAL_CALLBACK_MANAGER_H__
#define __DALI_INTERNAL_CALLBACK_MANAGER_H__

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
#include <dali/public-api/common/dali-common.h>
#include <dali/public-api/signals/callback.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

/**
 * Abstract interface to install call backs in to an applications main loop.
 */
class CallbackManager
{

public:

    /**
     * Create a new call back interface
     */
    static CallbackManager* New();

    /**
     * Virtual destructor
     */
    virtual ~CallbackManager() {}

    /**
     * Adds a call back to be run on idle.
     * Must be call from main thread only.
     * @param callback custom call back function
     * @param priority call back priority
     * @return true on success
     */
    virtual bool AddIdleCallback( CallbackBase* callback ) = 0;

    /**
     * Starts the callback manager.
     */
    virtual void Start() = 0;

    /**
     * Stop the callback manager and can remove all pending callbacks synchronously.
     * This call will synchronise with the main loop and not return
     * until all call backs have been deleted.
     */
    virtual void Stop() = 0;

protected:

    /**
     * constructor
     */
    CallbackManager() {}

private:

    // Undefined copy constructor.
    CallbackManager( const CallbackManager& );

    // Undefined assignment operator.
    CallbackManager& operator=( const CallbackManager& );

};

} // namespace Adaptor

} // namespace Internal

} // namespace Dali

#endif // __DALI_INTERNAL_CALLBACK_MANAGER_H__
