/// @file services/motion/client.hpp
///
/// @brief Implements a gRPC client for the `Motion` service.
#pragma once

#include "service/motion/v1/motion.grpc.pb.h"
#include "viam/sdk/services/motion/motion.hpp"
namespace viam {
namespace sdk {

/// @class MotionClient
/// @brief gRPC client implementation of a `Motion` service.
/// @ingroup Motion
class MotionClient : public Motion {
   public:
    MotionClient(std::string name, std::shared_ptr<grpc::Channel> channel);
    bool move(const PoseInFrame& destination,
              const Name& component_name,
              std::unique_ptr<WorldState> ws,
              std::unique_ptr<constraints> constraints,
              const AttributeMap& extra) override;

    bool move_on_map(const pose& destination,
                     const Name& component_name,
                     const Name& slam_name,
                     const AttributeMap& extra) override;

    PoseInFrame get_pose(const Name& component_name,
                         const std::string& destination_frame,
                         std::vector<WorldState::transform> supplemental_transforms,
                         AttributeMap extra) override;

    AttributeMap do_command(const AttributeMap& command) override;

   private:
    std::unique_ptr<service::motion::v1::MotionService::StubInterface> stub_;
    std::shared_ptr<grpc::Channel> channel_;
};

}  // namespace sdk
}  // namespace viam

