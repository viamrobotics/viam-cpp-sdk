#pragma once

#include <boost/optional/optional.hpp>

#include <viam/sdk/common/pose.hpp>
#include <viam/sdk/common/proto_convert.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>

VIAM_SDK_API_FWD_NAMESPACE_BEGIN(common)

class GeometriesInFrame;
class Transform;
class WorldState;

VIAM_SDK_API_FWD_NAMESPACE_END

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
        boost::optional<GeometryConfig> physical_object;
    };

    WorldState() = default;

    WorldState(std::vector<geometries_in_frame> obstacles, std::vector<transform> transforms);

    const std::vector<geometries_in_frame>& obstacles() const;
    const std::vector<transform>& transforms() const;

    friend bool operator==(const WorldState& lhs, const WorldState& rhs);
    friend bool operator==(const geometries_in_frame& lhs, const geometries_in_frame& rhs);
    friend bool operator==(const transform& lhs, const transform& rhs);

   private:
    std::vector<geometries_in_frame> obstacles_;
    std::vector<transform> transforms_;
};

namespace proto_convert_details {

template <>
struct to_proto<WorldState::geometries_in_frame> {
    void operator()(const WorldState::geometries_in_frame&, common::v1::GeometriesInFrame*) const;
};

template <>
struct from_proto<common::v1::GeometriesInFrame> {
    WorldState::geometries_in_frame operator()(const common::v1::GeometriesInFrame*) const;
};

template <>
struct to_proto<WorldState::transform> {
    void operator()(const WorldState::transform&, common::v1::Transform*) const;
};

template <>
struct from_proto<common::v1::Transform> {
    WorldState::transform operator()(const common::v1::Transform*) const;
};

template <>
struct to_proto<WorldState> {
    void operator()(const WorldState&, common::v1::WorldState*) const;
};

template <>
struct from_proto<common::v1::WorldState> {
    WorldState operator()(const common::v1::WorldState*) const;
};

}  // namespace proto_convert_details
}  // namespace sdk
}  // namespace viam
