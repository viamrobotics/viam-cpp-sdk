/// @file services/motion/motion.hpp
///
/// @brief Defines a `Motion` service.
#pragma once

#include "viam/sdk/common/proto_type.hpp"
#include "viam/sdk/registry/registry.hpp"
#include "viam/sdk/resource/resource_api.hpp"
#include "viam/sdk/services/service.hpp"
#include <string>

#include <viam/api/service/motion/v1/motion.pb.h>

namespace viam {
namespace sdk {

/// @defgroup Motion Classes related to the Motion service.

/// @class MotionRegistration
/// @brief Defines a `ResourceRegistration` for the `Motion` service.
/// @ingroup Motion
class MotionRegistration : public ResourceRegistration {
   public:
    explicit MotionRegistration(const google::protobuf::ServiceDescriptor* service_descriptor);
    std::shared_ptr<ResourceServer> create_resource_server(
        std::shared_ptr<ResourceManager> manager) override;
    std::shared_ptr<Resource> create_rpc_client(std::string name,
                                                std::shared_ptr<grpc::Channel> chan) override;
};

class PoseInFrame {};

/// @class Motion motion.hpp "services/motion/motion.hpp"
/// @brief The `Motion` service coordinates motion planning across all components of a given robot.
/// @ingroup Motion
/// The Viam Motion planning service calculates a valid path that avoids self-collision by default.
/// If additional constraints are supplied in the `world_state` message, the motion planning
/// service will account for those as well.
///
/// This acts as an abstract base class to be inherited from by any drivers representing
/// specific motion implementations. This class cannot be used on its own.
class Motion : public Service {
   public:
    // CR erodkin: flesh out APIs here, add documentation
    bool move(const Name& name);

    bool move_single_component(const Name& name);

    bool move_on_globe(const Name& name);

    bool move_on_map(const Name& name);

    PoseInFrame get_pose(const Name& name);

    AttributeMap do_command(AttributeMap command);
};

}  // namespace sdk
}  // namespace viam
