#include <viam/sdk/resource/reconfigurable.hpp>

#include <memory>

namespace viam {
namespace sdk {

Reconfigurable::~Reconfigurable() = default;
Reconfigurable::Reconfigurable() = default;

void Reconfigurable::reconfigure_if_reconfigurable(const std::shared_ptr<Resource>& resource,
                                                   const Dependencies& deps,
                                                   const ResourceConfig& cfg) {
    auto reconfigurable_res = std::dynamic_pointer_cast<Reconfigurable>(resource);
    if (reconfigurable_res) {
        reconfigurable_res->reconfigure(deps, cfg);
    }
}

}  // namespace sdk
}  // namespace viam
