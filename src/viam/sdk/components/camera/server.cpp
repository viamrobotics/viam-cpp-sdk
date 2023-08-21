#include <viam/sdk/components/camera/server.hpp>

#include <google/protobuf/util/time_util.h>
#include <grpcpp/support/status.h>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/camera/camera.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/rpc/server.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>

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

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<Camera> camera = std::dynamic_pointer_cast<Camera>(rb);
    const AttributeMap result = camera->do_command(struct_to_map(request->command()));

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

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<Camera> camera = std::dynamic_pointer_cast<Camera>(rb);

    AttributeMap extra;
    if (request->has_extra()) {
        extra = struct_to_map(request->extra());
    }

    const Camera::raw_image image = camera->get_image(request->mime_type(), extra);

    const std::string img_string = bytes_to_string(image.bytes);

    *response->mutable_mime_type() = image.mime_type;
    *response->mutable_image() = img_string;

    return ::grpc::Status();
}

::grpc::Status CameraServer::GetImages(
    ::grpc::ServerContext* context,
    const ::viam::component::camera::v1::GetImagesRequest* request,
    ::viam::component::camera::v1::GetImagesResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [GetImages] without a request");
    };

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<Camera> camera = std::dynamic_pointer_cast<Camera>(rb);

    const Camera::image_collection image_coll = camera->get_images();
    for (const auto& img : image_coll.images) {
        ::viam::component::camera::v1::Image proto_image;
        const std::string img_string = bytes_to_string(img.bytes);
        proto_image.set_source_name(img.source_name);
        proto_image.set_format(
            Camera::MIME_string_to_format(Camera::normalize_mime_type(img.mime_type)));
        proto_image.set_image(img_string);
        *response->mutable_images()->Add() = std::move(proto_image);
    }
    *response->mutable_response_metadata() = response_metadata::to_proto(image_coll.metadata);

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

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<Camera> camera = std::dynamic_pointer_cast<Camera>(rb);

    AttributeMap extra;
    if (request->has_extra()) {
        extra = struct_to_map(request->extra());
    }

    const Camera::raw_image image = camera->get_image(request->mime_type(), extra);

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

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());

    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<Camera> camera = std::dynamic_pointer_cast<Camera>(rb);

    const Camera::point_cloud point_cloud = camera->get_point_cloud(request->mime_type());

    *response->mutable_mime_type() = "pointcloud/pcd";
    *response->mutable_point_cloud() = bytes_to_string(point_cloud.pc);

    return ::grpc::Status();
}

::grpc::Status CameraServer::GetGeometries(::grpc::ServerContext* context,
                                           const ::viam::common::v1::GetGeometriesRequest* request,
                                           ::viam::common::v1::GetGeometriesResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [GetGeometries] without a request");
    };

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    AttributeMap extra;
    if (request->has_extra()) {
        extra = struct_to_map(request->extra());
    }

    const std::shared_ptr<Camera> camera = std::dynamic_pointer_cast<Camera>(rb);
    const std::vector<GeometryConfig> geometries = camera->get_geometries(extra);
    for (const auto& geometry : geometries) {
        *response->mutable_geometries()->Add() = geometry.to_proto();
    }

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

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<Camera> camera = std::dynamic_pointer_cast<Camera>(rb);

    const Camera::properties properties = camera->get_properties();

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
