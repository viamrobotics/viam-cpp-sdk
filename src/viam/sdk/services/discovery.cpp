#include <viam/sdk/services/discovery.hpp>

namespace viam {
namespace sdk {

API Discovery::api() const {
    return API::get<Discovery>();
}

API::traits<Discovery>::api() {
    return {kRDK, kService, "discovery"};
}

Discovery::Discovery(std::string name) : Service(std::move(name)) {}

}  // namespace sdk
}  // namespace viam
