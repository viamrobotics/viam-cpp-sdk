#include <viam/sdk/spatialmath/orientation_types.hpp>

#include <viam/api/app/v1/robot.pb.h>

namespace viam {
namespace sdk {

namespace proto_convert_details {

void to_proto<translation>::operator()(const translation& self, app::v1::Translation* t) const {
    t->set_x(self.x);
    t->set_y(self.y);
    t->set_z(self.z);
}

translation from_proto<app::v1::Translation>::operator()(const app::v1::Translation* proto) const {
    return {proto->x(), proto->y(), proto->z()};
}

}  // namespace proto_convert_details
}  // namespace sdk
}  // namespace viam
