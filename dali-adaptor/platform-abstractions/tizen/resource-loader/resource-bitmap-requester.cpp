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
#include "resource-bitmap-requester.h"

// EXTERNAL INCLUDES
#include <dali/integration-api/resource-cache.h>

// INTERNAL INCLUDES
#include "network/file-download.h"
#include "network/http-utils.h"

using namespace Dali::Integration;

namespace Dali
{
namespace TizenPlatform
{
namespace
{
enum ResourceScheme
{
  FILE_SYSTEM_RESOURCE,
  NETWORK_RESOURCE
};
}// unnamed namespace

ResourceBitmapRequester::ResourceBitmapRequester( ResourceLoader& resourceLoader )
: ResourceRequesterBase( resourceLoader ),
  mThreadImageLocal( NULL ),
  mThreadImageRemote( NULL )
{
}

ResourceBitmapRequester::~ResourceBitmapRequester()
{
  delete mThreadImageLocal;
  delete mThreadImageRemote;
}

void ResourceBitmapRequester::Pause()
{
  if( mThreadImageLocal )
  {
    mThreadImageLocal->Pause();
  }
  if( mThreadImageRemote )
  {
    mThreadImageRemote->Pause();
  }
}

void ResourceBitmapRequester::Resume()
{
  if( mThreadImageLocal )
  {
    mThreadImageLocal->Resume();
  }
  if( mThreadImageRemote )
  {
    mThreadImageRemote->Resume();
  }
}

void ResourceBitmapRequester::LoadResource( Integration::ResourceRequest& request )
{
  DALI_ASSERT_DEBUG( (0 != dynamic_cast<BitmapResourceType*>(request.GetType())) && "Only requsts for bitmap resources can ever be routed to ResourceBitmapRequester.\n");
  BitmapResourceType* resType = static_cast<BitmapResourceType*>(request.GetType());
  if( !resType )
  {
    return;
  }

  // Work out what thread to decode / load the image on:
  ResourceScheme scheme( FILE_SYSTEM_RESOURCE );

  // Work out if the resource is in memory, a file, or in a remote server:
  ResourceThreadBase::RequestType requestType;

  // if resource exists already, then it just needs decoding
  if( request.GetResource().Get() )
  {
    requestType = ResourceThreadBase::RequestDecode;
  }
  else
  {
    const std::string& resourcePath = request.GetPath();
    if( Network::IsHttpUrl( resourcePath) )
    {
      requestType = ResourceThreadBase::RequestDownload;
      scheme = NETWORK_RESOURCE;
    }
    else
    {
      requestType = ResourceThreadBase::RequestLoad;
    }
  }

  // Dispatch the job to the right thread
  // lazily create the thread
  if( scheme ==  FILE_SYSTEM_RESOURCE )
  {
    if( !mThreadImageLocal )
    {
      mThreadImageLocal = new ResourceThreadImage( mResourceLoader );
    }
    mThreadImageLocal->AddRequest( request, requestType );
  }
  else
  {
    if( !mThreadImageRemote )
    {
      mThreadImageRemote = new ResourceThreadImage( mResourceLoader );
    }
    mThreadImageRemote->AddRequest( request, requestType );
  }
}

Integration::LoadStatus ResourceBitmapRequester::LoadFurtherResources( Integration::ResourceRequest& request, LoadedResource partialResource )
{
  // Nothing to do
  return RESOURCE_COMPLETELY_LOADED;
}

void ResourceBitmapRequester::CancelLoad(Integration::ResourceId id, Integration::ResourceTypeId typeId)
{
  if( mThreadImageLocal )
  {
    mThreadImageLocal->CancelRequest(id);
  }
  if( mThreadImageRemote )
  {
    mThreadImageRemote->CancelRequest(id);
  }
}

} // TizenPlatform
} // Dali
