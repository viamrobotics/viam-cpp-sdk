#include <viam/sdk/resource/resource_server_base.hpp>

namespace viam {
namespace sdk {
const std::shared_ptr<SubtypeService> ResourceServerBase::get_sub_svc() const {
    return sub_svc_;
};

}  // namespace sdk
}  // namespace viam
