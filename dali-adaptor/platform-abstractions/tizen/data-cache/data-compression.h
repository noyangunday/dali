#ifndef __DALI_TIZEN_PLATFORM_DATA_COMPRESSION_H__
#define __DALI_TIZEN_PLATFORM_DATA_COMPRESSION_H__

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


namespace Dali
{
namespace TizenPlatform
{
namespace DataCompression
{

/**
 * Return the maximum size of a buffer required to hold a number of bytes.
 * Required because compressing data, can end up being bigger
 * than the original data.
 * @return maximum buffer size required to compress inputSize
 */
std::size_t GetMaximumRleCompressedSize(const std::size_t inputSize);

/**
 * RLE Encodes an array of data
 * @param input input data
 * @param inputLength input data length in bytes
 * @param output output data, pre-allocated by caller
 * @param outputLength size of the output buffer in bytes
 * @param encodedSize number of bytes written to outputBuffer
 */
void EncodeRle(  const unsigned char* input,
                 const std::size_t inputLength,
                 unsigned char* output,
                 const std::size_t outputLength,
                 std::size_t &encodedSize );

/**
 * RLE Decodes an array of data.
 * @param[in] input input data
 * @param[in] inputLength input data length in bytes
 * @param[out] output output data, pre-allocated by caller, allocated using GetMaximumRleCompressedSize()
 * @param[in] outputLength size of output data in bytes
 * @param[out] decodedSize the number of bytes decoded into outputBuffer
 */
bool DecodeRle( const unsigned char* input,
                const std::size_t inputLength,
                unsigned char* output,
                const std::size_t outputLength,
               std::size_t& decodedSize);


} // namespace DataCompression

} // namespace TizenPlatform

} // namespace Dali

#endif // __DALI_TIZEN_PLATFORM_DATA_COMPRESSION_H__
