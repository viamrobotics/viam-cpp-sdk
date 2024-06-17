/// @file components/arm.hpp
///
/// @brief Defines an `Arm` component
#pragma once

#include <string>

#include <boost/variant/variant.hpp>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/arm/v1/arm.pb.h>

#include <viam/sdk/common/pose.hpp>
#include <viam/sdk/resource/stoppable.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>

namespace viam {
namespace sdk {

/// @defgroup Arm Classes related to the Arm component.

/// @class Arm arm.hpp "components/arm.hpp"
/// @brief An `Arm` represents a physical robot arm that exists in three-dimensional space.
/// @ingroup Arm
///
/// This acts as an abstract parent class to be inherited from by any drivers representing
/// specific arm implementations. This class cannot be used on its own.
class Arm : public Component, public Stoppable {
    // Base class for use below in defining kinematics data strong typedefs
    template <class Tag>
    struct RawBytes {
        std::vector<unsigned char> bytes;
    };

    // Comparison operator helper for the data types below
    template <class DataType>
    struct EqCompare {
        inline friend bool operator==(const DataType& lhs, const DataType& rhs) {
            return lhs.bytes == rhs.bytes;
        }
    };

   public:
    struct KinematicsDataUnspecified : RawBytes<KinematicsDataUnspecified>,
                                       EqCompare<KinematicsDataUnspecified> {};
    struct KinematicsDataSVA : RawBytes<KinematicsDataSVA>, EqCompare<KinematicsDataSVA> {};
    struct KinematicsDataURDF : RawBytes<KinematicsDataURDF>, EqCompare<KinematicsDataURDF> {};

    /// @brief The kinematics of the component.
    /// @returns The data in Viam's Spatial Vector Algebra (SVA) format, or URDF.
    using KinematicsData =
        boost::variant<KinematicsDataUnspecified, KinematicsDataSVA, KinematicsDataURDF>;

    static KinematicsData from_proto(const viam::common::v1::GetKinematicsResponse& proto);

    /// @brief Get the current position of the end of the arm.
    inline pose get_end_position() {
        return get_end_position({});
    }

    /// @brief Get the current position of the end of the arm.
    /// @param pose The destination pose for the arm.
    /// @param extra Any additional arguments to the method.
    virtual pose get_end_position(const AttributeMap& extra) = 0;

    /// @brief Move the end of the arm to @param pose.
    inline void move_to_position(const pose& pose) {
        move_to_position(pose, {});
    }

    /// @brief Move the end of the arm to @param pose.
    /// @param pose The destination pose for the arm.
    /// @param extra Any additional arguments to the method.
    virtual void move_to_position(const pose& pose, const AttributeMap& extra) = 0;

    /// @brief Lists the joint positions in degrees of every joint on a robot arm
    inline std::vector<double> get_joint_positions() {
        return get_joint_positions({});
    }

    /// @brief Lists the joint positions in degrees of every joint on a robot arm
    /// @param extra Any additional arguments to the method.
    virtual std::vector<double> get_joint_positions(const AttributeMap& extra) = 0;

    /// @brief Move each joint on the arm to the corresponding angle specified in @param positions
    /// @param extra Any additional arguments to the method.
    virtual void move_to_joint_positions(const std::vector<double>& positions,
                                         const AttributeMap& extra) = 0;

    /// @brief Reports if the arm is in motion.
    virtual bool is_moving() = 0;

    /// @brief Send/receive arbitrary commands to the resource.
    /// @param Command the command to execute.
    /// @return The result of the executed command.
    virtual AttributeMap do_command(const AttributeMap& command) = 0;

    /// @brief Get the kinematics data associated with the arm.
    /// @param extra Any additional arguments to the method.
    /// @return A variant of kinematics data, with bytes field containing the raw bytes of the file
    /// and the object's type indicating the file format.
    virtual KinematicsData get_kinematics(const AttributeMap& extra) = 0;

    /// @brief Get the kinematics data associated with the arm.
    /// @return A variant of kinematics data, with bytes field containing the raw bytes of the file
    /// and the object's type indicating the file format.
    inline KinematicsData get_kinematics() {
        return get_kinematics({});
    }

    /// @brief Returns `GeometryConfig`s associated with the calling arm
    inline std::vector<GeometryConfig> get_geometries() {
        return get_geometries({});
    }

    /// @brief Returns `GeometryConfig`s associated with the calling arm
    /// @param extra Any additional arguments to the method
    virtual std::vector<GeometryConfig> get_geometries(const AttributeMap& extra) = 0;

    API api() const override;

   protected:
    explicit Arm(std::string name);
};

template <>
struct API::traits<Arm> {
    static API api();
};

}  // namespace sdk
}  // namespace viam
