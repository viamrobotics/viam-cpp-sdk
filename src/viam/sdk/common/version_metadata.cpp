#include <viam/sdk/common/version_metadata.hpp>

#include <array>
#include <sstream>

#include <viam/sdk/common/private/version_metadata.hpp>

namespace viam {
namespace sdk {

std::string sdk_version() {
    static const std::string result = [] {
        std::string version_metadata(impl::k_version);
        version_metadata.erase(0, version_metadata.find(';') + 1);
        return version_metadata.substr(0, version_metadata.find(';'));
    }();

    return result;
}

int get_sub_version(int which) {
    static const std::array<int, 3> components = [] {
        std::array<int, 3> result;
        auto version = sdk_version();
        version.erase(0, version.find_first_not_of('v'));

        std::string substr;
        std::stringstream ss(version);

        for (int i = 0; i < 3; ++i) {
            getline(ss, substr, '.');
            result[i] = std::stoi(substr);
        }

        return result;
    }();

    return components[which];
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
