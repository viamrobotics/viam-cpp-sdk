#include <viam/sdk/spatialmath/orientation_types.hpp>

#include <viam/api/app/v1/robot.pb.h>

namespace viam {
namespace sdk {

void proto_convert_details::to_proto<translation>::operator()(const translation& self,
                                                              app::v1::Translation* t) const {
    t->set_x(self.x);
    t->set_y(self.y);
    t->set_z(self.z);
}

}  // namespace sdk
}  // namespace viam
