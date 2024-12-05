#pragma once

#include <string>

#include <viam/sdk/common/proto_convert.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>
#include <viam/sdk/spatialmath/orientation.hpp>

namespace viam {
namespace app {
namespace v1 {

class Frame;
}
}  // namespace app
}  // namespace viam

namespace viam {
namespace sdk {

class LinkConfig {
   public:
    LinkConfig() = default;
    LinkConfig(translation t, Orientation o, GeometryConfig gcfg, std::string parent);

    const translation& get_translation() const;
    const Orientation& get_orientation() const;
    const GeometryConfig& get_geometry_config() const;
    const std::string& get_parent() const;

   private:
    translation translation_;
    Orientation orientation_;
    GeometryConfig geometry_;
    std::string parent_;
};

namespace proto_convert_details {

template <>
struct to_proto<LinkConfig> {
    void operator()(const LinkConfig&, app::v1::Frame*) const;
};

template <>
struct from_proto<app::v1::Frame> {
    LinkConfig operator()(const app::v1::Frame*) const;
};

}  // namespace proto_convert_details
}  // namespace sdk
}  // namespace viam
