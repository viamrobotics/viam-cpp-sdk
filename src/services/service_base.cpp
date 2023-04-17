#include <services/service_base.hpp>

#include <string>

#include <common/v1/common.pb.h>

#include <common/utils.hpp>
#include <resource/resource_base.hpp>

namespace viam {
namespace cppsdk {

ResourceName ServiceBase::get_resource_name(std::string name) {
    auto r = this->ResourceBase::get_resource_name(name);
    *r.mutable_type() = SERVICE;
    return r;
}

ServiceBase::ServiceBase() : ResourceBase("service"){};

ResourceType ServiceBase::type() {
    return {SERVICE};
}

}  // namespace cppsdk
}  // namespace viam
