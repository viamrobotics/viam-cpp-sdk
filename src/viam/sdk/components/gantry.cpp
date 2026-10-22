#include <viam/sdk/components/gantry.hpp>

#include <viam/sdk/common/exception.hpp>

namespace viam {
namespace sdk {

API Gantry::api() const {
    return API::get<Gantry>();
}

API API::traits<Gantry>::api() {
    return {kRDK, kComponent, "gantry"};
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

KinematicsData Gantry::get_kinematics(const ProtoStruct&) {
    throw Exception(ErrorCondition::k_not_supported,
                    "gantry '" + name() + "' does not provide raw kinematics data");
}

KinematicsData Gantry::get_kinematics() {
    return get_kinematics({});
}

#pragma GCC diagnostic pop

}  // namespace sdk
}  // namespace viam
