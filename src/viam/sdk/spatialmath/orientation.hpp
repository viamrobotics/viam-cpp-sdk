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
    viam::app::v1::Orientation to_proto() const;

    static OrientationConfig from_proto(viam::app::v1::Orientation proto);

   private:
    OrientationType type_;
    std::vector<std::uint8_t> value_;
    orientation orientation_;
};

}  // namespace sdk
}  // namespace viam
