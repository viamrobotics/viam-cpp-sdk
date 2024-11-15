#include <viam/sdk/services/generic.hpp>

#include <google/protobuf/descriptor.h>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

API GenericService::api() const {
    return API::get<GenericService>();
}

API API::traits<GenericService>::api() {
    return {kRDK, kService, "generic"};
}

GenericService::GenericService(std::string name) : Service(std::move(name)) {};

}  // namespace sdk
}  // namespace viam
