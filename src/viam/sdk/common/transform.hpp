#pragma once

#include <common/v1/common.pb.h>
#include <viam/sdk/spatialmath/geometry.hpp>

namespace viam {
namespace sdk {

struct transform {
    viam::common::v1::Transform to_proto() const;
    static transform from_proto(const viam::common::v1::Transform& proto);
    transform(std::string frame, struct pose_in_frame pif, GeometryConfig geometry)
        : reference_frame(std::move(frame)), pif(std::move(pif)), object(std::move(geometry)) {}

    std::string reference_frame;
    pose_in_frame pif;
    GeometryConfig object;
    friend bool operator==(const transform& lhs, const transform& rhs);
};

}  // namespace sdk
}  // namespace viam