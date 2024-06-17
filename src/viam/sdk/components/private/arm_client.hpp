/// @file components/private/arm_client.hpp
///
/// @brief Implements a gRPC client for the `Arm` component
#pragma once

#include <grpcpp/channel.h>

#include <viam/api/component/arm/v1/arm.grpc.pb.h>

#include <viam/sdk/components/arm.hpp>

namespace viam {
namespace sdk {
namespace impl {

/// @class ArmClient
/// @brief gRPC client implementation of an `Arm` component.
/// @ingroup Arm
class ArmClient : public Arm {
   public:
    using interface_type = Arm;
    ArmClient(std::string name, std::shared_ptr<grpc::Channel> channel);

    pose get_end_position(const AttributeMap& extra) override;
    void move_to_position(const pose& pose, const AttributeMap& extra) override;
    std::vector<double> get_joint_positions(const AttributeMap& extra) override;
    void move_to_joint_positions(const std::vector<double>& positions, const AttributeMap& extra) override;
    bool is_moving() override;
    void stop(const AttributeMap& extra) override;
    AttributeMap do_command(const AttributeMap& command) override;
    Arm::KinematicsData get_kinematics(const AttributeMap& extra) override;
    std::vector<GeometryConfig> get_geometries(const AttributeMap& extra) override;


    // Using declarations to introduce convenience overloads of interface which do not need to be passed the 
    // AttributeMap parameter. 
    using Arm::get_end_position;
    using Arm::move_to_position;
    using Arm::get_joint_positions;
    using Arm::move_to_joint_positions;
    using Arm::stop;
    using Arm::get_kinematics;
    using Arm::get_geometries;

   private:
    using StubType = viam::component::arm::v1::ArmService::StubInterface;
    std::unique_ptr<StubType> stub_;
    std::shared_ptr<grpc::Channel> channel_;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
