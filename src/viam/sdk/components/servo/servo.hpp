/// @file components/servo/servo.hpp
///
/// @brief Defines a `Servo` component.
#pragma once

#include <viam/api/component/servo/v1/servo.pb.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/resource/stoppable.hpp>

namespace viam {
  namespace sdk {

    /// @class ServoRegistration
    /// @brief Defines a `ResourceRegistration` for the `Servo` component.
    /// @ingroup Servo
    class ServoRegistration : public ResourceRegistration {
      public:
        explicit ServoRegistration(const google::protobuf::ServiceDescriptor* service_descriptor);
        std::shared_ptr<ResourceServer> create_resource_server(
            std::shared_ptr<ResourceManager> manager) override;
        std::shared_ptr<Resource> create_rpc_client(std::string name,
                                                    std::shared_ptr<grpc::Channel> chan) override;
    };

    /// @defgroup Servo Classes related to the Servo component.
 
    /// @class Servo servo.hpp "components/servo/servo.hpp"
    /// @ingroup Servo
    ///
    /// This acts as an abstract base class to be inherited form by any drivers representing
    /// specific servo implementations. This class cannot be used on its own.
    class Servo : public Component, public Stoppable {
      public:
        static API static_api();
        API dynamic_api() const override;

        /// @brief Creates a `position` struct from its proto representation.
        static position from_proto(viam::component::servo:v1::GetPositionResponse proto);

        /// @brief Move the servo to the provided angle
        /// @param angle_deg The desired angle of the servo in degrees. 
        inline void move(uint32_t angle_deg) {
          return move(angle_deg, {});
        }

        /// @brief Move the servo to the provided angle
        /// @param angle_deg The desired angle of the servo in degrees. 
        /// @param extra Any additional arguments to the method.
        virtual void move(uint32_t angle_deg, const AttributeMap& extra) = 0;

        /// @brief Get the current angle (degrees) of the servo.
        inline position get_position() {
          return get_position({});
        }

        /// @brief Reports the position of the robot's servo relative to its zero position.
        /// @param extra Any additional arguments to the method
        /// @throws runtime_error if position reporting is not supported
        virtual position get_position(const AttributeMap& extra) = 0;

        /// @brief Stops a resource from running.
        inline void stop() {
            return stop({});
        }

        /// @brief Stops a resource from running.
        /// @param extra Extra arguments to pass to the resource's `stop` method.
        void stop(const AttributeMap& extra) override = 0;

        /// @brief Reports if a component is in motion
        virtual bool is_moving() = 0;

        /// @brief Send/receive arbitrary commands to the resource.
        /// @param Command the command to execute.
        /// @return The result of the executed command.
        virtual AttributeMap do_command(const AttributeMap& command) = 0;

        /// @brief Returns `GeometryConfig`s associated with the calling servo.
        /// @return The requested `GeometryConfig`s associated with the component.
        inline std::vector<GeometryConfig> get_geometries() {
            return get_geometries({});
        }

        /// @brief Returns `GeometryConfig`s associated with the calling servo.
        /// @param extra Any additional arguments to the method.
        /// @return The requested `GeometryConfig`s associated with the component.
        virtual std::vector<GeometryConfig> get_geometries(const AttributeMap& extra) = 0;

      protected:
        explicit Servo(std::string name) : Component(std::move(name)){};
    };
  }  // namespace sdk
}  // namespace viam