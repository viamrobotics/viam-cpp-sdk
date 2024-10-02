#include <viam/sdk/components/private/camera_server.hpp>

#include <google/protobuf/util/time_util.h>
#include <grpcpp/support/status.h>

#include <viam/sdk/common/service_helper.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/camera.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/rpc/server.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>

namespace viam {
namespace sdk {
namespace impl {

CameraServer::CameraServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(std::move(manager)) {};

::grpc::Status CameraServer::DoCommand(::grpc::ServerContext*,
                                       const ::viam::common::v1::DoCommandRequest* request,
                                       ::viam::common::v1::DoCommandResponse* response) noexcept {
    return make_service_helper<Camera>(
        "CameraServer::DoCommand", this, request)([&](auto&, auto& camera) {
        const ProtoStruct result = camera->do_command(struct_to_map(request->command()));
        *response->mutable_result() = map_to_struct(result);
    });
}

::grpc::Status CameraServer::GetImage(
    ::grpc::ServerContext*,
    const ::viam::component::camera::v1::GetImageRequest* request,
    ::viam::component::camera::v1::GetImageResponse* response) noexcept {
    return make_service_helper<Camera>(
        "CameraServer::GetImage", this, request)([&](auto& helper, auto& camera) {
        const Camera::raw_image image = camera->get_image(request->mime_type(), helper.getExtra());

        const std::string img_string = bytes_to_string(image.bytes);

        *response->mutable_mime_type() = image.mime_type;
        *response->mutable_image() = img_string;
    });
}

::grpc::Status CameraServer::GetImages(
    ::grpc::ServerContext*,
    const ::viam::component::camera::v1::GetImagesRequest* request,
    ::viam::component::camera::v1::GetImagesResponse* response) noexcept {
    return make_service_helper<Camera>(
        "CameraServer::GetImages", this, request)([&](auto&, auto& camera) {
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
    });
}

::grpc::Status CameraServer::RenderFrame(
    ::grpc::ServerContext*,
    const ::viam::component::camera::v1::RenderFrameRequest* request,
    ::google::api::HttpBody* response) noexcept {
    return make_service_helper<Camera>(
        "CameraServer::RenderFrame", this, request)([&](auto& helper, auto& camera) {
        const Camera::raw_image image = camera->get_image(request->mime_type(), helper.getExtra());

        response->set_data(bytes_to_string(image.bytes));
        response->set_content_type(image.mime_type);
    });
}

::grpc::Status CameraServer::GetPointCloud(
    ::grpc::ServerContext*,
    const ::viam::component::camera::v1::GetPointCloudRequest* request,
    ::viam::component::camera::v1::GetPointCloudResponse* response) noexcept {
    return make_service_helper<Camera>(
        "CameraServer::GetPointCloud", this, request)([&](auto& helper, auto& camera) {
        const Camera::point_cloud point_cloud =
            camera->get_point_cloud(request->mime_type(), helper.getExtra());
        *response->mutable_mime_type() = "pointcloud/pcd";
        *response->mutable_point_cloud() = bytes_to_string(point_cloud.pc);
    });
}

::grpc::Status CameraServer::GetGeometries(
    ::grpc::ServerContext*,
    const ::viam::common::v1::GetGeometriesRequest* request,
    ::viam::common::v1::GetGeometriesResponse* response) noexcept {
    return make_service_helper<Camera>(
        "CameraServer::GetGeometries", this, request)([&](auto& helper, auto& camera) {
        const std::vector<GeometryConfig> geometries = camera->get_geometries(helper.getExtra());
        for (const auto& geometry : geometries) {
            *response->mutable_geometries()->Add() = geometry.to_proto();
        }
    });
}

::grpc::Status CameraServer::GetProperties(
    ::grpc::ServerContext*,
    const ::viam::component::camera::v1::GetPropertiesRequest* request,
    ::viam::component::camera::v1::GetPropertiesResponse* response) noexcept {
    return make_service_helper<Camera>(
        "CameraServer::GetProperties", this, request)([&](auto&, auto& camera) {
        const Camera::properties properties = camera->get_properties();

        *response->mutable_distortion_parameters() =
            Camera::to_proto(properties.distortion_parameters);
        *response->mutable_intrinsic_parameters() =
            Camera::to_proto(properties.intrinsic_parameters);
        response->set_supports_pcd(properties.supports_pcd);
        response->set_frame_rate(properties.frame_rate);
    });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
