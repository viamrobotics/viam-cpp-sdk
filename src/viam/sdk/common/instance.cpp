#include <viam/sdk/common/instance.hpp>

#include <atomic>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/private/instance.hpp>
#include <viam/sdk/registry/registry.hpp>

namespace viam {
namespace sdk {

namespace {

// Memory region sentinel to check if object is being destroyed.
std::aligned_storage_t<sizeof(Instance), alignof(Instance)> sentinel;

std::atomic<Instance*> current_instance{nullptr};

}  // namespace

Instance::Instance() {
    Instance* expected = nullptr;

    if (!current_instance.compare_exchange_strong(expected, this)) {
        throw Exception("tried to create duplicate instance");
    }

    impl_ = std::make_unique<Instance::Impl>();
    impl_->registry.initialize();
    impl_->log_mgr.init_logging();
}

Instance::~Instance() {
    current_instance.store(reinterpret_cast<Instance*>(&sentinel));
    impl_.reset();
}

Instance& Instance::current(Instance::Creation creation) {
    if (!current_instance.load()) {
        if (creation == Creation::if_needed) {
            // This variable declaration calls the default ctor, storing a current instance.
            static Instance inst;  // NOLINT (misc-const-correctness)
        } else {
            throw Exception("Instance has not yet been created");
        }
    }

    Instance* current = current_instance.load();

    if (current == reinterpret_cast<Instance*>(&sentinel)) {
        throw Exception("instance was destroyed");
    }

    return *current;
}

}  // namespace sdk
}  // namespace viam
