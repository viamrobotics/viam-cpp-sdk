#include <viam/sdk/referenceframe/frame.hpp>

#include <string>

#include <viam/api/app/v1/robot.pb.h>
#include <viam/api/common/v1/common.pb.h>

#include <viam/sdk/spatialmath/geometry.hpp>
#include <viam/sdk/spatialmath/orientation.hpp>
#include <viam/sdk/spatialmath/orientation_types.hpp>

namespace viam {
namespace sdk {

LinkConfig::LinkConfig(translation t, Orientation o, GeometryConfig gcfg, std::string parent)
    : translation_(std::move(t)),
      orientation_(std::move(o)),
      geometry_(std::move(gcfg)),
      parent_(std::move(parent)) {}

const translation& LinkConfig::get_translation() const {
    return translation_;
}

const Orientation& LinkConfig::get_orientation() const {
    return orientation_;
}

const GeometryConfig& LinkConfig::get_geometry_config() const {
    return geometry_;
}

const std::string& LinkConfig::get_parent() const {
    return parent_;
}

namespace proto_convert_details {

void to_proto_impl<LinkConfig>::operator()(const LinkConfig& self, app::v1::Frame* proto) const {
    *(proto->mutable_parent()) = self.get_parent();
    *(proto->mutable_geometry()) = v2::to_proto(self.get_geometry_config());
    *(proto->mutable_orientation()) = v2::to_proto(self.get_orientation());
    *(proto->mutable_translation()) = v2::to_proto(self.get_translation());
}

LinkConfig from_proto_impl<app::v1::Frame>::operator()(const app::v1::Frame* proto) const {
    return LinkConfig(
        v2::from_proto(proto->translation()),
        proto->has_orientation() ? v2::from_proto(proto->orientation()) : Orientation{},
        proto->has_geometry() ? v2::from_proto(proto->geometry()) : GeometryConfig{},
        proto->parent());
}

}  // namespace proto_convert_details
}  // namespace sdk
}  // namespace viam
