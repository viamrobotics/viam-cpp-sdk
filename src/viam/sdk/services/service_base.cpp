#include <viam/sdk/services/service_base.hpp>

#include <string>

#include <viam/api/common/v1/common.pb.h>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/resource/resource_base.hpp>

namespace viam {
namespace sdk {

ResourceName ServiceBase::get_resource_name(std::string name) {
    auto r = this->ResourceBase::get_resource_name(name);
    *r.mutable_type() = SERVICE;
    return r;
}

ServiceBase::ServiceBase() : ResourceBase("service"){};

ResourceType ServiceBase::type() {
    return {SERVICE};
}

}  // namespace sdk
}  // namespace viam
