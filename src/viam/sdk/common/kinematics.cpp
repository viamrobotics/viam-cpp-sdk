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

void to_proto_impl<KinematicsData>::operator()(const KinematicsData& self,
                                               common::v1::GetKinematicsResponse* proto) const {
    struct Visitor {
        using FileFormat = common::v1::KinematicsFileFormat;
        common::v1::GetKinematicsResponse* proto;

        void set_common(FileFormat format, const std::vector<unsigned char>& bytes) const {
            proto->set_format(format);
            proto->mutable_kinematics_data()->assign(bytes.begin(), bytes.end());
        }

        void operator()(const KinematicsDataUnspecified& v) const {
            set_common(FileFormat::KINEMATICS_FILE_FORMAT_UNSPECIFIED, v.bytes);
        }

        void operator()(const KinematicsDataSVA& v) const {
            set_common(FileFormat::KINEMATICS_FILE_FORMAT_SVA, v.bytes);
        }

        void operator()(const KinematicsDataURDF& v) const {
            set_common(FileFormat::KINEMATICS_FILE_FORMAT_URDF, v.bytes);
            for (const auto& entry : v.meshes_by_urdf_filepath) {
                proto->mutable_meshes_by_urdf_filepath()->insert(
                    {entry.first, to_proto(entry.second)});
            }
        }
    } visitor{proto};

    boost::apply_visitor(visitor, self);
}

void to_proto_impl<KinematicsResponse>::operator()(const KinematicsResponse& self,
                                                   common::v1::GetKinematicsResponse* proto) const {
    to_proto_impl<KinematicsData>{}(self.kinematics_data, proto);
    for (const auto& entry : self.meshes_by_urdf_filepath) {
        (*proto->mutable_meshes_by_urdf_filepath())[entry.first] = to_proto(entry.second);
    }
}

KinematicsData from_proto_impl<common::v1::GetKinematicsResponse>::operator()(
    const common::v1::GetKinematicsResponse* proto) const {
    std::vector<unsigned char> bytes(proto->kinematics_data().begin(),
                                     proto->kinematics_data().end());
    switch (proto->format()) {
        case common::v1::KinematicsFileFormat::KINEMATICS_FILE_FORMAT_SVA:
            return KinematicsDataSVA(std::move(bytes));
        case common::v1::KinematicsFileFormat::KINEMATICS_FILE_FORMAT_URDF: {
            KinematicsDataURDF data(std::move(bytes));
            for (const auto& entry : proto->meshes_by_urdf_filepath()) {
                data.meshes_by_urdf_filepath[entry.first] = from_proto(entry.second);
            }
            return data;
        }
        case common::v1::KinematicsFileFormat::KINEMATICS_FILE_FORMAT_UNSPECIFIED:  // fallthrough
        default:
            return KinematicsDataUnspecified{};
    }
}

}  // namespace proto_convert_details
}  // namespace sdk
}  // namespace viam
