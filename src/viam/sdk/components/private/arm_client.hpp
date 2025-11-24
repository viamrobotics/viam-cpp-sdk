/// @file components/private/arm_client.hpp
///
/// @brief Implements a gRPC client for the `Arm` component
#pragma once

#include <viam/api/component/arm/v1/arm.grpc.pb.h>

#include <viam/sdk/components/arm.hpp>
#include <viam/sdk/rpc/dial.hpp>

namespace viam {
namespace sdk {
namespace impl {

/// @class ArmClient
/// @brief gRPC client implementation of an `Arm` component.
/// @ingroup Arm
class ArmClient : public Arm {
   public:
    using interface_type = Arm;
    ArmClient(std::string name, const ViamChannel& channel);

    const ViamChannel& channel() const {
        return *channel_;
    }

    pose get_end_position(const ProtoStruct& extra) override;
    void move_to_position(const pose& pose, const ProtoStruct& extra) override;
    std::vector<double> get_joint_positions(const ProtoStruct& extra) override;
    void move_to_joint_positions(const std::vector<double>& positions,
                                 const ProtoStruct& extra) override;
    void move_through_joint_positions(const std::vector<std::vector<double>>& positions,
                                      const Arm::MoveOptions& options,
                                      const ProtoStruct& extra) override;
    bool is_moving() override;
    void stop(const ProtoStruct& extra) override;
    ProtoStruct do_command(const ProtoStruct& command) override;
    Arm::KinematicsData get_kinematics(const ProtoStruct& extra) override;
    std::vector<GeometryConfig> get_geometries(const ProtoStruct& extra) override;

    // Using declarations to introduce convenience overloads of interface which do not need to be
    // passed the ProtoStruct parameter.
    using Arm::get_end_position;
    using Arm::get_geometries;
    using Arm::get_joint_positions;
    using Arm::get_kinematics;
    using Arm::move_through_joint_positions;
    using Arm::move_to_joint_positions;
    using Arm::move_to_position;
    using Arm::stop;

   private:
    using StubType = viam::component::arm::v1::ArmService::StubInterface;
    std::unique_ptr<StubType> stub_;
    const ViamChannel* channel_;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
