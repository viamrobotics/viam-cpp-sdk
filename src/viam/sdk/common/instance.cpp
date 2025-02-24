#include <viam/sdk/common/instance.hpp>

#include <viam/sdk/registry/registry.hpp>

namespace viam {
namespace sdk {

struct Instance::Impl {
    Registry registry;
};

Instance::Instance() : impl_(std::make_unique<Instance::Impl>()) {
    impl_->registry.initialize();
}

Instance::~Instance() = default;

Registry* Instance::registry() {
    return &(impl_->registry);
}

}  // namespace sdk
}  // namespace viam
