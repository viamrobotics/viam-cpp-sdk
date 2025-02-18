#include <viam/sdk/common/instance.hpp>

namespace viam {
namespace sdk {

Instance::Instance() {
    registry_.initialize();
}

}  // namespace sdk
}  // namespace viam
