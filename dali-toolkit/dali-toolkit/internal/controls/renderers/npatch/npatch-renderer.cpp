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
#include "npatch-renderer.h"

// EXTERNAL INCLUDES
#include <dali/integration-api/platform-abstraction.h>
#include <dali/public-api/images/buffer-image.h>
#include <dali/public-api/images/resource-image.h>

// INTERNAL IINCLUDES
#include <dali-toolkit/internal/controls/renderers/renderer-factory-impl.h>
#include <dali-toolkit/internal/controls/renderers/renderer-factory-cache.h>
#include <dali-toolkit/internal/controls/renderers/control-renderer-impl.h>
#include <dali-toolkit/internal/controls/renderers/control-renderer-data-impl.h>


namespace Dali
{

namespace Toolkit
{

namespace Internal
{

namespace
{
const char * const RENDERER_TYPE("renderer-type");
const char * const RENDERER_TYPE_VALUE("n-patch-renderer");

const char * const IMAGE_URL_NAME("image-url");
const char * const BORDER_ONLY("border-only");

std::string TEXTURE_UNIFORM_NAME = "sTexture";

const char* VERTEX_SHADER = DALI_COMPOSE_SHADER(
  attribute mediump vec2 aPosition;\n
  varying mediump vec2 vTexCoord;\n
  uniform mediump mat4 uMvpMatrix;\n
  uniform mediump vec3 uSize;\n
  uniform mediump vec2 uNinePatchFactorsX[ FACTOR_SIZE_X ];\n
  uniform mediump vec2 uNinePatchFactorsY[ FACTOR_SIZE_Y ];\n
  \n
  void main()\n
  {\n
    mediump vec2 fixedFactor  = vec2( uNinePatchFactorsX[ int( ( aPosition.x + 1.0 ) * 0.5 ) ].x, uNinePatchFactorsY[ int( ( aPosition.y + 1.0 ) * 0.5 ) ].x );\n
    mediump vec2 stretch      = vec2( uNinePatchFactorsX[ int( ( aPosition.x       ) * 0.5 ) ].y, uNinePatchFactorsY[ int( ( aPosition.y       ) * 0.5 ) ].y );\n
    \n
    mediump vec2 fixedTotal   = vec2( uNinePatchFactorsX[ FACTOR_SIZE_X - 1 ].x, uNinePatchFactorsY[ FACTOR_SIZE_Y - 1 ].x );\n
    mediump vec2 stretchTotal = vec2( uNinePatchFactorsX[ FACTOR_SIZE_X - 1 ].y, uNinePatchFactorsY[ FACTOR_SIZE_Y - 1 ].y );\n
    \n
    mediump vec4 vertexPosition = vec4( ( fixedFactor + ( uSize.xy - fixedTotal ) * stretch / stretchTotal ), 0.0, 1.0 );\n
    vertexPosition.xy -= uSize.xy * vec2( 0.5, 0.5 );\n
    vertexPosition = uMvpMatrix * vertexPosition;\n
    \n
    vTexCoord = ( fixedFactor + stretch ) / ( fixedTotal + stretchTotal );\n
    \n
    gl_Position = vertexPosition;\n
  }\n
);

const char* VERTEX_SHADER_3X3 = DALI_COMPOSE_SHADER(
    attribute mediump vec2 aPosition;\n
    varying mediump vec2 vTexCoord;\n
    uniform mediump mat4 uModelMatrix;\n
    uniform mediump mat4 uMvpMatrix;\n
    uniform mediump vec3 uSize;\n
    uniform mediump vec2 uFixed[ 3 ];\n
    uniform mediump vec2 uStretchTotal;\n
    \n
    void main()\n
    {\n
      mediump vec2 scale        = vec2( length( uModelMatrix[ 0 ].xyz ), length( uModelMatrix[ 1 ].xyz ) );\n
      mediump vec2 size         = uSize.xy * scale;\n
      \n
      mediump vec2 fixedFactor  = vec2( uFixed[ int( ( aPosition.x + 1.0 ) * 0.5 ) ].x, uFixed[ int( ( aPosition.y  + 1.0 ) * 0.5 ) ].y );\n
      mediump vec2 stretch      = floor( aPosition * 0.5 );\n
      mediump vec2 fixedTotal   = uFixed[ 2 ];\n
      \n
      mediump vec4 vertexPosition = vec4( fixedFactor + ( size - fixedTotal ) * stretch, 0.0, 1.0 );\n
      vertexPosition.xy -= size * vec2( 0.5, 0.5 );\n
      vertexPosition.xy =  vertexPosition.xy / scale;\n
      \n
      vertexPosition = uMvpMatrix * vertexPosition;\n
      \n
      vTexCoord = ( fixedFactor + stretch * uStretchTotal ) / ( fixedTotal + uStretchTotal );\n
      \n
      gl_Position = vertexPosition;\n
    }\n
);

const char* FRAGMENT_SHADER = DALI_COMPOSE_SHADER(
  varying mediump vec2 vTexCoord;\n
  uniform sampler2D sTexture;\n
  uniform lowp vec4 uColor;\n
  \n
  void main()\n
  {\n
    gl_FragColor = texture2D( sTexture, vTexCoord ) * uColor;\n
  }\n
);

/**
 * @brief Creates the geometry formed from the vertices and indices
 *
 * @param[in]  vertices             The vertices to generate the geometry from
 * @param[in]  indices              The indices to generate the geometry from
 * @return The geometry formed from the vertices and indices
 */
Geometry GenerateGeometry( const Vector< Vector2 >& vertices, const Vector< unsigned int >& indices )
{
  Property::Map vertexFormat;
  vertexFormat[ "aPosition" ] = Property::VECTOR2;
  PropertyBuffer vertexPropertyBuffer = PropertyBuffer::New( vertexFormat, vertices.Size() );
  if( vertices.Size() > 0 )
  {
    vertexPropertyBuffer.SetData( &vertices[ 0 ] );
  }

  Property::Map indexFormat;
  indexFormat[ "indices" ] = Property::INTEGER;
  PropertyBuffer indexPropertyBuffer = PropertyBuffer::New( indexFormat, indices.Size() );
  if( indices.Size() > 0 )
  {
    indexPropertyBuffer.SetData( &indices[ 0 ] );
  }

  // Create the geometry object
  Geometry geometry = Geometry::New();
  geometry.AddVertexBuffer( vertexPropertyBuffer );
  geometry.SetIndexBuffer( indexPropertyBuffer );

  return geometry;
}

/**
 * @brief Adds the indices to form a quad composed off two triangles where the indices are organised in a grid
 *
 * @param[out] indices     The indices to add to
 * @param[in]  rowIdx      The row index to start the quad
 * @param[in]  nextRowIdx  The index to the next row
 */
void AddQuadIndices( Vector< unsigned int >& indices, unsigned int rowIdx, unsigned int nextRowIdx )
{
  indices.PushBack( rowIdx );
  indices.PushBack( nextRowIdx + 1 );
  indices.PushBack( rowIdx + 1 );

  indices.PushBack( rowIdx );
  indices.PushBack( nextRowIdx );
  indices.PushBack( nextRowIdx + 1 );
}

void AddVertex( Vector< Vector2 >& vertices, unsigned int x, unsigned int y )
{
  vertices.PushBack( Vector2( x, y ) );
}

void RegisterStretchProperties( Material& material, const char * uniformName, const NinePatchImage::StretchRanges& stretchPixels, uint16_t imageExtent)
{
  uint16_t prevEnd = 0;
  uint16_t prevFix = 0;
  uint16_t prevStretch = 0;
  unsigned int i = 1;
  for( NinePatchImage::StretchRanges::ConstIterator it = stretchPixels.Begin(); it != stretchPixels.End(); ++it, ++i )
  {
    uint16_t start = it->GetX();
    uint16_t end = it->GetY();

    uint16_t fix = prevFix + start - prevEnd;
    uint16_t stretch = prevStretch + end - start;

    std::stringstream uniform;
    uniform << uniformName << "[" << i << "]";
    material.RegisterProperty( uniform.str(), Vector2( fix, stretch ) );

    prevEnd = end;
    prevFix = fix;
    prevStretch = stretch;
  }

  {
    prevFix += imageExtent - prevEnd;
    std::stringstream uniform;
    uniform << uniformName << "[" << i << "]";
    material.RegisterProperty( uniform.str(), Vector2( prevFix, prevStretch ) );
  }
}

} //unnamed namespace

/////////////////NPatchRenderer////////////////

NPatchRenderer::NPatchRenderer( RendererFactoryCache& factoryCache )
: ControlRenderer( factoryCache ),
  mBorderOnly( false )
{
}

NPatchRenderer::~NPatchRenderer()
{
}

void NPatchRenderer::DoInitialize( Actor& actor, const Property::Map& propertyMap )
{
  Property::Value* imageURLValue = propertyMap.Find( IMAGE_URL_NAME );
  if( imageURLValue )
  {
    //Read the border-only property first since InitialiseFromImage relies on mBorderOnly to be properly set
    Property::Value* borderOnlyValue = propertyMap.Find( BORDER_ONLY );
    if( borderOnlyValue )
    {
      borderOnlyValue->Get( mBorderOnly );
    }

    if( imageURLValue->Get( mImageUrl ) )
    {
      NinePatchImage nPatch = NinePatchImage::New( mImageUrl );
      InitializeFromImage( nPatch );
    }
    else
    {
      InitializeFromBrokenImage();
      DALI_LOG_ERROR( "The property '%s' is not a string\n", IMAGE_URL_NAME );
    }
  }
}

void NPatchRenderer::GetNaturalSize( Vector2& naturalSize ) const
{
  if( mImage )
  {
    naturalSize.x = mImage.GetWidth();
    naturalSize.y = mImage.GetHeight();
  }
  else if( !mImageUrl.empty() )
  {
    ImageDimensions dimentions = ResourceImage::GetImageSize( mImageUrl );
    naturalSize.x = dimentions.GetWidth();
    naturalSize.y = dimentions.GetHeight();
  }
  else
  {
    naturalSize = Vector2::ZERO;
  }
}

void NPatchRenderer::SetClipRect( const Rect<int>& clipRect )
{
  ControlRenderer::SetClipRect( clipRect );
  //ToDo: renderer responds to the clipRect change
}

void NPatchRenderer::SetOffset( const Vector2& offset )
{
  //ToDo: renderer applies the offset
}

Geometry NPatchRenderer::CreateGeometry()
{
  Geometry geometry;
  if( mStretchPixelsX.Size() == 1 && mStretchPixelsY.Size() == 1 )
  {
    if( !mBorderOnly )
    {
      geometry = mFactoryCache.GetGeometry( RendererFactoryCache::NINE_PATCH_GEOMETRY );
      if( !geometry )
      {
        geometry = CreateGeometry( Uint16Pair( 3, 3 ) );
        mFactoryCache.SaveGeometry( RendererFactoryCache::NINE_PATCH_GEOMETRY, geometry );
      }
    }
    else
    {
      geometry = mFactoryCache.GetGeometry( RendererFactoryCache::NINE_PATCH_BORDER_GEOMETRY );
      if( !geometry )
      {
        geometry = CreateGeometryBorder( Uint16Pair( 3, 3 ) );
        mFactoryCache.SaveGeometry( RendererFactoryCache::NINE_PATCH_BORDER_GEOMETRY, geometry );
      }
    }
  }
  else if( mStretchPixelsX.Size() > 0 || mStretchPixelsY.Size() > 0)
  {
    Uint16Pair gridSize( 2 * mStretchPixelsX.Size() + 1,  2 * mStretchPixelsY.Size() + 1 );
    geometry = !mBorderOnly ? CreateGeometry( gridSize ) : CreateGeometryBorder( gridSize );
  }

  return geometry;
}

Shader NPatchRenderer::CreateShader()
{
  Shader shader;
  if( mStretchPixelsX.Size() == 1 && mStretchPixelsY.Size() == 1 )
  {
    shader = mFactoryCache.GetShader( RendererFactoryCache::NINE_PATCH_SHADER );
    if( !shader )
    {
      shader = Shader::New( VERTEX_SHADER_3X3, FRAGMENT_SHADER );
      mFactoryCache.SaveShader( RendererFactoryCache::NINE_PATCH_SHADER, shader );
    }
  }
  else if( mStretchPixelsX.Size() > 0 || mStretchPixelsY.Size() > 0)
  {
    std::stringstream vertexShader;
    vertexShader << "#define FACTOR_SIZE_X " << mStretchPixelsX.Size() + 2 << "\n"
                 << "#define FACTOR_SIZE_Y " << mStretchPixelsY.Size() + 2 << "\n"
                 << VERTEX_SHADER;

    shader = Shader::New( vertexShader.str(), FRAGMENT_SHADER );
  }
  return shader;
}

void NPatchRenderer::InitializeRenderer()
{
  Geometry geometry = CreateGeometry();
  Shader shader = CreateShader();

  if( !geometry || !shader )
  {
    DALI_LOG_ERROR("The 9 patch image '%s' doesn't have any valid stretch borders and so is not a valid 9 patch image\n", mImageUrl.c_str() );
    InitializeFromBrokenImage();
  }

  Material material = Material::New( shader );
  mImpl->mRenderer = Renderer::New( geometry, material );
}


void NPatchRenderer::DoSetOnStage( Actor& actor )
{
  if( !mCroppedImage )
  {
    if( !mImageUrl.empty() )
    {
      NinePatchImage nPatch = NinePatchImage::New( mImageUrl );
      InitializeFromImage( nPatch );
    }
    else if( mImage )
    {
      InitializeFromImage( mImage );
    }
  }

  //initialize the renderer after initializing from the image since we need to know the grid size from the image before creating the geometry
  InitializeRenderer();

  if( mCroppedImage )
  {
    ApplyImageToSampler();
  }
}

void NPatchRenderer::DoSetOffStage( Actor& actor )
{
  mCroppedImage.Reset();
}

void NPatchRenderer::DoCreatePropertyMap( Property::Map& map ) const
{
  map.Clear();
  map.Insert( RENDERER_TYPE, RENDERER_TYPE_VALUE );
  if( !mImageUrl.empty() )
  {
    map.Insert( IMAGE_URL_NAME, mImageUrl );
  }
  else if( mImage )
  {
    map.Insert( IMAGE_URL_NAME, mImage.GetUrl() );
  }
  map.Insert( BORDER_ONLY, mBorderOnly );
}

void NPatchRenderer::ChangeRenderer( bool oldBorderOnly, size_t oldGridX, size_t oldGridY )
{
  //check to see if the border style has changed

  bool borderOnlyChanged = oldBorderOnly != mBorderOnly;
  bool gridChanged = oldGridX != mStretchPixelsX.Size() || oldGridY != mStretchPixelsY.Size();

  if( borderOnlyChanged || gridChanged )
  {
    Geometry geometry = CreateGeometry();
    if( geometry )
    {
      mImpl->mRenderer.SetGeometry( geometry );
    }
    else
    {
      InitializeFromBrokenImage();
    }
  }

  if( gridChanged )
  {
    Shader shader = CreateShader();
    Material material;
    if( shader )
    {
      material = mImpl->mRenderer.GetMaterial();
      if( material )
      {
        material.SetShader( shader );
      }
    }

    if( !material )
    {
      InitializeFromBrokenImage();
    }
  }
}

void NPatchRenderer::SetImage( const std::string& imageUrl, bool borderOnly )
{
  bool oldBorderOnly = mBorderOnly;
  size_t oldGridX = mStretchPixelsX.Size();
  size_t oldGridY = mStretchPixelsY.Size();

  mBorderOnly = borderOnly;
  mImage.Reset();
  if( mImageUrl == imageUrl )
  {
    return;
  }

  mImageUrl = imageUrl;
  if( mImpl->mRenderer )
  {
    NinePatchImage nPatch = NinePatchImage::New( mImageUrl );
    InitializeFromImage( nPatch );

    ChangeRenderer( oldBorderOnly, oldGridX, oldGridY );

    if( mCroppedImage )
    {
      ApplyImageToSampler();
    }
  }
}

void NPatchRenderer::SetImage( NinePatchImage image, bool borderOnly )
{
  bool oldBorderOnly = mBorderOnly;
  size_t oldGridX = mStretchPixelsX.Size();
  size_t oldGridY = mStretchPixelsY.Size();

  mBorderOnly = borderOnly;
  mImageUrl.empty();
  if( mImage == image )
  {
    return;
  }

  mImage = image;
  if( mImpl->mRenderer )
  {
    InitializeFromImage( mImage );
    ChangeRenderer( oldBorderOnly, oldGridX, oldGridY );

    if( mCroppedImage )
    {
      ApplyImageToSampler();
    }
  }
}

void NPatchRenderer::InitializeFromImage( NinePatchImage nPatch )
{
  mCroppedImage = nPatch.CreateCroppedBufferImage();
  if( !mCroppedImage )
  {
    DALI_LOG_ERROR("'%s' specify a valid 9 patch image\n", mImageUrl.c_str() );
    InitializeFromBrokenImage();
    return;
  }

  mImageSize = ImageDimensions( mCroppedImage.GetWidth(), mCroppedImage.GetHeight() );

  mStretchPixelsX = nPatch.GetStretchPixelsX();
  mStretchPixelsY = nPatch.GetStretchPixelsY();
}

void NPatchRenderer::InitializeFromBrokenImage()
{
  mCroppedImage = RendererFactory::GetBrokenRendererImage();
  mImageSize = ImageDimensions( mCroppedImage.GetWidth(), mCroppedImage.GetHeight() );

  mStretchPixelsX.Clear();
  mStretchPixelsX.PushBack( Uint16Pair( 0, mImageSize.GetWidth() ) );
  mStretchPixelsY.Clear();
  mStretchPixelsY.PushBack( Uint16Pair( 0, mImageSize.GetHeight() ) );
}

void NPatchRenderer::ApplyImageToSampler()
{
  Material material = mImpl->mRenderer.GetMaterial();
  if( material )
  {
    int index = material.GetTextureIndex( TEXTURE_UNIFORM_NAME );
    if( index > -1 )
    {
      material.SetTextureImage( index, mCroppedImage );
    }
    else
    {
      material.AddTexture(  mCroppedImage, TEXTURE_UNIFORM_NAME );
    }

    if( mStretchPixelsX.Size() == 1 && mStretchPixelsY.Size() == 1 )
    {
      //special case for 9 patch
      Uint16Pair stretchX = mStretchPixelsX[ 0 ];
      Uint16Pair stretchY = mStretchPixelsY[ 0 ];

      uint16_t stretchWidth = stretchX.GetY() - stretchX.GetX();
      uint16_t stretchHeight = stretchY.GetY() - stretchY.GetX();

      material.RegisterProperty( "uFixed[0]", Vector2::ZERO );
      material.RegisterProperty( "uFixed[1]", Vector2( stretchX.GetX(), stretchY.GetX()) );
      material.RegisterProperty( "uFixed[2]", Vector2( mImageSize.GetWidth() - stretchWidth, mImageSize.GetHeight() - stretchHeight ) );
      material.RegisterProperty( "uStretchTotal", Vector2( stretchWidth, stretchHeight ) );
    }
    else
    {
      material.RegisterProperty( "uNinePatchFactorsX[0]", Vector2::ZERO );
      material.RegisterProperty( "uNinePatchFactorsY[0]", Vector2::ZERO );

      RegisterStretchProperties( material, "uNinePatchFactorsX", mStretchPixelsX, mImageSize.GetWidth() );
      RegisterStretchProperties( material, "uNinePatchFactorsY", mStretchPixelsY, mImageSize.GetHeight() );
    }
  }
}

Geometry NPatchRenderer::CreateGeometry( Uint16Pair gridSize )
{
  uint16_t gridWidth = gridSize.GetWidth();
  uint16_t gridHeight = gridSize.GetHeight();

  // Create vertices
  Vector< Vector2 > vertices;
  vertices.Reserve( ( gridWidth + 1 ) * ( gridHeight + 1 ) );

  for( int y = 0; y < gridHeight + 1; ++y )
  {
    for( int x = 0; x < gridWidth + 1; ++x )
    {
      AddVertex( vertices, x, y );
    }
  }

  // Create indices
  //TODO: compare performance with triangle strip when Geometry supports it
  Vector< unsigned int > indices;
  indices.Reserve( gridWidth * gridHeight * 6 );

  unsigned int rowIdx     = 0;
  unsigned int nextRowIdx = gridWidth + 1;
  for( int y = 0; y < gridHeight; ++y, ++nextRowIdx, ++rowIdx )
  {
    for( int x = 0; x < gridWidth; ++x, ++nextRowIdx, ++rowIdx )
    {
      AddQuadIndices( indices, rowIdx, nextRowIdx );
    }
  }

  return GenerateGeometry( vertices, indices );
}

Geometry NPatchRenderer::CreateGeometryBorder( Uint16Pair gridSize )
{
  uint16_t gridWidth = gridSize.GetWidth();
  uint16_t gridHeight = gridSize.GetHeight();

  // Create vertices
  Vector< Vector2 > vertices;
  vertices.Reserve( ( gridWidth + 1 ) * ( gridHeight + 1 ) );

  //top
  int y = 0;
  for(; y < 2; ++y)
  {
    for( int x = 0; x < gridWidth + 1; ++x )
    {
      AddVertex( vertices, x, y );
    }
  }

  for(; y < gridHeight - 1; ++y)
  {
    //left
    AddVertex( vertices, 0, y );
    AddVertex( vertices, 1, y );

    //right
    AddVertex( vertices, gridWidth - 1, y );
    AddVertex( vertices, gridWidth, y );
  }

  //bottom
  for(; y < gridHeight + 1; ++y)
  {
    for( int x = 0; x < gridWidth + 1; ++x )
    {
      AddVertex( vertices, x, y );
    }
  }

  // Create indices
  //TODO: compare performance with triangle strip when Geometry supports it
  Vector< unsigned int > indices;
  indices.Reserve( gridWidth * gridHeight * 6 );

  //top
  unsigned int rowIdx     = 0 ;
  unsigned int nextRowIdx = gridWidth + 1;
  for( int x = 0; x < gridWidth; ++x, ++nextRowIdx, ++rowIdx )
  {
    AddQuadIndices( indices, rowIdx, nextRowIdx );
  }

  if(gridHeight > 2)
  {
    rowIdx     = gridWidth + 1;
    nextRowIdx = ( gridWidth + 1 ) * 2;

    unsigned increment = gridWidth - 1;
    if(gridHeight > 3)
    {
      increment = 2;
      //second row left
      AddQuadIndices( indices, rowIdx, nextRowIdx );

      rowIdx     = gridWidth * 2;
      nextRowIdx = ( gridWidth + 1 ) * 2 + 2;
      //second row right
      AddQuadIndices( indices, rowIdx, nextRowIdx );

      //left and right
      rowIdx     = nextRowIdx - 2;
      nextRowIdx = rowIdx + 4;
      for(int y = 2; y < 2*(gridHeight - 3); ++y, rowIdx += 2, nextRowIdx += 2)
      {
        AddQuadIndices( indices, rowIdx, nextRowIdx );
      }
    }

    //second row left
    AddQuadIndices( indices, rowIdx, nextRowIdx );

    rowIdx     += increment;
    nextRowIdx += gridWidth - 1;
    //second row right
    AddQuadIndices( indices, rowIdx, nextRowIdx );
  }

  //bottom
  rowIdx     = nextRowIdx - gridWidth + 1;
  nextRowIdx = rowIdx + gridWidth + 1;
  for( int x = 0; x < gridWidth; ++x, ++nextRowIdx, ++rowIdx )
  {
    AddQuadIndices( indices, rowIdx, nextRowIdx );
  }

  return GenerateGeometry( vertices, indices );
}

} // namespace Internal

} // namespace Toolkit

} // namespace Dali
