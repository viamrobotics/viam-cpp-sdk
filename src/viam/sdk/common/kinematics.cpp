#include <viam/sdk/common/kinematics.hpp>

#include <boost/variant/apply_visitor.hpp>
#include <viam/api/common/v1/common.pb.h>

namespace viam {
namespace sdk {

KinematicsData get_kinematics(
    std::function<KinematicsData(const ProtoStruct&)> get_kinematics_func) {
    return get_kinematics_func({});
}

KinematicsData kinematics_from_proto(const ::viam::common::v1::GetKinematicsResponse& response) {
    std::vector<unsigned char> bytes(response.kinematics_data().begin(),
                                     response.kinematics_data().end());
    switch (response.format()) {
        case ::viam::common::v1::KinematicsFileFormat::KINEMATICS_FILE_FORMAT_SVA:
            return KinematicsDataSVA(std::move(bytes));
        case ::viam::common::v1::KinematicsFileFormat::KINEMATICS_FILE_FORMAT_URDF:
            return KinematicsDataURDF(std::move(bytes));
        case ::viam::common::v1::KinematicsFileFormat::KINEMATICS_FILE_FORMAT_UNSPECIFIED:  // fallthrough
        default:
            return KinematicsDataUnspecified{};
    }
}

void kinematics_to_proto(const KinematicsData& kinematics,
                         ::viam::common::v1::GetKinematicsResponse* response) {
    struct Visitor {
        using FileFormat = ::viam::common::v1::KinematicsFileFormat;
        auto operator()(const KinematicsDataUnspecified&) const noexcept {
            return FileFormat::KINEMATICS_FILE_FORMAT_UNSPECIFIED;
        }

        auto operator()(const KinematicsDataSVA&) const noexcept {
            return FileFormat::KINEMATICS_FILE_FORMAT_SVA;
        }

        auto operator()(const KinematicsDataURDF&) const noexcept {
            return FileFormat::KINEMATICS_FILE_FORMAT_URDF;
        }
    } visitor;

    boost::apply_visitor(
        [&](const auto& v) {
            response->set_format(visitor(v));
            response->mutable_kinematics_data()->assign(v.bytes.begin(), v.bytes.end());
        },
        kinematics);
}

}  // namespace sdk
}  // namespace viam

