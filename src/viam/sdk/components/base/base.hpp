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

namespace viam {
namespace sdk {

/// @defgroup Base Classes related to the `Base` component.

/// @class BaseSubtype
/// @brief Defines a `ResourceSubtype` for the `Base` component.
/// @ingroup Base
class BaseSubtype : public ResourceSubtype {
   public:
    std::shared_ptr<ResourceServer> create_resource_server(
        std::shared_ptr<ResourceManager> manager) override;
    std::shared_ptr<Resource> create_rpc_client(std::string name,
                                                std::shared_ptr<grpc::Channel> chan) override;
    BaseSubtype(const google::protobuf::ServiceDescriptor* service_descriptor)
        : ResourceSubtype(service_descriptor){};
};

/// @class Base base.hpp "components/base/base.hpp"
/// @brief A `Base` is the platform that the other parts of a mobile robot attach to.
/// @ingroup Base
///
/// This acts as an abstract parent class to be inherited from by any drivers representing
/// specific base implementations. This class cannot be used on its own.
class Base : public Component {
   public:
    // functions shared across all components
    static std::shared_ptr<ResourceSubtype> resource_subtype();
    static Subtype static_subtype();

    /// @brief Move a robot's base in a straight line by a given distance. This method blocks
    /// until completed or cancelled
    /// @param distance_mm Desired travel distance in millimeters
    /// @param mm_per_sec Desired travel velocity in millimeters/second
    virtual void move_straight(int64_t distance_mm, double mm_per_sec) = 0;

    /// @brief Spins a robot's base by an given angle, expressed in degrees. This method blocks
    /// until completed or cancelled
    /// @param angle_deg Desired angle
    /// @param degs_per_sec Desired angular velocity
    virtual void spin(double angle_deg, double degs_per_sec) = 0;

    /// @brief Sets the linear and angular power of a base -1 -> 1 in terms of power for
    /// each direction
    /// @param linear Desired linear power percentage (-1 <= % <= 1) for each direction
    /// @param angular Desired angular power percentage (-1 <= % <= 1) for each direction
    virtual void set_power(const Vector3& linear, const Vector3& angular) = 0;

    /// @brief Set the linear and angular velocity of a base
    /// @param linear Desired linear velocity in mm per second for each direction
    /// @param angular Desired angular velocity in degrees per second for each direction
    virtual void set_velocity(const Vector3& linear, const Vector3& angular) = 0;

    /// @brief Stops a robot's base
    virtual grpc::StatusCode stop() override = 0;

    /// @brief Reports if the base is in motion
    virtual bool is_moving() = 0;

    /// @brief Send/receive arbitrary commands to the resource.
    /// @param Command the command to execute.
    /// @return The result of the executed command.
    virtual AttributeMap do_command(AttributeMap command) = 0;

    Subtype dynamic_subtype() const override;

   protected:
    explicit Base(std::string name);
};

}  // namespace sdk
}  // namespace viam
