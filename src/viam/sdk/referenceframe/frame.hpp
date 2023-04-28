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
    static LinkConfig from_proto(viam::app::v1::Frame proto);

   private:
    std::string id_;
    translation translation_;
    OrientationConfig orientation_;
    GeometryConfig geometry_;
    std::string parent_;
};

}  // namespace sdk
}  // namespace viam
