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

bool operator==(const Vision::detection& a, const Vision::detection& b) {
    return a.x_min == b.x_min && a.y_min == b.y_min && a.x_max == b.x_max && a.y_max == b.y_max &&
           a.class_name == b.class_name && a.confidence == b.confidence &&
           a.x_min_normalized == b.x_min_normalized && a.y_min_normalized == b.y_min_normalized &&
           a.x_max_normalized == b.x_max_normalized && a.y_max_normalized == b.y_max_normalized;
}

bool operator==(const Vision::classification& a, const Vision::classification& b) {
    return a.class_name == b.class_name && a.confidence == b.confidence;
}

bool operator==(const Vision::point_cloud_object& a, const Vision::point_cloud_object& b) {
    return a.point_cloud == b.point_cloud && a.geometries == b.geometries;
}

bool operator==(const Vision::properties& a, const Vision::properties& b) {
    return a.classifications_supported == b.classifications_supported &&
           a.detections_supported == b.detections_supported &&
           a.object_point_clouds_supported == b.object_point_clouds_supported;
}

}  // namespace sdk
}  // namespace viam
