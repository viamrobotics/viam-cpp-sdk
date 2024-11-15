#pragma once

#include <viam/api/component/encoder/v1/encoder.grpc.pb.h>
#include <viam/api/component/encoder/v1/encoder.pb.h>

#include <viam/sdk/components/encoder.hpp>

namespace viam {
namespace sdk {
namespace impl {

Encoder::position_type from_proto(viam::component::encoder::v1::PositionType proto);
viam::component::encoder::v1::PositionType to_proto(Encoder::position_type position_type);

}  // namespace impl
}  // namespace sdk
}  // namespace viam

