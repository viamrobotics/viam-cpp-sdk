#include <viam/sdk/common/linear_algebra.hpp>

#include <array>

#include <viam/api/common/v1/common.pb.h>

namespace viam {
namespace sdk {

double Vector3::x() const {
    return this->data[0];
}

double Vector3::y() const {
    return this->data[1];
}

double Vector3::z() const {
    return this->data[2];
}

Vector3& Vector3::set_x(double x) {
    this->data[0] = x;
    return *this;
}

Vector3& Vector3::set_y(double y) {
    this->data[1] = y;
    return *this;
}

Vector3& Vector3::set_z(double z) {
    this->data[2] = z;
    return *this;
}

namespace proto_convert_details {

void to_proto<Vector3>::operator()(const Vector3& self, common::v1::Vector3* proto) const {
    proto->set_x(self.x());
    proto->set_y(self.y());
    proto->set_z(self.z());
}

Vector3 from_proto<common::v1::Vector3>::operator()(const common::v1::Vector3* proto) const {
    return {proto->x(), proto->y(), proto->z()};
}

}  // namespace proto_convert_details
}  // namespace sdk
}  // namespace viam
