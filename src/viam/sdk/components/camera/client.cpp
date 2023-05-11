#include <viam/sdk/components/camera/client.hpp>

#include <algorithm>
#include <memory>
#include <string>
#include <utility>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/camera/v1/camera.grpc.pb.h>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/camera/camera.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {

CameraClient::CameraClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : Camera(std::move(name)),
      stub_(viam::component::camera::v1::CameraService::NewStub(channel)),
      channel_(std::move(channel)){};

std::string normalize_mime_type(const std::string& str) {
    std::string mime_type = str;
    if (str.size() >= Camera::lazy_suffix.size() &&
        str.compare(str.size() - Camera::lazy_suffix.size(),
                    Camera::lazy_suffix.size(),
                    Camera::lazy_suffix) == 0) {
        mime_type = mime_type.substr(0, mime_type.length() - Camera::lazy_suffix.length());
    }
    return mime_type;
}

AttributeMap CameraClient::do_command(AttributeMap command) {
    viam::common::v1::DoCommandRequest req;
    viam::common::v1::DoCommandResponse resp;
    grpc::ClientContext ctx;

    google::protobuf::Struct proto_command = map_to_struct(command);
    *req.mutable_command() = proto_command;
    *req.mutable_name() = this->name();
    stub_->DoCommand(&ctx, req, &resp);
    return struct_to_map(resp.result());
};

Camera::raw_image CameraClient::get_image(std::string mime_type) {
    viam::component::camera::v1::GetImageRequest req;
    viam::component::camera::v1::GetImageResponse resp;
    grpc::ClientContext ctx;

    normalize_mime_type(mime_type);

    *req.mutable_mime_type() = mime_type;
    *req.mutable_name() = this->name();

    stub_->GetImage(&ctx, req, &resp);
    return from_proto(resp);
};

Camera::point_cloud CameraClient::get_point_cloud(std::string mime_type) {
    viam::component::camera::v1::GetPointCloudRequest req;
    viam::component::camera::v1::GetPointCloudResponse resp;
    grpc::ClientContext ctx;

    *req.mutable_name() = this->name();
    *req.mutable_mime_type() = mime_type;

    stub_->GetPointCloud(&ctx, req, &resp);
    return from_proto(resp);
};

Camera::properties CameraClient::get_properties() {
    viam::component::camera::v1::GetPropertiesRequest req;
    viam::component::camera::v1::GetPropertiesResponse resp;
    grpc::ClientContext ctx;

    *req.mutable_name() = this->name();

    stub_->GetProperties(&ctx, req, &resp);
    return from_proto(resp);
};

}  // namespace sdk
}  // namespace viam
