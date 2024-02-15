#include <viam/sdk/services/service.hpp>

#include <string>

#include <viam/api/common/v1/common.pb.h>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

common::v1::ResourceName Service::get_resource_name(std::string name) const {
    auto r = this->Resource::get_resource_name(name);
    *r.mutable_type() = kService;
    return r;
}

Service::Service() : Resource("service"){};

}  // namespace sdk
}  // namespace viam
