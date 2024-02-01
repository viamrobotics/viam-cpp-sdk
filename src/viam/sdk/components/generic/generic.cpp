#include <viam/sdk/components/generic/generic.hpp>

#include <stdexcept>

#include <google/protobuf/descriptor.h>

#include <viam/api/component/generic/v1/generic.grpc.pb.h>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

API GenericComponent::api() const {
    return API::get<GenericComponent>();
}

API API::traits<GenericComponent>::api() {
    return {kRDK, kComponent, "generic"};
}

GenericComponent::GenericComponent(std::string name) : Component(std::move(name)){};

}  // namespace sdk
}  // namespace viam
