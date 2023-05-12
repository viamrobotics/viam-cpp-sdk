#include <viam/sdk/components/camera/server.hpp>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/camera/camera.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/rpc/server.hpp>

namespace viam {
namespace sdk {

CameraServer::CameraServer() : ResourceServer(std::make_shared<ResourceManager>()){};
CameraServer::CameraServer(std::shared_ptr<ResourceManager> manager) : ResourceServer(manager){};

::grpc::Status CameraServer::DoCommand(::grpc::ServerContext* context,
                                       const ::viam::common::v1::DoCommandRequest* request,
                                       ::viam::common::v1::DoCommandResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [DoCommand] without a request");
    };

    std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Camera> camera = std::dynamic_pointer_cast<Camera>(rb);
    AttributeMap result = camera->do_command(struct_to_map(request->command()));

    *response->mutable_result() = map_to_struct(result);

    return ::grpc::Status();
}

::grpc::Status CameraServer::GetImage(::grpc::ServerContext* context,
                                      const ::viam::component::camera::v1::GetImageRequest* request,
                                      ::viam::component::camera::v1::GetImageResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [GetImage] without a request");
    };

    std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Camera> camera = std::dynamic_pointer_cast<Camera>(rb);

    Camera::raw_image image = camera->get_image(request->mime_type());

    std::string img_string = bytes_to_string(image.bytes);

    *response->mutable_mime_type() = image.mime_type;
    *response->mutable_image() = img_string;

    return ::grpc::Status();
}

::grpc::Status CameraServer::RenderFrame(
    ::grpc::ServerContext* context,
    const ::viam::component::camera::v1::RenderFrameRequest* request,
    ::google::api::HttpBody* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [RenderFrame] without a request");
    };

    std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Camera> camera = std::dynamic_pointer_cast<Camera>(rb);

    Camera::raw_image image = camera->get_image(request->mime_type());

    response->set_data(bytes_to_string(image.bytes));
    response->set_content_type(image.mime_type);

    return ::grpc::Status();
}

::grpc::Status CameraServer::GetPointCloud(
    ::grpc::ServerContext* context,
    const ::viam::component::camera::v1::GetPointCloudRequest* request,
    ::viam::component::camera::v1::GetPointCloudResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [GetPointCloud] without a request");
    };

    std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());

    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Camera> camera = std::dynamic_pointer_cast<Camera>(rb);

    Camera::point_cloud point_cloud = camera->get_point_cloud(request->mime_type());

    *response->mutable_mime_type() = "pointcloud/pcd";
    *response->mutable_point_cloud() = bytes_to_string(point_cloud.pc);

    return ::grpc::Status();
}

::grpc::Status CameraServer::GetProperties(
    ::grpc::ServerContext* context,
    const ::viam::component::camera::v1::GetPropertiesRequest* request,
    ::viam::component::camera::v1::GetPropertiesResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [GetProperties] without a request");
    };

    std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Camera> camera = std::dynamic_pointer_cast<Camera>(rb);

    Camera::properties properties = camera->get_properties();

    *response->mutable_distortion_parameters() = Camera::to_proto(properties.distortion_parameters);
    *response->mutable_intrinsic_parameters() = Camera::to_proto(properties.intrinsic_parameters);
    response->set_supports_pcd(properties.supports_pcd);

    return ::grpc::Status();
}

void CameraServer::register_server(std::shared_ptr<Server> server) {
    server->register_service(this);
}

}  // namespace sdk
}  // namespace viam
