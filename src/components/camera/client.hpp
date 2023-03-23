#pragma once

#include <component/camera/v1/camera.grpc.pb.h>
#include <grpcpp/channel.h>

#include <components/camera/camera.hpp>
#include <components/camera/server.hpp>
#include <config/resource.hpp>

class CameraClient : public Camera {
   public:
    AttributeMap do_command(AttributeMap command) override;
    raw_image get_image(std::string mime_type) override;
    point_cloud get_point_cloud(std::string mime_type) override;
    properties get_properties() override;
    CameraClient(std::string name, std::shared_ptr<grpc::Channel> channel_);

   protected:
    CameraClient(std::string name);
    std::unique_ptr<viam::component::camera::v1::CameraService::StubInterface> stub_;
    std::string name_;

   private:
    std::shared_ptr<grpc::Channel> channel_;
};
std::vector<unsigned char> string_to_bytes(std::string const& s);
