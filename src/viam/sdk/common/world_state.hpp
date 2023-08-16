#pragma once

#include <common/v1/common.pb.h>

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
        common::v1::GeometriesInFrame to_proto() const;
        static geometries_in_frame from_proto(const common::v1::GeometriesInFrame& proto);
    };

    struct transform {
        std::string reference_frame;
        pose_in_frame pose_in_observer_frame;
        std::shared_ptr<GeometryConfig> physical_object;

        common::v1::Transform to_proto() const;
        static transform from_proto(const common::v1::Transform& proto);
        transform() {}
    };

    common::v1::WorldState to_proto() const;
    static WorldState from_proto(const common::v1::WorldState& ws);

    WorldState() {}
    WorldState(std::vector<geometries_in_frame> obstacles, std::vector<transform> transforms)
        : obstacles_(obstacles), transforms_(transforms) {}

    friend bool operator==(const WorldState& lhs, const WorldState& rhs);
    friend bool operator==(const geometries_in_frame& lhs, const geometries_in_frame& rhs);
    friend bool operator==(const transform& lhs, const transform& rhs);

   private:
    std::vector<geometries_in_frame> obstacles_;
    std::vector<transform> transforms_;
};

}  // namespace sdk
}  // namespace viam
