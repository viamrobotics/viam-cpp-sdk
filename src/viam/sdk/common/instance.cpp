#include <viam/sdk/common/instance.hpp>

namespace viam {
namespace sdk {

Instance::Instance() {
    registry_.initialize();
}

Registry& Instance::registry() {
    return registry_;
}

}  // namespace sdk
}  // namespace viam
