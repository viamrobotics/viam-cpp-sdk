#include <viam/sdk/components/service_base.hpp>

namespace viam {
namespace sdk {
const ResourceManager ComponentServiceBase::manager() const {
    return manager_;
}
}  // namespace sdk
}  // namespace viam
