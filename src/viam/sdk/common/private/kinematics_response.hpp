#pragma once

#include <string>

#include <viam/sdk/common/kinematics.hpp>

namespace viam {
namespace sdk {
namespace impl {

/// @brief Put a component's model on a GetKinematics response, named for the resource whatever
/// name it came with, since that is what a client addresses it by. Mesh bytes go out as the
/// component returned them, so leaving out the ones a request did not ask for is up to the
/// component.
void set_response_model(const KinematicModel& model,
                        const std::string& resource_name,
                        common::v1::GetKinematicsResponse* response);

}  // namespace impl
}  // namespace sdk
}  // namespace viam
