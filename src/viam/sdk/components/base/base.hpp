/// @file components/base/base.hpp
///
/// @brief Defines a `Base` component.
#pragma once

#include <string>

#include <viam/api/component/base/v1/base.pb.h>

#include <viam/sdk/common/linear_algebra.hpp>
#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/resource/stoppable.hpp>

namespace viam {
namespace sdk {

/// @defgroup Base Classes related to the Base component.

/// @class BaseRegistration
/// @brief Defines a `ResourceRegistration` for the `Base` component.
/// @ingroup Base
class BaseRegistration : public ResourceRegistration {
   public:
    explicit BaseRegistration(const google::protobuf::ServiceDescriptor* service_descriptor);
    std::shared_ptr<ResourceServer> create_resource_server(
        std::shared_ptr<ResourceManager> manager) override;
    std::shared_ptr<Resource> create_rpc_client(std::string name,
                                                std::shared_ptr<grpc::Channel> chan) override;
};

/// @class Base base.hpp "components/base/base.hpp"
/// @brief A `Base` is the platform that the other parts of a mobile robot attach to.
/// @ingroup Base
///
/// This acts as an abstract parent class to be inherited from by any drivers representing
/// specific base implementations. This class cannot be used on its own.
class Base : public Component, public Stoppable {
   public:
    /// @struct properties
    /// @brief Information about the physical base
    struct properties {
        double width_meters;
        double turning_radius_meters;
        double wheel_circumference_meters;

        static properties from_proto(const component::base::v1::GetPropertiesResponse& proto);
    };
    friend std::ostream& operator<<(std::ostream& os, const properties& v);
    friend bool operator==(const properties& lhs, const properties& rhs);

    // functions shared across all components
    static std::shared_ptr<ResourceRegistration> resource_registration();

    /// @brief Move a robot's base in a straight line by a given distance. This method blocks
    /// until completed or cancelled
    /// @param distance_mm Desired travel distance in millimeters
    /// @param mm_per_sec Desired travel velocity in millimeters/second
    inline void move_straight(int64_t distance_mm, double mm_per_sec) {
        return move_straight(distance_mm, mm_per_sec, {});
    }

    /// @brief Move a robot's base in a straight line by a given distance. This method blocks
    /// until completed or cancelled
    /// @param distance_mm Desired travel distance in millimeters
    /// @param mm_per_sec Desired travel velocity in millimeters/second
    /// @param extra Any additional arguments to the method
    virtual void move_straight(int64_t distance_mm,
                               double mm_per_sec,
                               const AttributeMap& extra) = 0;

    /// @brief Spins a robot's base by an given angle, expressed in degrees. This method blocks
    /// until completed or cancelled
    /// @param angle_deg Desired angle
    /// @param degs_per_sec Desired angular velocity
    inline void spin(double angle_deg, double degs_per_sec) {
        return spin(angle_deg, degs_per_sec, {});
    }

    /// @brief Spins a robot's base by an given angle, expressed in degrees. This method blocks
    /// until completed or cancelled
    /// @param angle_deg Desired angle
    /// @param degs_per_sec Desired angular velocity
    /// @param extra Any additional arguments to the method
    virtual void spin(double angle_deg, double degs_per_sec, const AttributeMap& extra) = 0;

    /// @brief Sets the linear and angular power of a base -1 -> 1 in terms of power for
    /// each direction
    /// @param linear Desired linear power percentage (-1 <= % <= 1) for each direction
    /// @param angular Desired angular power percentage (-1 <= % <= 1) for each direction
    inline void set_power(const Vector3& linear, const Vector3& angular) {
        return set_power(linear, angular, {});
    }

    /// @brief Sets the linear and angular power of a base -1 -> 1 in terms of power for
    /// each direction
    /// @param linear Desired linear power percentage (-1 <= % <= 1) for each direction
    /// @param angular Desired angular power percentage (-1 <= % <= 1) for each direction
    /// @param extra Any additional arguments to the method
    virtual void set_power(const Vector3& linear,
                           const Vector3& angular,
                           const AttributeMap& extra) = 0;

    /// @brief Set the linear and angular velocity of a base
    /// @param linear Desired linear velocity in mm per second for each direction
    /// @param angular Desired angular velocity in degrees per second for each direction
    inline void set_velocity(const Vector3& linear, const Vector3& angular) {
        return set_velocity(linear, angular, {});
    }

    /// @brief Set the linear and angular velocity of a base
    /// @param linear Desired linear velocity in mm per second for each direction
    /// @param angular Desired angular velocity in degrees per second for each direction
    /// @param extra Any additional arguments to the method
    virtual void set_velocity(const Vector3& linear,
                              const Vector3& angular,
                              const AttributeMap& extra) = 0;

    /// @brief Stops a robot's base
    inline void stop() {
        return stop({});
    }

    /// @brief Stops a robot's base
    /// @param extra Any additional arguments to the method
    virtual void stop(const AttributeMap& extra) override = 0;

    /// @brief Reports if the base is in motion
    virtual bool is_moving() = 0;

    /// @brief Returns physical properties of the base (width, turning radius, wheel circumference)
    inline properties get_properties() {
        return get_properties({});
    }

    /// @brief Returns physical properties of the base (width, turning radius, wheel circumference)
    /// @param extra Any additional arguments to the method
    virtual properties get_properties(const AttributeMap& extra) = 0;

    /// @brief Send/receive arbitrary commands to the resource.
    /// @param Command the command to execute.
    /// @return The result of the executed command.
    virtual AttributeMap do_command(const AttributeMap& command) = 0;

    /// @brief Returns `GeometryConfig`s associated with the calling base.
    /// @return The requested `GeometryConfig`s associated with the component.
    inline std::vector<GeometryConfig> get_geometries() {
        return get_geometries({});
    }

    /// @brief Returns `GeometryConfig`s associated with the calling base.
    /// @param extra Any additional arguments to the method.
    /// @return The requested `GeometryConfig`s associated with the component.
    virtual std::vector<GeometryConfig> get_geometries(const AttributeMap& extra) = 0;

    API api() const override;

   protected:
    explicit Base(std::string name);
};

template <>
struct API::traits<Base> {
    static API api();
};

}  // namespace sdk
}  // namespace viam
