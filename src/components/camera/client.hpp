#pragma once

#include <grpcpp/channel.h>

#include <component/camera/v1/camera.grpc.pb.h>

#include <components/camera/camera.hpp>
#include <components/camera/server.hpp>
#include <config/resource.hpp>
#include <robot/client.hpp>

class CameraClient : public Camera {
   public:
    AttributeMap do_command(AttributeMap command) override;
    raw_image get_image(std::string mime_type) override;
    point_cloud get_point_cloud(std::string mime_type) override;
    properties get_properties() override;
    CameraClient(std::string name, std::shared_ptr<grpc::Channel> channel)
        : Camera(std::move(name)),
          stub_(viam::component::camera::v1::CameraService::NewStub(channel)),
          channel_(std::move(channel)){};

   protected:
    CameraClient(std::string name,
                 std::unique_ptr<viam::component::camera::v1::CameraService::StubInterface> stub)
        : Camera(std::move(name)), stub_(std::move(stub)){};

   private:
    std::unique_ptr<viam::component::camera::v1::CameraService::StubInterface> stub_;
    std::shared_ptr<grpc::Channel> channel_;
};
