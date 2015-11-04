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

#ifndef DALI_INTERNAL_PLATFORM_IMAGE_OPERATIONS_H_
#define DALI_INTERNAL_PLATFORM_IMAGE_OPERATIONS_H_

// EXTERNAL INCLUDES
#include <stdint.h>

// INTERNAL INCLUDES
#include <dali/integration-api/bitmap.h>
#include <dali/public-api/images/image-operations.h>

namespace Dali
{
namespace Internal
{
namespace Platform
{

/**
 * @brief Identify which combination of x and y dimensions matter in terminating iterative box filtering.
 */
enum BoxDimensionTest
{
  BoxDimensionTestEither,
  BoxDimensionTestBoth,
  BoxDimensionTestX,
  BoxDimensionTestY
};

/**
 * @brief The integer dimensions of an image or a region of an image packed into
 *        16 bits per component.
 * @note  This can only be used for images of up to 65535 x 65535 pixels.
  */
typedef Uint16Pair ImageDimensions;

/**
 * @brief Work out the true desired width and height, accounting for special
 * rules for zeros in either or both input requested dimensions.
 *
 * @param[in] rawDimensions Width and height of image before processing.
 * @param[in] requestedDimensions Width and height of area to scale image into. Can be zero.
 * @return Dimensions of area to scale image into after special rules are applied.
 */
ImageDimensions CalculateDesiredDimensions( ImageDimensions rawDimensions, ImageDimensions requestedDimensions );

/**
 * @defgroup BitmapOperations Bitmap-to-Bitmap Image operations.
 * @{
 */

/**
 * @brief Apply requested attributes to bitmap.
 *
 * This is the top-level function which runs the on-load image post-processing
 * pipeline. Bitmaps enter here as loaded from the file system by the file
 * loaders and leave downscaled and filtered as requested by the application,
 * ready for use.
 *
 * @param[in] bitmap The input bitmap.
 * @param[in] requestedAttributes Attributes which should be applied to bitmap.
 * @return A bitmap which results from applying the requested attributes to the
 *         bitmap passed-in, or the original bitmap passed in if the attributes
 *         have no effect.
 */
Integration::BitmapPtr ApplyAttributesToBitmap( Integration::BitmapPtr bitmap, ImageDimensions dimensions, FittingMode::Type fittingMode = FittingMode::DEFAULT, SamplingMode::Type samplingMode = SamplingMode::DEFAULT );

/**
 * @brief Apply downscaling to a bitmap according to requested attributes.
 * @note The input bitmap pixel buffer may be modified and used as scratch working space for efficiency, so it must be discarded.
 **/
Integration::BitmapPtr DownscaleBitmap( Integration::Bitmap& bitmap, ImageDimensions desired, FittingMode::Type fittingMode, SamplingMode::Type samplingMode );
/**@}*/

/**
 * @defgroup ImageBufferScalingAlgorithms Pixel buffer-level scaling algorithms.
 * @{
 */

/**
 * @brief Destructive in-place downscaling by a power of 2 factor.
 *
 * A box filter with a 2x2 kernel is repeatedly applied as long as the result
 * of the next downscaling step would not be smaller than the desired
 * dimensions.
 * @param[in,out] pixels The buffer both to read from and write the result to.
 * @param[in]     pixelFormat The format of the image pointed at by pixels.
 * @param[in]     inputWidth The width of the input image.
 * @param[in]     inputHeight The height of the input image.
 * @param[in]     desiredWidth The width the client is requesting.
 * @param[in]     desiredHeight The height the client is requesting.
 * @param[out]    outWidth  The resulting width after downscaling.
 * @param[out]    outHeight The resulting height after downscaling.
 */
void DownscaleInPlacePow2( unsigned char * const pixels,
                           Pixel::Format pixelFormat,
                           unsigned int inputWidth,
                           unsigned int inputHeight,
                           unsigned int desiredWidth,
                           unsigned int desiredHeight,
                           FittingMode::Type fittingMode,
                           SamplingMode::Type samplingMode,
                           unsigned& outWidth,
                           unsigned& outHeight );

/**
 * @brief Destructive in-place downscaling by a power of 2 factor.
 *
 * A box filter with a 2x2 kernel is repeatedly applied as long as the result
 * of the next downscaling step would not be smaller than the desired
 * dimensions.
 * @param[in,out] pixels The buffer both to read from and write the result to.
 * @param[in]     inputWidth The width of the input image.
 * @param[in]     inputHeight The height of the input image.
 * @param[in]     desiredWidth The width the client is requesting.
 * @param[in]     desiredHeight The height the client is requesting.
 * @param[out]    outWidth  The resulting width after downscaling.
 * @param[out]    outHeight The resulting height after downscaling.
 */
void DownscaleInPlacePow2RGB888( unsigned char * pixels,
                                 unsigned int inputWidth,
                                 unsigned int inputHeight,
                                 unsigned int desiredWidth,
                                 unsigned int desiredHeight,
                                 BoxDimensionTest dimensionTest,
                                 unsigned int& outWidth,
                                 unsigned int& outHeight );

/**
 * @copydoc DownscaleInPlacePow2RGB888
 */
void DownscaleInPlacePow2RGBA8888( unsigned char * pixels,
                                   unsigned int inputWidth,
                                   unsigned int inputHeight,
                                   unsigned int desiredWidth,
                                   unsigned int desiredHeight,
                                   BoxDimensionTest dimensionTest,
                                   unsigned int& outWidth,
                                   unsigned int& outHeight );

/**
 * @copydoc DownscaleInPlacePow2RGB888
 *
 * For the 2-byte packed 16 bit format RGB565.
 */
void DownscaleInPlacePow2RGB565( unsigned char * pixels,
                                 unsigned int inputWidth,
                                 unsigned int inputHeight,
                                 unsigned int desiredWidth,
                                 unsigned int desiredHeight,
                                 BoxDimensionTest dimensionTest,
                                 unsigned int& outWidth,
                                 unsigned int& outHeight );

/**
 * @copydoc DownscaleInPlacePow2RGB888
 *
 * For 2-byte formats such as lum8alpha8, but not packed 16 bit formats like RGB565.
 */
void DownscaleInPlacePow2ComponentPair( unsigned char * pixels,
                                        unsigned int inputWidth,
                                        unsigned int inputHeight,
                                        unsigned int desiredWidth,
                                        unsigned int desiredHeight,
                                        BoxDimensionTest dimensionTest,
                                        unsigned int& outWidth,
                                        unsigned int& outHeight );

/**
 * @copydoc DownscaleInPlacePow2RGB888
 *
 * For single-byte formats such as lum8 or alpha8.
 */
void DownscaleInPlacePow2SingleBytePerPixel( unsigned char * pixels,
                                             unsigned int inputWidth,
                                             unsigned int inputHeight,
                                             unsigned int desiredWidth,
                                             unsigned int desiredHeight,
                                             BoxDimensionTest dimensionTest,
                                             unsigned int& outWidth,
                                             unsigned int& outHeight );

/**
 * @brief Rescales an input image into the exact output dimensions passed-in.
 *
 * Uses point sampling, equivalent to GL_NEAREST texture filter mode, for the
 * fastest results, at the expense of aliasing (noisy images) when downscaling.
 * @note inPixels is allowed to alias outPixels if this is a downscaling,
 * but not for upscaling.
 */
void PointSample( const unsigned char * inPixels,
                  unsigned int inputWidth,
                  unsigned int inputHeight,
                  Pixel::Format pixelFormat,
                  unsigned char * outPixels,
                  unsigned int desiredWidth,
                  unsigned int desiredHeight );

/**
 * @copydoc PointSample
 *
 * Specialised for 4-byte formats like RGBA8888 and BGRA8888.
 */
void PointSample4BPP( const unsigned char * inPixels,
                      unsigned int inputWidth,
                      unsigned int inputHeight,
                      unsigned char * outPixels,
                      unsigned int desiredWidth,
                      unsigned int desiredHeight );

/**
 * @copydoc PointSample
 *
 * Specialised for 3-byte formats like RGB888 and BGR888.
 */
void PointSample3BPP( const unsigned char * inPixels,
                      unsigned int inputWidth,
                      unsigned int inputHeight,
                      unsigned char * outPixels,
                      unsigned int desiredWidth,
                      unsigned int desiredHeight );

/**
 * @copydoc PointSample
 *
 * Specialised for 2-byte formats like LA88.
 */
void PointSample2BPP( const unsigned char * inPixels,
                      unsigned int inputWidth,
                      unsigned int inputHeight,
                      unsigned char * outPixels,
                      unsigned int desiredWidth,
                      unsigned int desiredHeight );

/**
 * @copydoc PointSample
 *
 * Specialised for 1-byte formats like L8 and A8.
 */
void PointSample1BPP( const unsigned char * inPixels,
                      unsigned int inputWidth,
                      unsigned int inputHeight,
                      unsigned char * outPixels,
                      unsigned int desiredWidth,
                      unsigned int desiredHeight );

/**
 * @brief Resample input image to output image using a bilinear filter.
 *
 * Each output pixel is formed of a weighted sum of a 2x2 block of four input
 * pixels
 * @pre inPixels must not alias outPixels. The input image should be a totally
 * separate buffer from the input one.
 */
void LinearSample( const unsigned char * __restrict__ inPixels,
                   ImageDimensions inDimensions,
                   Pixel::Format pixelFormat,
                   unsigned char * __restrict__ outPixels,
                   ImageDimensions outDimensions );

/**
 * @copydoc LinearSample
 *
 * Specialised for one byte per pixel formats.
 */
void LinearSample1BPP( const unsigned char * __restrict__ inPixels,
                       ImageDimensions inputDimensions,
                       unsigned char * __restrict__ outPixels,
                       ImageDimensions desiredDimensions );

/**
 * @copydoc LinearSample
 *
 * Specialised for two byte per pixel formats.
 */
void LinearSample2BPP( const unsigned char * __restrict__ inPixels,
                       ImageDimensions inputDimensions,
                       unsigned char * __restrict__ outPixels,
                       ImageDimensions desiredDimensions );

/**
 * @copydoc LinearSample
 *
 * Specialised for RGB565 16 bit pixel format.
 */
void LinearSampleRGB565( const unsigned char * __restrict__ inPixels,
                       ImageDimensions inputDimensions,
                       unsigned char * __restrict__ outPixels,
                       ImageDimensions desiredDimensions );

/**
 * @copydoc LinearSample
 *
 * Specialised for three byte per pixel formats like RGB888.
 */
void LinearSample3BPP( const unsigned char * __restrict__ inPixels,
                       ImageDimensions inputDimensions,
                       unsigned char * __restrict__ outPixels,
                       ImageDimensions desiredDimensions );

/**
 * @copydoc LinearSample
 *
 * Specialised for four byte per pixel formats like RGBA888.
 * @note, If used on RGBA8888, the A component will be blended independently.
 */
void LinearSample4BPP( const unsigned char * __restrict__ inPixels,
                       ImageDimensions inputDimensions,
                       unsigned char * __restrict__ outPixels,
                       ImageDimensions desiredDimensions );

/**@}*/

/**
 * @defgroup ScalingAlgorithmFragments Composable subunits of the scaling algorithms.
 * @{
 */

/**
 * @brief Average adjacent pairs of pixels, overwriting the input array.
 * @param[in,out] pixels The array of pixels to work on.
 * @param[i]      width  The number of pixels in the array passed-in.
 */
void HalveScanlineInPlaceRGB888( unsigned char * pixels, unsigned int width );

/**
 * @copydoc HalveScanlineInPlaceRGB888
 */
void HalveScanlineInPlaceRGBA8888( unsigned char * pixels, unsigned int width );

/**
 * @copydoc HalveScanlineInPlaceRGB888
 */
void HalveScanlineInPlaceRGB565( unsigned char * pixels, unsigned int width );

/**
 * @copydoc HalveScanlineInPlaceRGB888
 */
void HalveScanlineInPlace2Bytes( unsigned char * pixels, unsigned int width );

/**
 * @copydoc HalveScanlineInPlaceRGB888
 */
void HalveScanlineInPlace1Byte( unsigned char * pixels, unsigned int width );

/**
 * @brief Average pixels at corresponding offsets in two scanlines.
 *
 * outputScanline is allowed to alias scanline1.
 * @param[in] scanline1 First scanline of pixels to average.
 * @param[in] scanline2 Second scanline of pixels to average.
 * @param[out] outputScanline Destination for the averaged pixels.
 * @param[in] width The widths of all the scanlines passed-in.
 */
void AverageScanlines1( const unsigned char * scanline1,
                        const unsigned char * scanline2,
                        unsigned char* outputScanline,
                        /** Image width in pixels (1 byte == 1 pixel: e.g. lum8 or alpha8).*/
                        unsigned int width );

/**
 * @copydoc AverageScanlines1
 */
void AverageScanlines2( const unsigned char * scanline1,
                        const unsigned char * scanline2,
                        unsigned char* outputScanline,
                        /** Image width in pixels (2 bytes == 1 pixel: e.g. lum8alpha8).*/
                        unsigned int width );

/**
 * @copydoc AverageScanlines1
 */
void AverageScanlines3( const unsigned char * scanline1,
                        const unsigned char * scanline2,
                        unsigned char* outputScanline,
                        /** Image width in pixels (3 bytes == 1 pixel: e.g. RGB888).*/
                        unsigned int width );

/**
 * @copydoc AverageScanlines1
 */
void AverageScanlinesRGBA8888( const unsigned char * scanline1,
                               const unsigned char * scanline2,
                               unsigned char * outputScanline,
                               unsigned int width );

/**
 * @copydoc AverageScanlines1
 */
void AverageScanlinesRGB565( const unsigned char * scanline1,
                             const unsigned char * scanline2,
                             unsigned char* outputScanline,
                             unsigned int width );
/**@}*/

/**
 * @defgroup TestableInlines Inline functions exposed in header to allow unit testing.
 * @{
 */

/**
 * @brief Average two integer arguments.
 * @return The average of two uint arguments.
 * @param[in] a First component to average.
 * @param[in] b Second component to average.
 **/
inline unsigned int AverageComponent( unsigned int a, unsigned int b )
{
  unsigned int avg = (a + b) >> 1u;
  return avg;
}

/**
 * @brief Average a pair of RGBA8888 pixels.
 * @return The average of two RGBA8888 pixels.
 * @param[in] a First pixel to average.
 * @param[in] b Second pixel to average
 **/
inline uint32_t AveragePixelRGBA8888( uint32_t a, uint32_t b )
{
  const unsigned int avg =
    ((AverageComponent( (a & 0xff000000) >> 1u, (b & 0xff000000) >> 1u ) << 1u) & 0xff000000 ) +
    (AverageComponent( a & 0x00ff0000, b & 0x00ff0000 ) & 0x00ff0000 ) +
    (AverageComponent( a & 0x0000ff00, b & 0x0000ff00 ) & 0x0000ff00 ) +
    (AverageComponent( a & 0x000000ff, b & 0x000000ff ) );
  return avg;
  ///@ToDo: Optimise by trying return (((a ^ b) & 0xfefefefeUL) >> 1) + (a & b);
  ///@ToDo: Optimise for ARM using the single ARMV6 instruction: UHADD8  R4, R0, R5. This is not Neon. It runs in the normal integer pipeline so there is no downside like a stall moving between integer and copro.
}

/**
 * @brief Average a pair of RGB565 pixels.
 * @param a[in] Low 16 bits hold a color value as RGB565 to average with parameter b.
 * @param b[in] Low 16 bits hold a color value as RGB565 to average with parameter a.
 * @return The average color of the two RGB565 pixels passed in, in the low 16 bits of the returned value.
 **/
inline uint32_t AveragePixelRGB565( uint32_t a, uint32_t b )
{
  const unsigned int avg =
    (AverageComponent( a & 0xf800, b & 0xf800 ) & 0xf800 ) +
    (AverageComponent( a & 0x7e0,  b & 0x7e0 )  & 0x7e0 ) +
    (AverageComponent( a & 0x1f,   b & 0x1f ) );
  return avg;
}

/** @return The weighted blend of two integers as a 16.16 fixed-point number, given a 0.16 fixed-point blending factor. */
inline unsigned int WeightedBlendIntToFixed1616(unsigned int a, unsigned int b, unsigned int fractBlend )
{
  DALI_ASSERT_DEBUG( fractBlend <= 65535u && "Factor should be in 0.16 fixed-point." );
  const unsigned int weightedAFixed = a * (65535u - fractBlend);
  const unsigned int weightedBFixed = b * fractBlend;
  const unsigned blended = (weightedAFixed + weightedBFixed);
  return blended;
}

/** @brief Blend two 16.16 inputs to give a 16.32 output. */
inline uint64_t WeightedBlendFixed1616ToFixed1632(unsigned int a, unsigned int b, unsigned int fractBlend )
{
  DALI_ASSERT_DEBUG( fractBlend <= 65535u && "Factor should be in 0.16 fixed-point." );
  // Blend while promoting intermediates to 16.32 fixed point:
  const uint64_t weightedAFixed = uint64_t(a) * (65535u - fractBlend);
  const uint64_t weightedBFixed = uint64_t(b) * fractBlend;
  const uint64_t blended = (weightedAFixed + weightedBFixed);
  return blended;
}

/**
 * @brief Blend 4 taps into one value using horizontal and vertical weights.
 */
inline unsigned int BilinearFilter1Component(unsigned int tl, unsigned int tr, unsigned int bl, unsigned int br, unsigned int fractBlendHorizontal, unsigned int fractBlendVertical )
{
  DALI_ASSERT_DEBUG( fractBlendHorizontal <= 65535u && "Factor should be in 0.16 fixed-point." );
  DALI_ASSERT_DEBUG( fractBlendVertical   <= 65535u && "Factor should be in 0.16 fixed-point." );

  const unsigned int topBlend = WeightedBlendIntToFixed1616( tl, tr, fractBlendHorizontal );
  const unsigned int botBlend = WeightedBlendIntToFixed1616( bl, br, fractBlendHorizontal );
  const uint64_t blended2x2 = WeightedBlendFixed1616ToFixed1632( topBlend, botBlend, fractBlendVertical );
  const unsigned int rounded = (blended2x2 + (1u << 31u) ) >> 32u;
  return rounded;
}

/**@}*/

} /* namespace Platform */
} /* namespace Internal */
} /* namespace Dali */

#endif /* DALI_INTERNAL_PLATFORM_IMAGE_OPERATIONS_H_ */
