#include <viam/sdk/tests/mocks/mock_base.hpp>

#include <viam/sdk/common/linear_algebra.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/components/base.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace base {

using namespace viam::sdk;

Base::properties fake_properties() {
    return {2, 4, 8};
}

void MockBase::move_straight(int64_t distance_mm, double mm_per_sec, const ProtoStruct&) {
    this->peek_move_straight_distance_mm = distance_mm;
    this->peek_move_straight_mm_per_sec = mm_per_sec;
}
void MockBase::spin(double angle_deg, double degs_per_sec, const ProtoStruct&) {
    this->peek_spin_angle_deg = angle_deg;
    this->peek_spin_degs_per_sec = degs_per_sec;
}
void MockBase::set_power(const Vector3& linear, const Vector3& angular, const ProtoStruct&) {
    this->peek_set_power_linear = linear;
    this->peek_set_power_angular = angular;
}
void MockBase::set_velocity(const Vector3& linear, const Vector3& angular, const ProtoStruct&) {
    this->peek_set_velocity_linear = linear;
    this->peek_set_velocity_angular = angular;
}
void MockBase::stop(const ProtoStruct&) {
    this->peek_stop_called = true;
}
bool MockBase::is_moving() {
    return false;
}
Base::properties MockBase::get_properties(const sdk::ProtoStruct&) {
    return fake_properties();
}
std::vector<GeometryConfig> MockBase::get_geometries(const sdk::ProtoStruct&) {
    return fake_geometries();
}

std::shared_ptr<MockBase> MockBase::get_mock_base() {
    return std::make_shared<MockBase>("mock_base");
}
ProtoStruct MockBase::do_command(const ProtoStruct& command) {
    this->peek_do_command_command = command;
    return command;
}

}  // namespace base
}  // namespace sdktests
}  // namespace viam
