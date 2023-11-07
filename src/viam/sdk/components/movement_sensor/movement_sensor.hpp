/// @file components/movement_sensor/movement_sensor.hpp
///
/// @brief Defines a `MovementSensor` component.
#pragma once

#include <string>

#include <viam/api/component/movementsensor/v1/movementsensor.pb.h>

#include <viam/sdk/common/linear_algebra.hpp>
#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/movement_sensor/server.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/spatialmath/orientation.hpp>

namespace viam {
namespace sdk {

/// @defgroup MovementSensor Classes related to the MovementSensor component.

/// @class MovementSensor movementsensor.hpp "components/movement_sensor/movement_sensor.hpp"
/// @ingroup MovementSensor
///
/// This acts as an abstract base class to be inherited from by any drivers representing
/// specific movementsensor implementations. This class cannot be used on its own.
class MovementSensor : public Component {
   public:
    /// @struct compassheading
    struct compassheading {
        /// A number from 0-359 where 0 is North, 90 is East, 180 is South, and 270 is   West
        double value;
    };

    /// @struct orientation
    /// @brief Struct version of viam::common::v1::Orientation.
    struct orientation {
        double o_x, o_y, o_z, theta;
    };

    /// @struct position
    struct position {
        geo_point coordinate;
        float altitude_m;
    };

    /// @struct properties
    struct properties {
        bool linear_velocity_supported;
        bool angular_velocity_supported;
        bool orientation_supported;
        bool position_supported;
        bool compass_heading_supported;
        bool linear_acceleration_supported;
    };

    static API static_api();
    API dynamic_api() const override;

    /// @brief Creates a `compassheading` struct from its proto representation.
    static compassheading from_proto(
        viam::component::movementsensor::v1::GetCompassHeadingResponse proto);

    /// @brief Creates a `position` struct from its proto representation.
    static position from_proto(viam::component::movementsensor::v1::GetPositionResponse proto);

    /// @brief Creates an `orientation` struct from its proto representation.
    static orientation from_proto(viam::common::v1::Orientation proto);

    /// @brief Creates a `properties` struct from its proto representation.
    static properties from_proto(viam::component::movementsensor::v1::GetPropertiesResponse proto);

    /// @brief Converts a `compassheading` struct to its proto representation.
    static viam::component::movementsensor::v1::GetCompassHeadingResponse to_proto(
        compassheading compassheading);

    /// @brief Converts a `position` struct to its proto representation.
    static viam::component::movementsensor::v1::GetPositionResponse to_proto(position position);

    /// @brief Converts an `orientation` struct to its proto representation.
    static viam::common::v1::Orientation to_proto(orientation orientation);

    /// @brief Converts a `properties` struct to its proto representation.
    static viam::component::movementsensor::v1::GetPropertiesResponse to_proto(
        properties properties);

    inline Vector3 get_linear_velocity() {
        return get_linear_velocity({});
    }

    virtual Vector3 get_linear_velocity(const AttributeMap& extra) = 0;

    inline Vector3 get_angular_velocity() {
        return get_angular_velocity({});
    }

    virtual Vector3 get_angular_velocity(const AttributeMap& extra) = 0;

    inline compassheading get_compass_heading() {
        return get_compass_heading({});
    }

    virtual compassheading get_compass_heading(const AttributeMap& extra) = 0;

    inline orientation get_orientation() {
        return get_orientation({});
    }

    virtual orientation get_orientation(const AttributeMap& extra) = 0;

    inline position get_position() {
        return get_position({});
    }

    virtual position get_position(const AttributeMap& extra) = 0;

    inline properties get_properties() {
        return get_properties({});
    }

    virtual properties get_properties(const AttributeMap& extra) = 0;

    inline std::unordered_map<std::string, float> get_accuracy() {
        return get_accuracy({});
    }

    virtual std::unordered_map<std::string, float> get_accuracy(const AttributeMap& extra) = 0;

    inline Vector3 get_linear_acceleration() {
        return get_linear_acceleration({});
    }

    virtual Vector3 get_linear_acceleration(const AttributeMap& extra) = 0;

    /// @brief Send/receive arbitrary commands to the resource.
    /// @param Command the command to execute.
    /// @return The result of the executed command.
    virtual AttributeMap do_command(const AttributeMap& command) = 0;

    /// @brief Returns `GeometryConfig`s associated with the calling movementsensor.
    /// @return The requested `GeometryConfig`s associated with the component.
    inline std::vector<GeometryConfig> get_geometries() {
        return get_geometries({});
    }

    /// @brief Returns `GeometryConfig`s associated with the calling movementsensor.
    /// @param extra Any additional arguments to the method.
    /// @return The requested `GeometryConfig`s associated with the component.
    virtual std::vector<GeometryConfig> get_geometries(const AttributeMap& extra) = 0;

   protected:
    explicit MovementSensor(std::string name) : Component(std::move(name)){};
};

bool operator==(const MovementSensor::compassheading& lhs,
                const MovementSensor::compassheading& rhs);
bool operator==(const MovementSensor::position& lhs, const MovementSensor::position& rhs);
bool operator==(const MovementSensor::properties& lhs, const MovementSensor::properties& rhs);
}  // namespace sdk
}  // namespace viam
