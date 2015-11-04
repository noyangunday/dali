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

// CLASS HEADER
#include "abort-handler.h"

// EXTERNAL INCLUDES
#include <cstring>

namespace Dali
{
namespace Internal
{
namespace Adaptor
{

AbortHandler* AbortHandler::gInstance(NULL);

AbortHandler::AbortHandler( CallbackBase* callback )
: mSignalMask( 0 ),
  mCallback( callback )
{
  DALI_ASSERT_ALWAYS( gInstance == NULL && "Only one instance of abort handler allowed" );
  gInstance = this;

  memset( mSignalOldHandlers, 0, sizeof(SignalHandlerFuncPtr) * (_NSIG-1));
}

AbortHandler::~AbortHandler()
{
  delete mCallback;

  int signum;
  for ( signum = 1; signum < _NSIG; signum++ )
  {
    if ( mSignalMask & (1 << (signum-1) ) )
    {
      // set signals back to default handling
      signal( signum, mSignalOldHandlers[signum-1] );
    }
  }
  gInstance = NULL;
}

bool AbortHandler::AbortOnSignal( int signum )
{
  bool status = false;

  if ( signum < _NSIG )
  {
    SignalHandlerFuncPtr signalHandlerPrevious = signal( signum, &AbortHandler::SignalHandler );

    if ( SIG_ERR != signalHandlerPrevious )
    {
      mSignalOldHandlers[signum-1] = signalHandlerPrevious;
      mSignalMask |= ( 1 << (signum-1) );
      status = true;
    }
  }
  return status;
}

void AbortHandler::SignalHandler( int signum )
{
  if( gInstance )
  {
    if( gInstance->mCallback )
    {
      CallbackBase::Execute( *gInstance->mCallback );
    }
  }
}

} // Adaptor
} // Internal
} // Dali
