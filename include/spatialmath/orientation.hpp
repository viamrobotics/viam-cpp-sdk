#ifndef ORIENTATION_H
#define ORIENTATION_H

#include <app/v1/robot.pb.h>

#include <boost/variant/variant.hpp>
#include <spatialmath/orientation_types.hpp>
#include <string>
#include <vector>

namespace SDK = Viam::SDK;

// CR erodkin: this should be in orientation_types, not in orientation
enum OrientationType {
    AxisAngles,
    OrientationVector,
    OrientationVectorDegrees,
    EulerAngles,
    Quaternion,
};

class orientation {
   public:
    boost::variant<SDK::axis_angles,
                   SDK::euler_angles,
                   SDK::orientation_vector,
                   SDK::orientation_vector_degrees,
                   SDK::quaternion>
        orientation_;
};

class OrientationConfig {
   public:
    OrientationType type;
    std::vector<std::uint8_t> value;
    orientation orientation_;
    viam::app::v1::Orientation to_proto();

    static OrientationConfig from_proto(viam::app::v1::Orientation proto);
};

#endif
