#include <viam/sdk/common/version_metadata.hpp>

#include <sstream>

#include <viam/sdk/common/private/version_metadata.hpp>

namespace viam {
namespace sdk {

std::string sdk_version() {
    std::string version_metadata(impl::k_version);
    version_metadata.erase(0, version_metadata.find(';') + 1);
    return version_metadata.substr(0, version_metadata.find(';'));
}

int get_sub_version(int which) {
    auto version = sdk_version();
    version.erase(0, version.find_first_not_of('v'));

    std::string substr;
    std::stringstream ss(version);

    for (int i = 0; i <= which; ++i) {
        getline(ss, substr, '.');
    }
    return std::stoi(substr);
}

int sdk_major_version() {
    return get_sub_version(0);
}

int sdk_minor_version() {
    return get_sub_version(1);
}

int sdk_patch_version() {
    return get_sub_version(2);
}
}  // namespace sdk
}  // namespace viam
