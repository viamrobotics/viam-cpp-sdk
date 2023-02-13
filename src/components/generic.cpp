#include <common/utils.hpp>
#include <components/generic.hpp>
#include <resource/resource.hpp>

#include "component/generic/v1/generic.grpc.pb.h"
#include "component/generic/v1/generic.pb.h"
#include "google/protobuf/descriptor.h"
#include "registry/registry.hpp"

// CR erodkin: if this works, we should instead have a way to add subtypes to the registry listed in
// registry, and then each component wrapper can call it on its own
ResourceSubtype Generic::resource_subtype() {
    std::cout << "WE ARE CALLING RESOURCE SUBTYPE SO WE NEED IT DONt DELETE" << std::endl;
    const google::protobuf::DescriptorPool* p = google::protobuf::DescriptorPool::generated_pool();
    // CR erodkin: we should have a way to handle if this doesn't exist?
    const google::protobuf::ServiceDescriptor* sd =
        p->FindServiceByName(viam::component::generic::v1::GenericService::service_full_name());
    return ResourceSubtype(sd);
}

Subtype Generic::subtype() {
    return Subtype(RDK, COMPONENT, "generic");
}
