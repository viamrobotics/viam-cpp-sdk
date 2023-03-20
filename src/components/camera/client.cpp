#include <algorithm>
#include <utility>
#include <string>
#include <components/camera/client.hpp>
#include <components/camera/camera.hpp>

#include "common/v1/common.pb.h"
#include "component/camera/v1/camera.grpc.pb.h"




std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> 
CameraClient::do_command(std::string name, std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> command) {
    viam::common::v1::DoCommandRequest req;
    viam::common::v1::DoCommandResponse resp;
    grpc::ClientContext ctx;

    google::protobuf::Struct proto_command = map_to_struct(command);
    *req.mutable_command() = proto_command;
    *req.mutable_name() = name;
    stub_->DoCommand(&ctx, req, &resp);
    return struct_to_map(resp.result());
}; 


Camera::raw_image CameraClient::get_image(std::string name, std::string mime_type) {
    viam::component::camera::v1::GetImageRequest req;
    viam::component::camera::v1::GetImageResponse resp;
    grpc::ClientContext ctx;

   if(has_suffix(mime_type, Camera::lazy_suffix)) {
        mime_type = mime_type.substr(0, mime_type.length() - Camera::lazy_suffix.length());
    } 

   *req.mutable_mime_type() = mime_type;
   *req.mutable_name() = name;

    stub_->GetImage(&ctx, req, &resp); 

    Camera::raw_image raw_image;

    const std::string img_string = resp.image();

    raw_image.bytes = string_to_bytes(img_string);
    raw_image.mime_type = resp.mime_type();

    return raw_image;

};

Camera::point_cloud CameraClient::get_point_cloud(std::string name, std:: string mime_type) {

    viam::component::camera::v1::GetPointCloudRequest req;
    viam::component::camera::v1::GetPointCloudResponse resp;
    grpc::ClientContext ctx;

    *req.mutable_name() = name;
    *req.mutable_mime_type() = mime_type; 

    stub_->GetPointCloud(&ctx, req, &resp);

    Camera::point_cloud point_cloud;

    std::string pc_string = resp.point_cloud();
    point_cloud.pc = string_to_bytes(pc_string);
    point_cloud.mime_type = resp.mime_type();

    return point_cloud;
    
};

Camera::properties CameraClient::get_properties(std::string name) {

    viam::component::camera::v1::GetPropertiesRequest req;
    viam::component::camera::v1::GetPropertiesResponse resp;
    grpc::ClientContext ctx;

    *req.mutable_name() = name;

    stub_->GetProperties(&ctx, req, &resp);

    Camera::distortion_parameters distortion_parameters;
    Camera::intrinsic_parameters intrinsic_parameters;
    Camera::properties properties;


    viam::component::camera::v1::DistortionParameters distortion_parameters_proto = resp.distortion_parameters(); 
    distortion_parameters.from_proto(distortion_parameters_proto);


    viam::component::camera::v1::IntrinsicParameters intrinsic_parameters_proto = resp.intrinsic_parameters();
    intrinsic_parameters.from_proto(intrinsic_parameters_proto);

    properties.distortion_parameters = distortion_parameters;
    properties.intrinsic_parameters = intrinsic_parameters; 
    properties.supports_pcd = resp.supports_pcd();

    return properties;
};


bool has_suffix(const std::string &str, const std::string &suffix) {
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

 std::vector<unsigned char> string_to_bytes(std::string const &s) {
    std::vector<unsigned char> bytes;
    bytes.reserve(s.size());
 
    std::transform(std::begin(s), std::end(s), std::back_inserter(bytes), [](char c){
        return c;
    });
    return bytes; 
}; 



  CameraClient::CameraClient(std::shared_ptr<grpc::Channel> channel_)
    : channel_(channel_), stub_(viam::component::camera::v1::CameraService::NewStub(channel_)){}; 


    CameraClient::CameraClient() 
          : channel_(nullptr), stub_(nullptr){}; 











