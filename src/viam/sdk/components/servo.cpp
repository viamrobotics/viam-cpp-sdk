#include <viam/sdk/components/servo.hpp>

namespace viam {
namespace sdk {

API Servo::api() const {
    return API::get<Servo>();
}

API API::traits<Servo>::api() {
    return {kRDK, kComponent, "servo"};
}

}  // namespace sdk
}  // namespace viam
