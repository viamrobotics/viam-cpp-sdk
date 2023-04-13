/// @file components/camera/client.hpp
///
/// @brief Implements a gRPC client for the `Camera` component.
#pragma once

#include <grpcpp/channel.h>

#include <component/camera/v1/camera.grpc.pb.h>

#include <components/camera/camera.hpp>
#include <components/camera/server.hpp>
#include <config/resource.hpp>
#include <robot/client.hpp>

/// @class CameraClient
/// @brief gRPC client implementation of a `Camera` component.
/// @ingroup Camera
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
    // This constructor leaves the `channel_` as a nullptr. This is useful for testing
    // purposes, but renders it unusable for production use. Care should be taken to
    // avoid use of this constructor outside of tests.
    CameraClient(std::string name,
                 std::unique_ptr<viam::component::camera::v1::CameraService::StubInterface> stub)
        : Camera(std::move(name)), stub_(std::move(stub)){};

   private:
    std::unique_ptr<viam::component::camera::v1::CameraService::StubInterface> stub_;
    std::shared_ptr<grpc::Channel> channel_;
};
