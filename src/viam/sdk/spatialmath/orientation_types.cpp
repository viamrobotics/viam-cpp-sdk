#include <viam/sdk/spatialmath/orientation_types.hpp>

#include <viam/api/app/v1/robot.pb.h>

namespace viam {
namespace sdk {

viam::app::v1::Translation translation::to_proto() const {
    viam::app::v1::Translation t;
    t.set_x(x);
    t.set_y(y);
    t.set_z(z);
    return t;
}

}  // namespace sdk
}  // namespace viam
