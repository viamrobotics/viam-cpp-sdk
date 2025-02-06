#include <viam/sdk/components/generic.hpp>

namespace viam {
namespace sdk {

API GenericComponent::api() const {
    return API::get<GenericComponent>();
}

API API::traits<GenericComponent>::api() {
    return {kRDK, kComponent, "generic"};
}

GenericComponent::GenericComponent(std::string name) : Component(std::move(name)){};

}  // namespace sdk
}  // namespace viam
