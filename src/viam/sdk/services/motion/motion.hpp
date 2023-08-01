/// @file services/motion/motion.hpp
///
/// @brief Defines a `Motion` service.
#pragma once

// CR erodkin: fix this
#include "viam/sdk/common/proto_type.hpp"
#include "viam/sdk/common/world_state.hpp"
#include "viam/sdk/registry/registry.hpp"
#include "viam/sdk/resource/resource_api.hpp"
#include "viam/sdk/services/service.hpp"
#include "viam/sdk/spatialmath/geometry.hpp"
#include <string>
#include <viam/sdk/common/pose_in_frame.hpp>

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
    struct linear_constraint {
        float line_tolerance_mm;
        float orientation_tolerance_degs;
    };
    struct orientation_constraint {
        float orientation_tolerance_degs;
    };
    struct collision_specification {
        // CR erodkin: consider whether this is the right place for allowed_frame_collisions
        struct allowed_frame_collisions {
            std::string frame1;
            std::string frame2;
        };
        std::vector<allowed_frame_collisions> allows;
    };
    struct constraints {
        std::vector<linear_constraint> linear_constraints;
        std::vector<orientation_constraint> orientation_constraints;
        std::vector<collision_specification> collision_specifications;

        static constraints from_proto(const service::motion::v1::Constraints& proto);
    };

    static API static_api();
    API dynamic_api() const override;

    /// @brief Creates a `ResourceRegistration` for the `Motion` service.
    static std::shared_ptr<ResourceRegistration> resource_registration();

    // CR erodkin: flesh out APIs here, add documentation. make sure we have no raw ptrs,
    // only smart ptrs!
    virtual bool move(const PoseInFrame& destination,
                      const Name& name,
                      std::unique_ptr<WorldState> ws,
                      std::unique_ptr<constraints> constraints,
                      const AttributeMap& extra) = 0;

    virtual bool move_on_map(const pose& destination,
                             const Name& component_name,
                             const Name& slam_name,
                             const AttributeMap& extra) = 0;

    virtual bool move_on_globe(const geo_point& destination,
                               const Name& component_name,
                               const Name& movement_sensor_name,
                               std::vector<geo_obstacle> obstacles,
                               double* heading,
                               double* linear_meters_per_sec,
                               double* angular_deg_per_sec,
                               AttributeMap extra) = 0;

    virtual bool move_single_component(const PoseInFrame& destination,
                                       const Name& component_name,
                                       const WorldState& world_state,
                                       AttributeMap extra) = 0;

    virtual PoseInFrame get_pose(const Name& component_name,
                                 const std::string& destination_frame,
                                 std::vector<WorldState::transform> supplemental_transforms,
                                 AttributeMap extra) = 0;

    virtual AttributeMap do_command(const AttributeMap& command) = 0;

   protected:
    explicit Motion(std::string name);
};

}  // namespace sdk
}  // namespace viam
