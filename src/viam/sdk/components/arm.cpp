#include <viam/sdk/components/arm.hpp>

namespace viam {
namespace sdk {

API Arm::api() const {
    return API::get<Arm>();
}

API API::traits<Arm>::api() {
    return {kRDK, kComponent, "arm"};
}

Arm::KinematicsData Arm::from_proto(const viam::common::v1::GetKinematicsResponse& proto) {
    std::vector<unsigned char> bytes(proto.kinematics_data().begin(),
                                     proto.kinematics_data().end());
    switch (proto.format()) {
        case common::v1::KinematicsFileFormat::KINEMATICS_FILE_FORMAT_SVA:
            return Arm::KinematicsDataSVA(std::move(bytes));
        case common::v1::KinematicsFileFormat::KINEMATICS_FILE_FORMAT_URDF:
            return Arm::KinematicsDataURDF(std::move(bytes));
        case common::v1::KinematicsFileFormat::KINEMATICS_FILE_FORMAT_UNSPECIFIED:  // fallthrough
        default:
            return Arm::KinematicsDataUnspecified{};
    }
}

Arm::Arm(std::string name) : Component(std::move(name)) {}


}  // namespace sdk
}  // namespace viam
