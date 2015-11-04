#ifndef _DALI_PLATFORM_RESOURCE_LOADING_CLIENT_H_
#define _DALI_PLATFORM_RESOURCE_LOADING_CLIENT_H_
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

// INTERNAL INCLUDES

// EXTERNAL INCLUDES

namespace Dali
{
namespace TizenPlatform
{

/**
 * @brief Abstract interface to the caller of a low-level resource loading
 * function such as a loader for an image format.
 */
class ResourceLoadingClient
{
public:
  /**
   * @brief Check whether the current request has been cancelled.
   *
   * This will throw an exception to unwind the stack if the current request
   * has been cancelled.
   *
   * @note Only place calls to this function at exception-safe locations in loader code.
   **/
  virtual void InterruptionPoint() const = 0;

protected:
  /** Construction is restricted to derived / implementing classes. */
  ResourceLoadingClient() {}
  /** Destruction of an object through this interface is not allowed. */
  ~ResourceLoadingClient() {}

private:
  ResourceLoadingClient( const ResourceLoadingClient& rhs );
  ResourceLoadingClient& operator =( ResourceLoadingClient& rhs );
};

/**
 * @brief Default implementation of a caller of a low-level resource loading
 * function which does nothing.
 */
class StubbedResourceLoadingClient : public ResourceLoadingClient
{
public:
  /**
   * @brief Check whether the current request has been cancelled.
   *
   * This does nothing and so can never throw an exception.
   **/
  virtual void InterruptionPoint() const {}

  /** Construction is a NOP. */
  StubbedResourceLoadingClient() {}
  /** Destruction has no work to do. */
  ~StubbedResourceLoadingClient() {}

private:
  StubbedResourceLoadingClient( const StubbedResourceLoadingClient& rhs );
  StubbedResourceLoadingClient& operator =( StubbedResourceLoadingClient& rhs );
};

} /* namespace TizenPlatform */
} /* namespace Dali */

#endif /* _DALI_PLATFORM_RESOURCE_LOADING_CLIENT_H_ */
