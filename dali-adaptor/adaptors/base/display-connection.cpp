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

// CLASS HEADER
#include <base/display-connection.h>

// EXTERNAL INCLUDES

// INTERNAL INCLUDES
#include <display-connection-impl.h>
#include <egl-interface.h>

namespace Dali
{

DisplayConnection* DisplayConnection::New()
{
  Internal::Adaptor::DisplayConnection* internal(Internal::Adaptor::DisplayConnection::New());

  return new DisplayConnection(internal);
}

DisplayConnection::DisplayConnection()
{
}

DisplayConnection::DisplayConnection(Internal::Adaptor::DisplayConnection* impl)
: mImpl(impl)
{
}

DisplayConnection::~DisplayConnection()
{
  if (mImpl)
  {
    delete mImpl;
    mImpl = NULL;
  }
}

Any DisplayConnection::GetDisplay()
{
  return mImpl->GetDisplay();
}

void DisplayConnection::GetDpi(unsigned int& dpiHorizontal, unsigned int& dpiVertical)
{
  Internal::Adaptor::DisplayConnection::GetDpi(dpiHorizontal, dpiVertical);
}

void DisplayConnection::ConsumeEvents()
{
  mImpl->ConsumeEvents();
}

bool DisplayConnection::InitializeEgl(EglInterface& egl)
{
  return mImpl->InitializeEgl(egl);
}

}
