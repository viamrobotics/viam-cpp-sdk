#pragma once

#include <component/camera/v1/camera.grpc.pb.h>
#include <common/v1/common.pb.h>
#include <gen/google/api/http.pb.h>
#include <resource/resource_server_base.hpp>
#include <subtype/subtype.hpp>


class CameraServer : public ResourceServerBase,
                    public viam::component::camera::v1::CameraService::Service{
   public:
    ::grpc::Status DoCommand(::grpc::ServerContext* context,
                             const ::viam::common::v1::DoCommandRequest* request,
                             ::viam::common::v1::DoCommandResponse* response) override;
    ::grpc::Status GetImage(::grpc::ServerContext* context,
                             const ::viam::component::camera::v1::GetImageRequest* request,
                             ::viam::component::camera::v1::GetImageResponse* response) override;
    ::grpc::Status RenderFrame(::grpc::ServerContext* context,
                             const ::viam::component::camera::v1::RenderFrameRequest* request,
                             ::google::api::HttpBody* response) override;                     
    ::grpc::Status GetPointCloud(::grpc::ServerContext* context,
                             const ::viam::component::camera::v1::GetPointCloudRequest* request,
                             ::viam::component::camera::v1::GetPointCloudResponse* response) override; 
    ::grpc::Status GetProperties(::grpc::ServerContext* context,
                             const ::viam::component::camera::v1::GetPropertiesRequest* request,
                             ::viam::component::camera::v1::GetPropertiesResponse* response) override;

    void register_server() override;

    CameraServer(){};
    CameraServer(std::shared_ptr<SubtypeService> sub_svc) : sub_svc(sub_svc){};
    std::shared_ptr<SubtypeService> sub_svc;


   private:
    static bool initialized;
};

  std::string bytes_to_string(std::vector<unsigned char> const &b);

