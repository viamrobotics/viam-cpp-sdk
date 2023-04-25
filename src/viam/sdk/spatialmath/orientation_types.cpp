#include <viam/sdk/spatialmath/orientation_types.hpp>

#include <viam/api/app/v1/robot.pb.h>

namespace viam {
namespace sdk {

viam::app::v1::Translation translation::to_proto() {
    viam::app::v1::Translation t;
    t.set_x(x_);
    t.set_y(y_);
    t.set_z(z_);
    return t;
}

}  // namespace sdk
}  // namespace viam
