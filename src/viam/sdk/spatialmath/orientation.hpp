#pragma once

#include <string>
#include <vector>

#include <boost/variant/variant.hpp>

#include <viam/api/app/v1/robot.pb.h>

#include <viam/sdk/spatialmath/orientation_types.hpp>

namespace viam {
namespace sdk {

typedef boost::
    variant<axis_angles, euler_angles, orientation_vector, orientation_vector_degrees, quaternion>
        orientation;

class OrientationConfig {
   public:
    OrientationType type;
    std::vector<std::uint8_t> value;
    orientation orientation_;
    viam::app::v1::Orientation to_proto();

    static OrientationConfig from_proto(viam::app::v1::Orientation proto);
};

}  // namespace sdk
}  // namespace viam
