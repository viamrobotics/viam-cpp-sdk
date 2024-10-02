#include <viam/sdk/components/private/camera_client.hpp>

#include <algorithm>
#include <memory>
#include <string>
#include <utility>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/camera/v1/camera.grpc.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/camera.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {
namespace impl {

CameraClient::CameraClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : Camera(std::move(name)),
      stub_(viam::component::camera::v1::CameraService::NewStub(channel)),
      channel_(std::move(channel)){};

ProtoStruct CameraClient::do_command(const ProtoStruct& command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = map_to_struct(command); })
        .invoke([](auto& response) { return struct_to_map(response.result()); });
};

Camera::raw_image CameraClient::get_image(std::string mime_type, const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetImage)
        .with(extra,
              [&](auto& request) {
                  *request.mutable_mime_type() = Camera::normalize_mime_type(mime_type);
              })
        .invoke([](auto& response) { return from_proto(response); });
};

Camera::image_collection CameraClient::get_images() {
    return make_client_helper(this, *stub_, &StubType::GetImages).invoke([](auto& response) {
        return from_proto(response);
    });
};

Camera::point_cloud CameraClient::get_point_cloud(std::string mime_type, const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetPointCloud)
        .with(extra, [&](auto& request) { *request.mutable_mime_type() = mime_type; })
        .invoke([](auto& response) { return from_proto(response); });
};

std::vector<GeometryConfig> CameraClient::get_geometries(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetGeometries)
        .with(extra)
        .invoke([](auto& response) { return GeometryConfig::from_proto(response); });
};

Camera::properties CameraClient::get_properties() {
    return make_client_helper(this, *stub_, &StubType::GetProperties).invoke([](auto& response) {
        return from_proto(response);
    });
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
