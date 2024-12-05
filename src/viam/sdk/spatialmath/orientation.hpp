#pragma once

#include <string>
#include <vector>

#include <boost/variant/variant.hpp>

#include <viam/sdk/common/proto_convert.hpp>
#include <viam/sdk/spatialmath/orientation_types.hpp>

namespace viam {
namespace app {
namespace v1 {

class Orientation;
}
}  // namespace app
}  // namespace viam

namespace viam {
namespace sdk {

// Note that quaternion must be the first type because this is what is default constructed.
using Orientation = boost::
    variant<quaternion, axis_angles, orientation_vector, orientation_vector_degrees, euler_angles>;

OrientationType get_type(const Orientation&);

namespace proto_convert_details {

template <>
struct to_proto<Orientation> {
    void operator()(const Orientation&, app::v1::Orientation*) const;
};
template <>
struct from_proto<app::v1::Orientation> {
    Orientation operator()(const app::v1::Orientation*) const;
};

}  // namespace proto_convert_details
}  // namespace sdk
}  // namespace viam
