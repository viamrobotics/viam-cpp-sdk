#pragma once

#include <app/v1/robot.pb.h>

#include <boost/variant/variant.hpp>
#include <spatialmath/orientation_types.hpp>
#include <string>
#include <vector>

typedef boost::variant<Viam::SDK::axis_angles,
                       Viam::SDK::euler_angles,
                       Viam::SDK::orientation_vector,
                       Viam::SDK::orientation_vector_degrees,
                       Viam::SDK::quaternion>
    orientation;

class OrientationConfig {
   public:
    Viam::SDK::OrientationType type;
    std::vector<std::uint8_t> value;
    orientation orientation_;
    viam::app::v1::Orientation to_proto();

    static OrientationConfig from_proto(viam::app::v1::Orientation proto);
};
