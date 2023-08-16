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
                           AttributeMap extra) override;

    AttributeMap do_command(const AttributeMap& command) override;

   private:
    std::unique_ptr<service::motion::v1::MotionService::StubInterface> stub_;
    std::shared_ptr<grpc::Channel> channel_;
};

}  // namespace sdk
}  // namespace viam

