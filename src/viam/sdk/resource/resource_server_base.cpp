#include <viam/sdk/resource/resource_server_base.hpp>

namespace viam {
namespace sdk {
const std::shared_ptr<ResourceManager>& ResourceServer::resource_manager() const {
    return manager_;
};

std::shared_ptr<ResourceManager>& ResourceServer::resource_manager() {
    return manager_;
}

}  // namespace sdk
}  // namespace viam
