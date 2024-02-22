#pragma once

#include <string>

#include <viam/api/app/v1/robot.pb.h>

#include <viam/sdk/spatialmath/geometry.hpp>
#include <viam/sdk/spatialmath/orientation.hpp>

namespace viam {
namespace sdk {

class LinkConfig {
   public:
    viam::app::v1::Frame to_proto() const;
    static LinkConfig from_proto(const viam::app::v1::Frame& proto);
    translation get_translation() const;
    OrientationConfig get_orientation_config() const;
    GeometryConfig get_geometry_config() const;
    std::string get_parent() const;

   private:
    std::string id_;
    translation translation_;
    OrientationConfig orientation_;
    GeometryConfig geometry_;
    std::string parent_;
};

}  // namespace sdk
}  // namespace viam
