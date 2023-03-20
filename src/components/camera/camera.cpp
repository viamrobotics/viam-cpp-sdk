#include <component/camera/v1/camera.grpc.pb.h>
#include <component/camera/v1/camera.pb.h>
#include <google/protobuf/descriptor.h>

#include <common/utils.hpp>
#include <components/camera/camera.hpp>
#include <components/camera/server.hpp>
#include <registry/registry.hpp>
#include <resource/resource.hpp>


 std::shared_ptr<ResourceServerBase> CameraSubtype::create_resource_server(
    std::shared_ptr<SubtypeService> svc) {
    return std::make_shared<CameraServer>(svc);
}; 

std::shared_ptr<ResourceSubtype> Camera::resource_subtype() {
    const google::protobuf::DescriptorPool* p = google::protobuf::DescriptorPool::generated_pool();
    const google::protobuf::ServiceDescriptor* sd =
        p->FindServiceByName(viam::component::camera::v1::CameraService::service_full_name());
    if (sd == nullptr) {
        throw std::runtime_error("Unable to get service descriptor for the camera service");
    }
    return std::make_shared<CameraSubtype>(sd);
}

   bool init() {
        Registry::register_subtype(Camera::subtype(), Camera::resource_subtype());
        return true;
    };

    bool inited = init(); 


Subtype Camera::subtype() {
    return Subtype(RDK, COMPONENT, "camera");
}






