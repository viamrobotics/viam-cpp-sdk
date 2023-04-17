#pragma once

#include <string>

#include <app/v1/robot.pb.h>

#include <spatialmath/geometry.hpp>
#include <spatialmath/orientation.hpp>

namespace viam {
namespace cppsdk {

class LinkConfig {
   public:
    viam::app::v1::Frame to_proto();
    static LinkConfig from_proto(viam::app::v1::Frame proto);

   private:
    std::string id;
    class translation translation;
    OrientationConfig orientation;
    GeometryConfig geometry;
    std::string parent;
};

}  // namespace cppsdk
}  // namespace viam
