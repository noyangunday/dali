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
#include <dali/internal/event/actors/image-actor-impl.h>

// EXTERNAL INCLUDES
#include <cstring> // for strcmp

// INTERNAL INCLUDES
#include <dali/public-api/object/type-registry.h>
#include <dali/devel-api/scripting/scripting.h>
#include <dali/internal/event/common/property-helper.h>
#include <dali/internal/event/effects/shader-effect-impl.h>
#include <dali/internal/event/images/image-connector.h>
#include <dali/internal/event/images/nine-patch-image-impl.h>

namespace Dali
{

namespace Internal
{

namespace
{

// Properties

//              Name           Type   writable animatable constraint-input  enum for index-checking
DALI_PROPERTY_TABLE_BEGIN
DALI_PROPERTY( "pixel-area",   RECTANGLE, true,    false,   true,    Dali::ImageActor::Property::PIXEL_AREA )
DALI_PROPERTY( "style",        STRING,    true,    false,   true,    Dali::ImageActor::Property::STYLE      )
DALI_PROPERTY( "border",       VECTOR4,   true,    false,   true,    Dali::ImageActor::Property::BORDER     )
DALI_PROPERTY( "image",        MAP,       true,    false,   false,   Dali::ImageActor::Property::IMAGE      )
DALI_PROPERTY_TABLE_END( DEFAULT_DERIVED_ACTOR_PROPERTY_START_INDEX )

BaseHandle Create()
{
  return Dali::ImageActor::New();
}

TypeRegistration mType( typeid( Dali::ImageActor ), typeid( Dali::Actor ), Create );

ImageActor::Style StyleEnum(const std::string &s)
{
  if(s == "STYLE_NINE_PATCH")
  {
    return Dali::ImageActor::STYLE_NINE_PATCH;
  }
  else if(s == "STYLE_NINE_PATCH_NO_CENTER")
  {
    return Dali::ImageActor::STYLE_NINE_PATCH_NO_CENTER;
  }
  else // if(s == "QUAD")
  {
    return Dali::ImageActor::STYLE_QUAD;
  }
}

std::string StyleString(const ImageActor::Style style)
{
  if(style == Dali::ImageActor::STYLE_NINE_PATCH)
  {
    return "STYLE_NINE_PATCH";
  }
  else if(style == Dali::ImageActor::STYLE_NINE_PATCH_NO_CENTER)
  {
    return "STYLE_NINE_PATCH_NO_CENTER";
  }
  else // if(s == "QUAD")
  {
    return "STYLE_QUAD";
  }
}
}

ImageActorPtr ImageActor::New()
{
  ImageActorPtr actor( new ImageActor );

  // Second-phase construction of base class
  actor->Initialize();

  // Create the attachment
  actor->mImageAttachment = ImageAttachment::New( actor->GetEventThreadServices(), *actor->mNode );
  actor->Attach( *actor->mImageAttachment );

  return actor;
}

void ImageActor::OnInitialize()
{
  // TODO: Remove this, at the moment its needed for size negotiation to work
  SetResizePolicy( ResizePolicy::USE_NATURAL_SIZE, Dimension::ALL_DIMENSIONS );
}

void ImageActor::SetImage( ImagePtr& image )
{
  ImagePtr currentImage = mImageAttachment->GetImage();
  // early exit if it's the same image as we already have
  if ( currentImage == image )
  {
    return;
  }

  // NOTE! image might be pointing to NULL, which is fine as in that case app wants to just remove the image
  ImagePtr newImage( image );
  // if image is not NULL, check for 9 patch
  if( newImage )
  {
    // Automatically convert nine-patch images to cropped bitmap
    NinePatchImage* ninePatchImage = NinePatchImage::DownCast( image.Get() );
    if( ninePatchImage )
    {
      newImage = ninePatchImage->CreateCroppedBufferImage();
      SetStyle( Dali::ImageActor::STYLE_NINE_PATCH );

      const NinePatchImage::StretchRanges& stretchPixelsX = ninePatchImage->GetStretchPixelsX();
      const NinePatchImage::StretchRanges& stretchPixelsY = ninePatchImage->GetStretchPixelsY();

      if( stretchPixelsX.Size() > 0 && stretchPixelsY.Size() > 0 )
      {
        Vector4 border;
        //The NinePatchImage stretch pixels are in the cropped image space, inset by 1 to get it to uncropped image space
        border.x = stretchPixelsX[ 0 ].GetX() + 1;
        border.y = stretchPixelsY[ 0 ].GetX() + 1;
        border.z = image->GetWidth() - stretchPixelsX[ 0 ].GetY() - 1;
        border.w = image->GetHeight() - stretchPixelsY[ 0 ].GetY() - 1;

        SetNinePatchBorder( border, true );
      }
    }
  }
  // set the actual image (normal or 9 patch) and natural size based on that
  mImageAttachment->SetImage( newImage );

  RelayoutRequest();
}

ImagePtr ImageActor::GetImage()
{
  return mImageAttachment->GetImage();
}

void ImageActor::SetPixelArea( const PixelArea& pixelArea )
{
  mImageAttachment->SetPixelArea( pixelArea );

  RelayoutRequest();
}

const ImageActor::PixelArea& ImageActor::GetPixelArea() const
{
  return mImageAttachment->GetPixelArea();
}

bool ImageActor::IsPixelAreaSet() const
{
  return mImageAttachment->IsPixelAreaSet();
}

void ImageActor::ClearPixelArea()
{
  mImageAttachment->ClearPixelArea();

  RelayoutRequest();
}

void ImageActor::SetStyle( Style style )
{
  mImageAttachment->SetStyle( style );
}

ImageActor::Style ImageActor::GetStyle() const
{
  return mImageAttachment->GetStyle();
}

void ImageActor::SetNinePatchBorder( const Vector4& border, bool inPixels )
{
  mImageAttachment->SetNinePatchBorder( border, inPixels );
}

Vector4 ImageActor::GetNinePatchBorder() const
{
  return mImageAttachment->GetNinePatchBorder();
}

ImageAttachment& ImageActor::GetImageAttachment()
{
  return *mImageAttachment;
}

RenderableAttachment& ImageActor::GetRenderableAttachment() const
{
  DALI_ASSERT_DEBUG( mImageAttachment && "ImageAttachment missing from ImageActor" );
  return *mImageAttachment;
}

ImageActor::ImageActor()
: Actor( Actor::RENDERABLE )
{
}

ImageActor::~ImageActor()
{
}

Vector3 ImageActor::GetNaturalSize() const
{
  Vector2 naturalSize( CalculateNaturalSize() );
  return Vector3( naturalSize.width, naturalSize.height, 0.f );
}

Vector2 ImageActor::CalculateNaturalSize() const
{
  // if no image then natural size is 0
  Vector2 size( 0.0f, 0.0f );

  ImagePtr image = mImageAttachment->GetImage();
  if( image )
  {
    if( IsPixelAreaSet() )
    {
      PixelArea area(GetPixelArea());
      size.width = area.width;
      size.height = area.height;
    }
    else
    {
      size = image->GetNaturalSize();
    }
  }

  return size;
}

void ImageActor::OnStageConnectionInternal()
{
}

void ImageActor::OnStageDisconnectionInternal()
{
}

unsigned int ImageActor::GetDefaultPropertyCount() const
{
  return Actor::GetDefaultPropertyCount() + DEFAULT_PROPERTY_COUNT;
}

void ImageActor::GetDefaultPropertyIndices( Property::IndexContainer& indices ) const
{
  Actor::GetDefaultPropertyIndices( indices ); // Actor class properties

  indices.Reserve( indices.Size() + DEFAULT_PROPERTY_COUNT );

  int index = DEFAULT_DERIVED_ACTOR_PROPERTY_START_INDEX;
  for ( int i = 0; i < DEFAULT_PROPERTY_COUNT; ++i, ++index )
  {
    indices.PushBack( index );
  }
}

bool ImageActor::IsDefaultPropertyWritable( Property::Index index ) const
{
  if( index < DEFAULT_ACTOR_PROPERTY_MAX_COUNT )
  {
    return Actor::IsDefaultPropertyWritable(index);
  }

  index -= DEFAULT_DERIVED_ACTOR_PROPERTY_START_INDEX;
  if ( ( index >= 0 ) && ( index < DEFAULT_PROPERTY_COUNT ) )
  {
    return DEFAULT_PROPERTY_DETAILS[ index ].writable;
  }

  return false;
}

bool ImageActor::IsDefaultPropertyAnimatable( Property::Index index ) const
{
  if( index < DEFAULT_ACTOR_PROPERTY_MAX_COUNT )
  {
    return Actor::IsDefaultPropertyAnimatable( index );
  }

  index -= DEFAULT_DERIVED_ACTOR_PROPERTY_START_INDEX;
  if ( ( index >= 0 ) && ( index < DEFAULT_PROPERTY_COUNT ) )
  {
    return DEFAULT_PROPERTY_DETAILS[ index ].animatable;
  }

  return false;
}

bool ImageActor::IsDefaultPropertyAConstraintInput( Property::Index index ) const
{
  if( index < DEFAULT_ACTOR_PROPERTY_MAX_COUNT )
  {
    return Actor::IsDefaultPropertyAConstraintInput( index );
  }

  index -= DEFAULT_DERIVED_ACTOR_PROPERTY_START_INDEX;
  if ( ( index >= 0 ) && ( index < DEFAULT_PROPERTY_COUNT ) )
  {
    return DEFAULT_PROPERTY_DETAILS[ index ].constraintInput;
  }

  return false;
}

Property::Type ImageActor::GetDefaultPropertyType( Property::Index index ) const
{
  if( index < DEFAULT_ACTOR_PROPERTY_MAX_COUNT )
  {
    return Actor::GetDefaultPropertyType( index );
  }

  index -= DEFAULT_DERIVED_ACTOR_PROPERTY_START_INDEX;
  if ( ( index >= 0 ) && ( index < DEFAULT_PROPERTY_COUNT ) )
  {
    return DEFAULT_PROPERTY_DETAILS[index].type;
  }

  // index out-of-bounds
  return Property::NONE;
}

const char* ImageActor::GetDefaultPropertyName( Property::Index index ) const
{
  if( index < DEFAULT_ACTOR_PROPERTY_MAX_COUNT)
  {
    return Actor::GetDefaultPropertyName(index);
  }

  index -= DEFAULT_DERIVED_ACTOR_PROPERTY_START_INDEX;
  if ( ( index >= 0 ) && ( index < DEFAULT_PROPERTY_COUNT ) )
  {
    return DEFAULT_PROPERTY_DETAILS[index].name;
  }

  // index out-of-bounds
  return NULL;
}

Property::Index ImageActor::GetDefaultPropertyIndex(const std::string& name) const
{
  Property::Index index = Property::INVALID_INDEX;

  // Look for name in default properties
  for( int i = 0; i < DEFAULT_PROPERTY_COUNT; ++i )
  {
    const Internal::PropertyDetails* property = &DEFAULT_PROPERTY_DETAILS[ i ];
    if( 0 == strcmp( name.c_str(), property->name ) ) // Don't want to convert rhs to string
    {
      index = i + DEFAULT_DERIVED_ACTOR_PROPERTY_START_INDEX;
      break;
    }
  }

  // If not found, check in base class
  if( Property::INVALID_INDEX == index )
  {
    index = Actor::GetDefaultPropertyIndex( name );
  }
  return index;
}

void ImageActor::SetDefaultProperty( Property::Index index, const Property::Value& propertyValue )
{
  if( index < DEFAULT_ACTOR_PROPERTY_MAX_COUNT )
  {
    Actor::SetDefaultProperty( index, propertyValue );
  }
  else
  {
    switch(index)
    {
      case Dali::ImageActor::Property::PIXEL_AREA:
      {
        SetPixelArea(propertyValue.Get<Rect<int> >());
        break;
      }
      case Dali::ImageActor::Property::STYLE:
      {
        SetStyle( StyleEnum( propertyValue.Get<std::string>() ) );
        break;
      }
      case Dali::ImageActor::Property::BORDER:
      {
        SetNinePatchBorder( propertyValue.Get<Vector4>(), true /*in pixels*/ );
        break;
      }
      case Dali::ImageActor::Property::IMAGE:
      {
        Dali::Image img = Scripting::NewImage( propertyValue );
        if(img)
        {
          ImagePtr image( &GetImplementation(img) );
          SetImage( image );
        }
        else
        {
          DALI_LOG_WARNING("Cannot create image from property value\n");
        }
        break;
      }
      default:
      {
        DALI_LOG_WARNING("Unknown property (%d)\n", index);
        break;
      }
    } // switch(index)

  } // else
}

Property::Value ImageActor::GetDefaultProperty( Property::Index index ) const
{
  Property::Value ret;
  if( index < DEFAULT_ACTOR_PROPERTY_MAX_COUNT )
  {
    ret = Actor::GetDefaultProperty( index );
  }
  else
  {
    switch( index )
    {
      case Dali::ImageActor::Property::PIXEL_AREA:
      {
        Rect<int> r = GetPixelArea();
        ret = r;
        break;
      }
      case Dali::ImageActor::Property::STYLE:
      {
        ret = StyleString( GetStyle() );
        break;
      }
      case Dali::ImageActor::Property::BORDER:
      {
        ret = GetNinePatchBorder();
        break;
      }
      case Dali::ImageActor::Property::IMAGE:
      {
        Property::Map map;
        Scripting::CreatePropertyMap( Dali::Image( mImageAttachment->GetImage().Get() ), map );
        ret = Property::Value( map );
        break;
      }
      default:
      {
        DALI_LOG_WARNING( "Unknown property (%d)\n", index );
        break;
      }
    } // switch(index)
  }

  return ret;
}


void ImageActor::SetSortModifier(float modifier)
{
  mImageAttachment->SetSortModifier( modifier );
}

float ImageActor::GetSortModifier() const
{
  return mImageAttachment->GetSortModifier();
}

void ImageActor::SetDepthIndex( int depthIndex )
{
   mImageAttachment->SetSortModifier( depthIndex );
}

int ImageActor::GetDepthIndex() const
{
  return static_cast< int >( mImageAttachment->GetSortModifier() );
}

void ImageActor::SetCullFace(CullFaceMode mode)
{
  mImageAttachment->SetCullFace( mode );
}

CullFaceMode ImageActor::GetCullFace() const
{
  return mImageAttachment->GetCullFace();
}

void ImageActor::SetBlendMode( BlendingMode::Type mode )
{
  mImageAttachment->SetBlendMode( mode );
}

BlendingMode::Type ImageActor::GetBlendMode() const
{
  return mImageAttachment->GetBlendMode();
}

void ImageActor::SetBlendFunc( BlendingFactor::Type srcFactorRgba,   BlendingFactor::Type destFactorRgba )
{
  mImageAttachment->SetBlendFunc( srcFactorRgba, destFactorRgba, srcFactorRgba, destFactorRgba );
}

void ImageActor::SetBlendFunc( BlendingFactor::Type srcFactorRgb,   BlendingFactor::Type destFactorRgb,
                               BlendingFactor::Type srcFactorAlpha, BlendingFactor::Type destFactorAlpha )
{
  mImageAttachment->SetBlendFunc( srcFactorRgb, destFactorRgb, srcFactorAlpha, destFactorAlpha );
}

void ImageActor::GetBlendFunc( BlendingFactor::Type& srcFactorRgb,   BlendingFactor::Type& destFactorRgb,
                                    BlendingFactor::Type& srcFactorAlpha, BlendingFactor::Type& destFactorAlpha ) const
{
  mImageAttachment->GetBlendFunc( srcFactorRgb, destFactorRgb, srcFactorAlpha, destFactorAlpha );
}

void ImageActor::SetBlendEquation( BlendingEquation::Type equationRgba )
{
  mImageAttachment->SetBlendEquation( equationRgba, equationRgba );
}

void ImageActor::SetBlendEquation( BlendingEquation::Type equationRgb, BlendingEquation::Type equationAlpha )
{
  mImageAttachment->SetBlendEquation( equationRgb, equationAlpha );
}

void ImageActor::GetBlendEquation( BlendingEquation::Type& equationRgb, BlendingEquation::Type& equationAlpha ) const
{
  mImageAttachment->GetBlendEquation( equationRgb, equationAlpha );
}

void ImageActor::SetBlendColor( const Vector4& color )
{
  mImageAttachment->SetBlendColor( color );
}

const Vector4& ImageActor::GetBlendColor() const
{
  return mImageAttachment->GetBlendColor();
}

void ImageActor::SetFilterMode( FilterMode::Type minFilter, FilterMode::Type magFilter )
{
  mImageAttachment->SetFilterMode( minFilter, magFilter );
}

void ImageActor::GetFilterMode( FilterMode::Type& minFilter, FilterMode::Type& magFilter ) const
{
  return mImageAttachment->GetFilterMode( minFilter, magFilter );
}

void ImageActor::SetShaderEffect(ShaderEffect& effect)
{
  mImageAttachment->SetShaderEffect( effect );
}

ShaderEffectPtr ImageActor::GetShaderEffect() const
{
  return mImageAttachment->GetShaderEffect();
}

void ImageActor::RemoveShaderEffect()
{
  return mImageAttachment->RemoveShaderEffect();
}


} // namespace Internal

} // namespace Dali
