#pragma once

#include <boost/variant/variant.hpp>
#include <map>
#include <string>
#include <vector>

#include <viam/sdk/common/mesh.hpp>
#include <viam/sdk/common/proto_convert.hpp>
#include <viam/sdk/common/proto_value.hpp>

namespace viam {
namespace common {
namespace v1 {
class GetKinematicsResponse;
}
}  // namespace common
}  // namespace viam

namespace viam {
namespace sdk {

// Base class for use below in defining kinematics data strong typedefs
template <class Tag>
struct raw_bytes {
    // Pre c++17 our derived classes aren't aggregate initializable so we need to define
    // and using declare some ctors

    raw_bytes() = default;
    raw_bytes(std::vector<unsigned char> b) : bytes(std::move(b)) {}

    std::vector<unsigned char> bytes{};
};

// Comparison operator helper for the data types below
template <class DataType>
struct EqCompare {
    inline friend bool operator==(const DataType& lhs, const DataType& rhs) {
        return lhs.bytes == rhs.bytes;
    }
};

/// @brief Kinematics data with unspecified format
struct KinematicsDataUnspecified : raw_bytes<KinematicsDataUnspecified>,
                                   EqCompare<KinematicsDataUnspecified> {};

/// @brief Kinematics data in Viam's Spatial Vector Algebra (SVA) format
struct KinematicsDataSVA : raw_bytes<KinematicsDataSVA>, EqCompare<KinematicsDataSVA> {
    using raw_bytes<KinematicsDataSVA>::raw_bytes;
};

/// @brief Kinematics data in URDF format with optional meshes by URDF filepath
struct KinematicsDataURDF : raw_bytes<KinematicsDataURDF> {
    using raw_bytes<KinematicsDataURDF>::raw_bytes;
    std::map<std::string, mesh> meshes_by_urdf_filepath;

    inline friend bool operator==(const KinematicsDataURDF& lhs, const KinematicsDataURDF& rhs) {
        return lhs.bytes == rhs.bytes && lhs.meshes_by_urdf_filepath == rhs.meshes_by_urdf_filepath;
    }
};

/// @brief The kinematics of a component.
/// @returns The data in Viam's Spatial Vector Algebra (SVA) format, or URDF.
using KinematicsData =
    boost::variant<KinematicsDataUnspecified, KinematicsDataSVA, KinematicsDataURDF>;

/// @brief Response from get_kinematics containing kinematics data and optional meshes.
struct KinematicsResponse {
    /// The kinematics data in SVA or URDF format.
    KinematicsData kinematics_data;
    /// Map of URDF filepaths to their associated meshes. Only populated for URDF format.
    std::map<std::string, mesh> meshes_by_urdf_filepath;
};

bool operator==(const KinematicsResponse& lhs, const KinematicsResponse& rhs);

namespace proto_convert_details {

template <>
struct to_proto_impl<KinematicsData> {
    void operator()(const KinematicsData&, common::v1::GetKinematicsResponse*) const;
};

template <>
struct to_proto_impl<KinematicsResponse> {
    void operator()(const KinematicsResponse&, common::v1::GetKinematicsResponse*) const;
};

template <>
struct from_proto_impl<common::v1::GetKinematicsResponse> {
    KinematicsData operator()(const common::v1::GetKinematicsResponse*) const;
};

}  // namespace proto_convert_details
}  // namespace sdk
}  // namespace viam
