#include <viam/sdk/components/servo/servo.hpp>

#include <viam/api/component/servo/v1/servo.grpc.pb.h>
#include <viam/api/component/servo/v1/servo.pb.h>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

API Servo::api() const {
    return API::get<Servo>();
}

API API::traits<Servo>::api() {
    return {kRDK, kComponent, "servo"};
}

Servo::position Servo::from_proto(const viam::component::servo::v1::GetPositionResponse& proto) {
    return proto.position_deg();
}

}  // namespace sdk
}  // namespace viam
