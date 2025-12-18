#pragma once

#include <boost/variant/variant.hpp>
#include <functional>
#include <vector>

#include <viam/sdk/common/proto_convert.hpp>
#include <viam/sdk/common/proto_value.hpp>

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

/// @brief Kinematics data in URDF format
struct KinematicsDataURDF : raw_bytes<KinematicsDataURDF>, EqCompare<KinematicsDataURDF> {
    using raw_bytes<KinematicsDataURDF>::raw_bytes;
};

/// @brief The kinematics of a component.
/// @returns The data in Viam's Spatial Vector Algebra (SVA) format, or URDF.
using KinematicsData =
    boost::variant<KinematicsDataUnspecified, KinematicsDataSVA, KinematicsDataURDF>;

/// @brief Helper function to get kinematics data without extra parameters.
/// @param get_kinematics_func Function that takes ProtoStruct and returns KinematicsData.
/// @return A variant of kinematics data.
KinematicsData get_kinematics(
    std::function<KinematicsData(const ProtoStruct&)> get_kinematics_func);

namespace viam {
namespace common {
namespace v1 {
class GetKinematicsResponse;
enum KinematicsFileFormat : int;
}  // namespace v1
}  // namespace common
}  // namespace viam

/// @brief Convert a GetKinematicsResponse protobuf message to KinematicsData.
/// @param response The protobuf response containing format and kinematics_data.
/// @return A variant of kinematics data.
KinematicsData kinematics_from_proto(const common::v1::GetKinematicsResponse& response);

/// @brief Convert KinematicsData to a GetKinematicsResponse protobuf message.
/// @param kinematics The kinematics data variant to convert.
/// @param response The protobuf response to populate with format and kinematics_data.
void kinematics_to_proto(const KinematicsData& kinematics,
                         common::v1::GetKinematicsResponse* response);

namespace proto_convert_details {

template <>
struct to_proto_impl<KinematicsData> {
    void operator()(const KinematicsData&, common::v1::GetKinematicsResponse*) const;
};

template <>
struct from_proto_impl<common::v1::GetKinematicsResponse> {
    KinematicsData operator()(const common::v1::GetKinematicsResponse*) const;
};

}  // namespace proto_convert_details
}  // namespace sdk
}  // namespace viam
