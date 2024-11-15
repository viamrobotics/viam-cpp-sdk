#pragma once

#include <viam/sdk/common/pose.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>

namespace viam {
namespace sdk {

// TODO(RSDK-4553) Add documentation for these types
class WorldState {
   public:
    struct geometries_in_frame {
        std::vector<GeometryConfig> geometries;
        std::string reference_frame;
    };

    struct transform {
        std::string reference_frame;
        pose_in_frame pose_in_observer_frame;
        std::shared_ptr<GeometryConfig> physical_object;

        transform() {}
    };

    const std::vector<geometries_in_frame>& obstacles() const;
    const std::vector<transform>& transforms() const;

    WorldState() {}
    WorldState(std::vector<geometries_in_frame> obstacles, std::vector<transform> transforms)
        : obstacles_(std::move(obstacles)), transforms_(std::move(transforms)) {}

    friend bool operator==(const WorldState& lhs, const WorldState& rhs);
    friend bool operator==(const geometries_in_frame& lhs, const geometries_in_frame& rhs);
    friend bool operator==(const transform& lhs, const transform& rhs);

   private:
    std::vector<geometries_in_frame> obstacles_;
    std::vector<transform> transforms_;
};

}  // namespace sdk
}  // namespace viam
