/// @file common/kinematics.hpp
///
/// @brief Kinematics data types shared by the components that serve `GetKinematics`.
#pragma once

#include <boost/optional/optional.hpp>
#include <boost/variant/variant.hpp>
#include <map>
#include <string>
#include <vector>

#include <viam/sdk/common/linear_algebra.hpp>
#include <viam/sdk/common/mesh.hpp>
#include <viam/sdk/common/proto_convert.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>
#include <viam/sdk/spatialmath/orientation.hpp>

namespace viam {
namespace common {
namespace v1 {

class GetKinematicsResponse;
class KinematicModel;
class KinematicLink;
class KinematicJoint;
class LinkPose;
class MimicJoint;
class KinematicProperties;

}  // namespace v1
}  // namespace common
}  // namespace viam

namespace viam {
namespace sdk {

/// @brief Options for a `GetKinematics` request.
struct kinematics_options {
    /// @brief Include collision mesh bytes in the response. Otherwise meshes carry only their
    /// path.
    bool include_collision_meshes = false;

    /// @brief Include visual mesh bytes in the response.
    bool include_visual_meshes = false;
};

/// @brief Pose of a link relative to its parent: translation in mm, orientation in degrees.
/// Identity when default constructed.
struct LinkPose {
    Vector3 translation{};

    /// @brief One of `quaternion`, `orientation_vector_degrees`, `euler_angles` or `axis_angles`.
    /// An `orientation_vector` (radians) is accepted and sent as degrees.
    Orientation orientation = orientation_vector_degrees{0, 0, 1, 0};

    friend bool operator==(const LinkPose& lhs, const LinkPose& rhs);
};

/// @brief A revolute joint. Limits are in degrees, deg/s and deg/s^2; absent means unbounded.
struct RevoluteJoint {
    boost::optional<double> min_deg;
    boost::optional<double> max_deg;
    boost::optional<double> max_velocity_deg_per_sec;
    boost::optional<double> max_acceleration_deg_per_sec2;

    friend bool operator==(const RevoluteJoint& lhs, const RevoluteJoint& rhs);
};

/// @brief A revolute joint with no position bound. Limits are in deg/s and deg/s^2; absent means
/// unbounded.
struct ContinuousJoint {
    boost::optional<double> max_velocity_deg_per_sec;
    boost::optional<double> max_acceleration_deg_per_sec2;

    friend bool operator==(const ContinuousJoint& lhs, const ContinuousJoint& rhs);
};

/// @brief A prismatic joint. Limits are in mm, mm/s and mm/s^2; absent means unbounded.
struct PrismaticJoint {
    boost::optional<double> min_mm;
    boost::optional<double> max_mm;
    boost::optional<double> max_velocity_mm_per_sec;
    boost::optional<double> max_acceleration_mm_per_sec2;

    /// @brief Geometry swept by the joint.
    boost::optional<GeometryConfig> geometry;

    friend bool operator==(const PrismaticJoint& lhs, const PrismaticJoint& rhs);
};

/// @brief The kind of a joint together with its kind-specific limits.
using joint_spec = boost::variant<RevoluteJoint, PrismaticJoint, ContinuousJoint>;

/// @brief A joint whose position is `multiplier * position(joint) + offset`.
struct MimicJoint {
    std::string joint;
    double multiplier = 0.0;
    double offset = 0.0;

    friend bool operator==(const MimicJoint& lhs, const MimicJoint& rhs);
};

/// @brief Properties of a kinematic component as a whole.
struct KinematicProperties {
    /// @brief Rate the component expects to be driven at.
    boost::optional<double> operating_freq_hz;

    /// @brief Maximum linear speed of the tool center point.
    boost::optional<double> max_linear_tcp_speed_mm_per_sec;

    friend bool operator==(const KinematicProperties& lhs, const KinematicProperties& rhs);
};

/// @brief A link of a kinematic model.
struct KinematicLink {
    std::string id;

    /// @brief Parent link or joint id, empty for the root.
    std::string parent;

    /// @brief Pose relative to the parent.
    LinkPose pose;

    /// @brief Collision geometries. Meshes must be stl or ply.
    std::vector<GeometryConfig> collision;

    /// @brief Display geometries, not used for collision checking.
    std::vector<GeometryConfig> visual;

    friend bool operator==(const KinematicLink& lhs, const KinematicLink& rhs);
};

/// @brief A joint of a kinematic model.
struct KinematicJoint {
    std::string id;

    /// @brief Parent link id.
    std::string parent;

    /// @brief Axis of rotation or translation, in the parent link frame.
    Vector3 axis{};

