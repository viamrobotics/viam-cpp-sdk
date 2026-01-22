#include <viam/sdk/common/kinematics.hpp>

#include <tuple>

#include <boost/variant/apply_visitor.hpp>
#include <viam/api/common/v1/common.pb.h>

namespace viam {
namespace sdk {

bool operator==(const KinematicsResponse& lhs, const KinematicsResponse& rhs) {
    return std::tie(lhs.kinematics_data, lhs.meshes_by_urdf_filepath) ==
           std::tie(rhs.kinematics_data, rhs.meshes_by_urdf_filepath);
}

namespace proto_convert_details {

void to_proto_impl<KinematicsResponse>::operator()(const KinematicsResponse& self,
                                                   common::v1::GetKinematicsResponse* proto) const {
    struct Visitor {
        using FileFormat = common::v1::KinematicsFileFormat;
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
            proto->set_format(visitor(v));
            proto->mutable_kinematics_data()->assign(v.bytes.begin(), v.bytes.end());
        },
        self.kinematics_data);

    for (const auto& entry : self.meshes_by_urdf_filepath) {
        proto->mutable_meshes_by_urdf_filepath()->insert({entry.first, to_proto(entry.second)});
    }
}

KinematicsData from_proto_impl<common::v1::GetKinematicsResponse>::operator()(
    const common::v1::GetKinematicsResponse* proto) const {
    std::vector<unsigned char> bytes(proto->kinematics_data().begin(),
                                     proto->kinematics_data().end());
    switch (proto->format()) {
        case common::v1::KinematicsFileFormat::KINEMATICS_FILE_FORMAT_SVA:
            return KinematicsDataSVA(std::move(bytes));
        case common::v1::KinematicsFileFormat::KINEMATICS_FILE_FORMAT_URDF:
            return KinematicsDataURDF(std::move(bytes));
        case common::v1::KinematicsFileFormat::KINEMATICS_FILE_FORMAT_UNSPECIFIED:  // fallthrough
        default:
            return KinematicsDataUnspecified{};
    }
}

}  // namespace proto_convert_details
}  // namespace sdk
}  // namespace viam
