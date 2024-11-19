#pragma once

#include <vector>

#include <boost/variant/variant.hpp>

#include <viam/sdk/spatialmath/orientation_types.hpp>

namespace viam {
namespace sdk {

typedef boost::
    variant<axis_angles, orientation_vector, orientation_vector_degrees, euler_angles, quaternion>
        orientation;

class OrientationConfig {
   public:
    OrientationConfig(OrientationType type_,
                      std::vector<std::uint8_t> value,
                      orientation orientation)
        : type_(std::move(type_)), value_(std::move(value)), orientation_(std::move(orientation)) {}
    // Defaults to sentinel no rotation value
    OrientationConfig();

    OrientationType get_type() const;
    const std::vector<std::uint8_t>& get_value() const;
    const orientation& get_orientation() const;

   private:
    OrientationType type_;
    std::vector<std::uint8_t> value_;
    orientation orientation_;
};

}  // namespace sdk
}  // namespace viam
