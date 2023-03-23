#include <algorithm>
#include <common/utils.hpp>
#include <components/camera/camera.hpp>
#include <components/camera/client.hpp>
#include <config/resource.hpp>
#include <string>
#include <utility>

#include "common/v1/common.pb.h"
#include "component/camera/v1/camera.grpc.pb.h"

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
    *req.mutable_name() = this->name_;
    stub_->DoCommand(&ctx, req, &resp);
    return struct_to_map(resp.result());
};

Camera::raw_image CameraClient::get_image(std::string mime_type) {
    viam::component::camera::v1::GetImageRequest req;
    viam::component::camera::v1::GetImageResponse resp;
    grpc::ClientContext ctx;

    normalize_mime_type(mime_type);

    *req.mutable_mime_type() = mime_type;
    *req.mutable_name() = this->name_;

    stub_->GetImage(&ctx, req, &resp);
    return from_proto(resp);
};

Camera::point_cloud CameraClient::get_point_cloud(std::string mime_type) {
    viam::component::camera::v1::GetPointCloudRequest req;
    viam::component::camera::v1::GetPointCloudResponse resp;
    grpc::ClientContext ctx;

    *req.mutable_name() = this->name_;
    *req.mutable_mime_type() = mime_type;

    stub_->GetPointCloud(&ctx, req, &resp);
    return from_proto(resp);
};

Camera::properties CameraClient::get_properties() {
    viam::component::camera::v1::GetPropertiesRequest req;
    viam::component::camera::v1::GetPropertiesResponse resp;
    grpc::ClientContext ctx;

    *req.mutable_name() = this->name_;

    stub_->GetProperties(&ctx, req, &resp);
    return from_proto(resp);
};

CameraClient::CameraClient(std::string name, std::shared_ptr<grpc::Channel> channel_)
    : channel_(channel_), stub_(viam::component::camera::v1::CameraService::NewStub(channel_)){};

CameraClient::CameraClient(std::string name) : name_(name), channel_(nullptr), stub_(nullptr){};
