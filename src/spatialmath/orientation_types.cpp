#include <spatialmath/orientation_types.hpp>

#include <app/v1/robot.pb.h>

namespace viam {
namespace cppsdk {

viam::app::v1::Translation translation::to_proto() {
    viam::app::v1::Translation t;
    t.set_x(x);
    t.set_y(y);
    t.set_z(z);
    return t;
}

}  // namespace cppsdk
}  // namespace viam
