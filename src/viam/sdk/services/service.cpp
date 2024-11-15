#include <viam/sdk/services/service.hpp>

#include <string>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

Name Service::get_resource_name() const {
    return Resource::get_resource_name(kService);
}

Service::Service() : Resource("service"){};

}  // namespace sdk
}  // namespace viam
