/// @file components/camera/server.hpp
///
/// @brief Implements a gRPC server for the `Camera` component.
#pragma once

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/camera/v1/camera.grpc.pb.h>

#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/resource/resource_server_base.hpp>

namespace viam {
namespace sdk {

/// @class CameraServer
/// @brief gRPC server implementation of a `Camera` component.
/// @ingroup Camera
class CameraServer : public ResourceServer,
                     public viam::component::camera::v1::CameraService::Service {
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
    ::grpc::Status GetPointCloud(
        ::grpc::ServerContext* context,
        const ::viam::component::camera::v1::GetPointCloudRequest* request,
        ::viam::component::camera::v1::GetPointCloudResponse* response) override;
    ::grpc::Status GetProperties(
        ::grpc::ServerContext* context,
        const ::viam::component::camera::v1::GetPropertiesRequest* request,
        ::viam::component::camera::v1::GetPropertiesResponse* response) override;

    void register_server(std::shared_ptr<Server> server) override;

    CameraServer() : ResourceServer(std::make_shared<ResourceManager>()){};
    CameraServer(std::shared_ptr<ResourceManager> manager) : ResourceServer(manager){};
};

}  // namespace sdk
}  // namespace viam
