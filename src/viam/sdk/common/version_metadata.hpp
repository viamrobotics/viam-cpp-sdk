#pragma once

#include <string>

namespace viam {
namespace sdk {

std::string sdk_version();
int sdk_major_version();
int sdk_minor_version();
int sdk_patch_version();

}  // namespace sdk
}  // namespace viam
