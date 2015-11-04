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

// CLASS  HEADER
#include "segmentation-impl.h"

// INTERNAL INCLUDES
#include <singleton-service-impl.h>

// EXTERNAL INCLUDES
#include <linebreak.h>
#include <wordbreak.h>

namespace Dali
{

namespace TextAbstraction
{

namespace Internal
{

struct Segmentation::Plugin
{
  void GetLineBreakPositions( const Character* const text,
                              Length numberOfCharacters,
                              LineBreakInfo* breakInfo )
  {
    set_linebreaks_utf32( text, numberOfCharacters, NULL, breakInfo );
  }

  void GetWordBreakPositions( const Character* const text,
                              Length numberOfCharacters,
                              WordBreakInfo* breakInfo )
  {
    set_wordbreaks_utf32( text, numberOfCharacters, NULL, breakInfo );
  }
};

Segmentation::Segmentation()
: mPlugin( NULL )
{}

Segmentation::~Segmentation()
{
  delete mPlugin;
}

TextAbstraction::Segmentation Segmentation::Get()
{
  TextAbstraction::Segmentation segmentationHandle;

  SingletonService service( SingletonService::Get() );
  if( service )
  {
    // Check whether the singleton is already created
    Dali::BaseHandle handle = service.GetSingleton( typeid( TextAbstraction::Segmentation ) );
    if( handle )
    {
      // If so, downcast the handle
      Segmentation* impl = dynamic_cast< Internal::Segmentation* >( handle.GetObjectPtr() );
      segmentationHandle = TextAbstraction::Segmentation( impl );
    }
    else // create and register the object
    {
      segmentationHandle = TextAbstraction::Segmentation( new Segmentation );
      service.Register( typeid( segmentationHandle ), segmentationHandle );
    }
  }

  return segmentationHandle;
}

void Segmentation::GetLineBreakPositions( const Character* const text,
                                          Length numberOfCharacters,
                                          LineBreakInfo* breakInfo )
{
  CreatePlugin();

  mPlugin->GetLineBreakPositions( text, numberOfCharacters, breakInfo );
}

void Segmentation::GetWordBreakPositions( const Character* const text,
                                          Length numberOfCharacters,
                                          WordBreakInfo* breakInfo )
{
  CreatePlugin();

  mPlugin->GetWordBreakPositions( text, numberOfCharacters, breakInfo );
}

void Segmentation::CreatePlugin()
{
  if( !mPlugin )
  {
    mPlugin = new Plugin();
  }
}

} // namespace Internal

} // namespace TextAbstraction

} // namespace Dali
