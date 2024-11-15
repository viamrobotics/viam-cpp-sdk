#include <viam/sdk/common/linear_algebra.hpp>

#include <array>

namespace viam {
namespace sdk {

Vector3::Vector3(scalar_type x, scalar_type y, scalar_type z) : data_{x, y, z} {};
Vector3::Vector3() : Vector3(0.0, 0.0, 0.0) {};

double Vector3::x() const {
    return this->data_[0];
}

double Vector3::y() const {
    return this->data_[1];
}

double Vector3::z() const {
    return this->data_[2];
}

Vector3& Vector3::set_x(double x) {
    this->data_[0] = x;
    return *this;
}

Vector3& Vector3::set_y(double y) {
    this->data_[1] = y;
    return *this;
}

Vector3& Vector3::set_z(double z) {
    this->data_[2] = z;
    return *this;
}

const std::array<double, 3>& Vector3::data() const {
    return this->data_;
}

std::array<double, 3>& Vector3::data() {
    return this->data_;
}

}  // namespace sdk
}  // namespace viam
