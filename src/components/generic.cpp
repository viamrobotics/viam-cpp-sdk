#include <component/generic/v1/generic.grpc.pb.h>
#include <component/generic/v1/generic.pb.h>
#include <google/protobuf/descriptor.h>

#include <common/utils.hpp>
#include <components/generic.hpp>
#include <registry/registry.hpp>
#include <resource/resource.hpp>
#include <stdexcept>

std::shared_ptr<ResourceSubtype> Generic::resource_subtype() {
    const google::protobuf::DescriptorPool* p = google::protobuf::DescriptorPool::generated_pool();
    const google::protobuf::ServiceDescriptor* sd =
        p->FindServiceByName(viam::component::generic::v1::GenericService::service_full_name());
    if (sd == nullptr) {
        throw std::runtime_error("Unable to get service descriptor for the generic service");
    }
    return ResourceSubtype::new_from_descriptor(sd);
}

Subtype Generic::subtype() {
    return Subtype(RDK, COMPONENT, "generic");
}

bool Generic::init() {
    Registry::register_subtype(subtype(), resource_subtype());
    return true;
};

bool Generic::inited = init();

