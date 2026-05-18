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

#include <viam/sdk/services/vision.hpp>

#include <viam/sdk/common/utils.hpp>

namespace viam {
namespace sdk {

API Vision::api() const {
    return API::get<Vision>();
}

API API::traits<Vision>::api() {
    return API(kRDK, kService, "vision");
}

Vision::Vision(std::string name) : Service(std::move(name)) {}

bool operator==(const Vision::detection& lhs, const Vision::detection& rhs) {
    return lhs.x_min == rhs.x_min && lhs.y_min == rhs.y_min && lhs.x_max == rhs.x_max &&
           lhs.y_max == rhs.y_max && lhs.class_name == rhs.class_name &&
           lhs.confidence == rhs.confidence && lhs.x_min_normalized == rhs.x_min_normalized &&
           lhs.y_min_normalized == rhs.y_min_normalized &&
           lhs.x_max_normalized == rhs.x_max_normalized &&
           lhs.y_max_normalized == rhs.y_max_normalized;
}

bool operator==(const Vision::classification& lhs, const Vision::classification& rhs) {
    return lhs.class_name == rhs.class_name && lhs.confidence == rhs.confidence;
}

bool operator==(const Vision::point_cloud_object& lhs, const Vision::point_cloud_object& rhs) {
    return lhs.cloud == rhs.cloud && lhs.geometries == rhs.geometries;
}

bool operator==(const Vision::properties& lhs, const Vision::properties& rhs) {
    return lhs.classifications_supported == rhs.classifications_supported &&
           lhs.detections_supported == rhs.detections_supported &&
           lhs.object_point_clouds_supported == rhs.object_point_clouds_supported;
}

}  // namespace sdk
}  // namespace viam
