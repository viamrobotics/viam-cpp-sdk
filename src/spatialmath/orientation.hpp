#pragma once

#include <string>
#include <vector>

#include <boost/variant/variant.hpp>

#include <app/v1/robot.pb.h>

#include <spatialmath/orientation_types.hpp>

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
