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

#include "utc-image-loading-common.h"

void utc_image_loading_cancel_all_loads_startup(void)
{
  utc_dali_loading_startup();
}

void utc_image_loading_cancel_all_loads_cleanup(void)
{
  utc_dali_loading_cleanup();
}

/**
 * @brief Test case for load cancellation.
 *
 * Load lots of images in batches, cancelling all in a batch after a small delay to
 * allow the first of a batch to be launched before cancellation starts.
 * Assert that all loads issued are either completed or cancelled.
 *
 * @note Many loads will succeed despite our cancellations due to the coarse
 * granularity of the waits we introduce after loading each batch. That is
 * expected.
 */
int UtcDaliCancelAllLoads(void)
{
  tet_printf( "Running load cancel-all test.\n" );

  DALI_ASSERT_ALWAYS( gAbstraction != 0 );

  // Start a bunch of loads that should work:

  Dali::Integration::LoadResourcePriority priority = LoadPriorityNormal;
  unsigned loadsLaunched = 0;
  Dali::Internal::Platform::ResourceCollector resourceSink;

  for( unsigned loadGroup = 0; loadGroup < NUM_CANCELLED_LOAD_GROUPS_TO_ISSUE; ++loadGroup )
  {
    // Issue load requests for a batch of images:
    for( unsigned validImage = 0; validImage < NUM_VALID_IMAGES; ++validImage )
    {
      const ImageParameters & loadParams = gCancelAttributes[ loadsLaunched % gCancelAttributes.size() ];
      const BitmapResourceType bitmapResourceType( loadParams.first, loadParams.second.first, loadParams.second.second.first, loadParams.second.second.second );
      const ResourceId resourceId = loadGroup * NUM_VALID_IMAGES + validImage + 1;
      gAbstraction->LoadResource( ResourceRequest( resourceId, bitmapResourceType, VALID_IMAGES[validImage], priority ) );
      loadsLaunched += 1;
    }

    // Poll for at least one completed load so we have a good chance of catching an
    // in-flight load as we run through the cancellations further below:
    PollForNotification( resourceSink, *gAbstraction, 100 );

    // Cancel all the launched loads in the batch from oldest to newest:
    for( unsigned validImage = 0; validImage < NUM_VALID_IMAGES; ++validImage )
    {
      const ResourceId resourceId = loadGroup * NUM_VALID_IMAGES + validImage + 1;
      gAbstraction->CancelLoad( resourceId, ResourceBitmap );
    }
  }

  // Drain the completed loads:

  unsigned lastNotifications = -1;
  for( unsigned i = 0; resourceSink.mGrandTotalCompletions < loadsLaunched && resourceSink.mGrandTotalNotifications != lastNotifications; )
  {
    lastNotifications = resourceSink.mGrandTotalNotifications;
    gAbstraction->GetResources( resourceSink );

    ++i;
    if( i < MAX_NUM_RESOURCE_TRIES && resourceSink.mGrandTotalCompletions < loadsLaunched )
    {
      usleep( 1000 * 10 );
    }
    else
    {
      break;
    }
  }

  // Check the loads completed as expected:

  tet_printf( "Issued Loads: %u, Completed Loads: %u, Successful Loads: %u, Failed Loads: %u \n", loadsLaunched, resourceSink.mGrandTotalCompletions, unsigned(resourceSink.mSuccessCounts.size()), unsigned(resourceSink.mFailureCounts.size()) );
  DALI_TEST_CHECK( loadsLaunched > resourceSink.mGrandTotalCompletions );
  DALI_TEST_CHECK( loadsLaunched > resourceSink.mSuccessCounts.size() );
  DALI_TEST_CHECK( 0 == resourceSink.mFailureCounts.size() );

  // Check that each success was reported exactly once:
  for( ResourceCounterMap::const_iterator it = resourceSink.mSuccessCounts.begin(), end = resourceSink.mSuccessCounts.end(); it != end; ++it )
  {
    DALI_TEST_CHECK( it->second == 1u );
  }

  END_TEST;
}
