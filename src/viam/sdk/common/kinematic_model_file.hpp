/// @file common/kinematic_model_file.hpp
///
/// @brief Reads a kinematic model file into a KinematicModel.
#pragma once

#include <string>

#include <viam/sdk/common/kinematics.hpp>

namespace viam {
namespace sdk {

/// @brief Read a kinematic model file, which is the proto JSON of a KinematicModel.
///
/// The parse is strict, so a field the message does not have, a number that is not finite, a
/// parent that is not in the model, a cycle or a mimic of something that is not a joint all fail
/// the read. A mesh that names a path and carries no bytes is read from that path, relative to the
/// model file, and comes back with its bytes, the content type taken from the extension and the
/// path as it was written. A mesh that already carries bytes is left alone.
///
/// The name on the model is the one in the file. A component server replaces it with the
/// resource name when the model is served.
///
/// @throws Exception naming the file and the link, joint or mesh at fault.
KinematicModel read_kinematic_model_file(const std::string& path);

}  // namespace sdk
}  // namespace viam
