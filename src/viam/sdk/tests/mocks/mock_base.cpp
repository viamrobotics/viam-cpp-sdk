#include <viam/sdk/tests/mocks/mock_base.hpp>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/base/v1/base.grpc.pb.h>
#include <viam/api/component/base/v1/base.pb.h>

#include <viam/sdk/common/linear_algebra.hpp>
#include <viam/sdk/components/base/base.hpp>
#include <viam/sdk/components/base/server.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace base {

using namespace viam::sdk;

void MockBase::move_straight(int64_t distance_mm, double mm_per_sec) {
    this->peek_move_straight_distance_mm = distance_mm;
    this->peek_move_straight_mm_per_sec = mm_per_sec;
};
void MockBase::spin(double angle_deg, double degs_per_sec) {
    this->peek_spin_angle_deg = angle_deg;
    this->peek_spin_degs_per_sec = degs_per_sec;
};
void MockBase::set_power(Vector3 linear, Vector3 angular) {
    this->peek_set_power_linear = linear;
    this->peek_set_power_angular = angular;
};
void MockBase::set_velocity(Vector3 linear, Vector3 angular) {
    this->peek_set_velocity_linear = linear;
    this->peek_set_velocity_angular = angular;
};
grpc::StatusCode MockBase::stop() {
    this->peek_stop_called = true;
    return grpc::StatusCode::OK;
};
bool MockBase::is_moving() {
    return false;
};

std::shared_ptr<MockBase> MockBase::get_mock_base() {
    return std::make_shared<MockBase>("mock_base");
}
AttributeMap MockBase::do_command(AttributeMap command) {
    this->peek_do_command_command = command;
    return command;
};

}  // namespace base
}  // namespace sdktests
}  // namespace viam
