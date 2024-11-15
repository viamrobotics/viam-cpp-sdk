#include <viam/sdk/spatialmath/orientation.hpp>

#include <vector>

#include <boost/variant/get.hpp>
#include <boost/variant/variant.hpp>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/spatialmath/orientation_types.hpp>

namespace viam {
namespace sdk {

OrientationConfig::OrientationConfig() {
    type_ = Quaternion;
    quaternion quat;
    quat.w = 1;
    quat.x = 0;
    quat.y = 0;
    quat.z = 0;
    orientation_ = quat;
}

OrientationType OrientationConfig::get_type() const {
    return type_;
};
const std::vector<std::uint8_t>& OrientationConfig::get_value() const {
    return value_;
};
const orientation& OrientationConfig::get_orientation() const {
    return orientation_;
}

}  // namespace sdk
}  // namespace viam
