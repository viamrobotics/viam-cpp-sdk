#include <viam/sdk/services/discovery.hpp>

#include <viam/sdk/common/utils.hpp>

namespace viam {
namespace sdk {

Discovery::Discovery(std::string name) : Service(std::move(name)) {}

API Discovery::api() const {
    return API::get<Discovery>();
}

API API::traits<Discovery>::api() {
    return {kRDK, kService, "discovery"};
}

}  // namespace sdk
}  // namespace viam
