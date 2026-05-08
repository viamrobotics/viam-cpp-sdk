// Copyright 2024 Viam Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/service/vision/v1/vision.pb.h>
#include <viam/sdk/services/vision.hpp>

namespace viam {
namespace sdk {
namespace impl {
namespace vision {

::viam::service::vision::v1::Detection to_proto(const Vision::detection&);
Vision::detection from_proto(const ::viam::service::vision::v1::Detection&);

::viam::service::vision::v1::Classification to_proto(const Vision::classification&);
Vision::classification from_proto(const ::viam::service::vision::v1::Classification&);

void to_proto(const Vision::point_cloud_object&, ::viam::common::v1::PointCloudObject* out);
Vision::point_cloud_object from_proto(const ::viam::common::v1::PointCloudObject&);

void to_proto(const Vision::properties&, ::viam::service::vision::v1::GetPropertiesResponse* out);
Vision::properties from_proto(const ::viam::service::vision::v1::GetPropertiesResponse&);

}  // namespace vision
}  // namespace impl
}  // namespace sdk
}  // namespace viam
