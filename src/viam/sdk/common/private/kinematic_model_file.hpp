#pragma once

#include <filesystem>

#include <viam/api/common/v1/common.pb.h>

namespace viam {
namespace sdk {
namespace impl {

// Reads a kinematic model file into the message it encodes, with every check and mesh read that
// KinematicModel::from_file documents. The shared fixtures say what a loader has to produce as a
// message, so the test runner compares against this rather than against the sdk type.
// Throws viam::sdk::Exception.
viam::common::v1::KinematicModel read_kinematic_model_file_message(
    const std::filesystem::path& path);

}  // namespace impl
}  // namespace sdk
}  // namespace viam
