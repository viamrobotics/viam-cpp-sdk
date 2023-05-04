#include <viam/sdk/services/service_base.hpp>

#include <string>

#include <viam/api/common/v1/common.pb.h>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/resource/resource_base.hpp>

namespace viam {
namespace sdk {

ResourceName Service::get_resource_name(std::string name) {
    auto r = this->Resource::get_resource_name(name);
    *r.mutable_type() = SERVICE;
    return r;
}

Service::Service() : Resource("service"){};

ResourceType Service::type() const {
    return {SERVICE};
}

}  // namespace sdk
}  // namespace viam
