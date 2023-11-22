/// @file services/motion/client.hpp
///
/// @brief Implements a gRPC client for the `Motion` service.
#pragma once

#include <viam/api/service/motion/v1/motion.grpc.pb.h>

#include <viam/sdk/services/motion/motion.hpp>

namespace viam {
namespace sdk {

/// @class MotionClient
/// @brief gRPC client implementation of a `Motion` service.
/// @ingroup Motion
class MotionClient : public Motion {
   public:
    MotionClient(std::string name, std::shared_ptr<grpc::Channel> channel);
    bool move(const pose_in_frame& destination,
              const Name& component_name,
              std::shared_ptr<WorldState> world_state,
              std::shared_ptr<constraints> constraints,
              const AttributeMap& extra) override;

    bool move_on_map(const pose& destination,
                     const Name& component_name,
                     const Name& slam_name,
                     const AttributeMap& extra) override;

    bool move_on_globe(const geo_point& destination,
                       const boost::optional<double>& heading,
                       const Name& component_name,
                       const Name& movement_sensor_name,
                       const std::vector<geo_obstacle>& obstacles,
                       std::shared_ptr<motion_configuration> motion_configuration,
                       const AttributeMap& extra) override;

    pose_in_frame get_pose(const Name& component_name,
                           const std::string& destination_frame,
                           const std::vector<WorldState::transform>& supplemental_transforms,
                           const AttributeMap& extra) override;

    AttributeMap do_command(const AttributeMap& command) override;

    // the `extra` param is frequently unnecessary but needs to be supported. Ideally, we'd
    // like to live in a world where implementers of derived classes don't need to go out of
    // their way to support two versions of a method (an `extra` version and a non-`extra`
    // version), and users don't need to pass an unnecessary parameters to all method calls.
    //
    // To do this, we define in the parent resource class a non-virtual version of the methods
    // that calls the virtual method and passes a `nullptr` by default in place of the `extra`
    // param. In order to access these versions of the methods within the client code, however,
    // we need to include these `using` lines.
    using Motion::get_pose;
    using Motion::move;
    using Motion::move_on_globe;
    using Motion::move_on_map;

   private:
    using StubType = service::motion::v1::MotionService::StubInterface;
    std::unique_ptr<StubType> stub_;
    std::shared_ptr<grpc::Channel> channel_;
};

}  // namespace sdk
}  // namespace viam