    /// @brief Joint kind and its limits.
    joint_spec spec;

    boost::optional<MimicJoint> mimic;

    friend bool operator==(const KinematicJoint& lhs, const KinematicJoint& rhs);
};

/// @brief A component's kinematics as a tree of links and joints. Its proto JSON encoding is
/// also the on-disk kinematics file format.
struct KinematicModel {
    /// @brief Set to the resource name by the SDK when served.
    std::string name;

    std::vector<KinematicLink> links;
    std::vector<KinematicJoint> joints;

    /// @brief Frames whose poses the model reports.
    std::vector<std::string> output_frames;

    boost::optional<KinematicProperties> properties;

    friend bool operator==(const KinematicModel& lhs, const KinematicModel& rhs);
};

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
struct [[deprecated("raw-bytes kinematics are deprecated, use KinematicModel")]]
KinematicsDataUnspecified : raw_bytes<KinematicsDataUnspecified>,
                            EqCompare<KinematicsDataUnspecified> {};

/// @brief Kinematics data in Viam's Spatial Vector Algebra (SVA) format
struct [[deprecated("raw-bytes kinematics are deprecated, use KinematicModel")]] KinematicsDataSVA
    : raw_bytes<KinematicsDataSVA>,
      EqCompare<KinematicsDataSVA> {
    using raw_bytes<KinematicsDataSVA>::raw_bytes;
};

/// @brief Kinematics data in URDF format with optional meshes by URDF filepath
struct [[deprecated("raw-bytes kinematics are deprecated, use KinematicModel")]] KinematicsDataURDF
    : raw_bytes<KinematicsDataURDF> {
    using raw_bytes<KinematicsDataURDF>::raw_bytes;
    std::map<std::string, mesh> meshes_by_urdf_filepath;

    friend bool operator==(const KinematicsDataURDF& lhs, const KinematicsDataURDF& rhs);
};

/// @brief The kinematics of a component as a raw file.
/// @returns The data in Viam's Spatial Vector Algebra (SVA) format, or URDF.
using KinematicsData [[deprecated("use KinematicModel, served by get_kinematic_model")]] =
    boost::variant<KinematicsDataUnspecified, KinematicsDataSVA, KinematicsDataURDF>;

namespace proto_convert_details {

template <>
struct to_proto_impl<LinkPose> {
    void operator()(const LinkPose&, common::v1::LinkPose*) const;
};

template <>
struct from_proto_impl<common::v1::LinkPose> {
    LinkPose operator()(const common::v1::LinkPose*) const;
};

template <>
struct to_proto_impl<MimicJoint> {
    void operator()(const MimicJoint&, common::v1::MimicJoint*) const;
};

template <>
struct from_proto_impl<common::v1::MimicJoint> {
    MimicJoint operator()(const common::v1::MimicJoint*) const;
};

template <>
struct to_proto_impl<KinematicProperties> {
    void operator()(const KinematicProperties&, common::v1::KinematicProperties*) const;
};

template <>
struct from_proto_impl<common::v1::KinematicProperties> {
    KinematicProperties operator()(const common::v1::KinematicProperties*) const;
};

template <>
struct to_proto_impl<KinematicLink> {
    void operator()(const KinematicLink&, common::v1::KinematicLink*) const;
};

template <>
struct from_proto_impl<common::v1::KinematicLink> {
    KinematicLink operator()(const common::v1::KinematicLink*) const;
};

template <>
struct to_proto_impl<KinematicJoint> {
    void operator()(const KinematicJoint&, common::v1::KinematicJoint*) const;
};

template <>
struct from_proto_impl<common::v1::KinematicJoint> {
    KinematicJoint operator()(const common::v1::KinematicJoint*) const;
};

template <>
struct to_proto_impl<KinematicModel> {
    void operator()(const KinematicModel&, common::v1::KinematicModel*) const;
};

template <>
struct from_proto_impl<common::v1::KinematicModel> {
    KinematicModel operator()(const common::v1::KinematicModel*) const;
};

// The converters for the deprecated raw-bytes data stay available for callers that still speak
// that format; naming the type here is the only use that is not itself deprecated.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

template <>
struct to_proto_impl<KinematicsData> {
    void operator()(const KinematicsData&, common::v1::GetKinematicsResponse*) const;
};

template <>
struct from_proto_impl<common::v1::GetKinematicsResponse> {
    KinematicsData operator()(const common::v1::GetKinematicsResponse*) const;
};

#pragma GCC diagnostic pop

}  // namespace proto_convert_details
}  // namespace sdk
}  // namespace viam
