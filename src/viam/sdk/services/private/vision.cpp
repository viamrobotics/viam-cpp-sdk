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

#include <viam/sdk/services/private/vision.hpp>

#include <viam/sdk/common/private/repeated_ptr_convert.hpp>
#include <viam/sdk/common/proto_convert.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>

namespace viam {
namespace sdk {
namespace impl {
namespace vision {

namespace vpb = ::viam::service::vision::v1;

vpb::Detection to_proto(const Vision::detection& d) {
    vpb::Detection out;
    if (d.x_min) {
        out.set_x_min(*d.x_min);
    }
    if (d.y_min) {
        out.set_y_min(*d.y_min);
    }
    if (d.x_max) {
        out.set_x_max(*d.x_max);
    }
    if (d.y_max) {
        out.set_y_max(*d.y_max);
    }
    if (d.x_min_normalized) {
        out.set_x_min_normalized(*d.x_min_normalized);
    }
    if (d.y_min_normalized) {
        out.set_y_min_normalized(*d.y_min_normalized);
    }
    if (d.x_max_normalized) {
        out.set_x_max_normalized(*d.x_max_normalized);
    }
    if (d.y_max_normalized) {
        out.set_y_max_normalized(*d.y_max_normalized);
    }
    out.set_class_name(d.class_name);
    out.set_confidence(d.confidence);
    return out;
}

Vision::detection from_proto(const vpb::Detection& p) {
    Vision::detection out;
    if (p.has_x_min()) {
        out.x_min = p.x_min();
    }
    if (p.has_y_min()) {
        out.y_min = p.y_min();
    }
    if (p.has_x_max()) {
        out.x_max = p.x_max();
    }
    if (p.has_y_max()) {
        out.y_max = p.y_max();
    }
    if (p.has_x_min_normalized()) {
        out.x_min_normalized = p.x_min_normalized();
    }
    if (p.has_y_min_normalized()) {
        out.y_min_normalized = p.y_min_normalized();
    }
    if (p.has_x_max_normalized()) {
        out.x_max_normalized = p.x_max_normalized();
    }
    if (p.has_y_max_normalized()) {
        out.y_max_normalized = p.y_max_normalized();
    }
    out.class_name = p.class_name();
    out.confidence = p.confidence();
    return out;
}

vpb::Classification to_proto(const Vision::classification& c) {
    vpb::Classification out;
    out.set_class_name(c.class_name);
    out.set_confidence(c.confidence);
    return out;
}

Vision::classification from_proto(const vpb::Classification& p) {
    return Vision::classification{p.class_name(), p.confidence()};
}

void to_proto(const Vision::point_cloud_object& o, ::viam::common::v1::PointCloudObject* out) {
    out->set_point_cloud(
        std::string(reinterpret_cast<const char*>(o.cloud.pc.data()), o.cloud.pc.size()));
    *(out->mutable_geometries()->mutable_geometries()) = impl::to_repeated_field(o.geometries);
}

Vision::point_cloud_object from_proto(const ::viam::common::v1::PointCloudObject& p) {
    Vision::point_cloud_object out;
    const auto& bytes = p.point_cloud();
    out.cloud.pc.assign(reinterpret_cast<const unsigned char*>(bytes.data()),
                        reinterpret_cast<const unsigned char*>(bytes.data()) + bytes.size());
    out.geometries = impl::from_repeated_field(p.geometries().geometries());
    return out;
}

void to_proto(const Vision::properties& props, vpb::GetPropertiesResponse* out) {
    out->set_classifications_supported(props.classifications_supported);
    out->set_detections_supported(props.detections_supported);
    out->set_object_point_clouds_supported(props.object_point_clouds_supported);
}

Vision::properties from_proto(const vpb::GetPropertiesResponse& p) {
    return Vision::properties{
        p.classifications_supported(), p.detections_supported(), p.object_point_clouds_supported()};
}

}  // namespace vision
}  // namespace impl
}  // namespace sdk
}  // namespace viam
