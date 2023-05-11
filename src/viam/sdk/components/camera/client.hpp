/// @file components/camera/client.hpp
///
/// @brief Implements a gRPC client for the `Camera` component.
#pragma once

#include <grpcpp/channel.h>

#include <viam/api/component/camera/v1/camera.grpc.pb.h>

#include <viam/sdk/components/camera/camera.hpp>
#include <viam/sdk/components/camera/server.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {

/// @class CameraClient
/// @brief gRPC client implementation of a `Camera` component.
/// @ingroup Camera
class CameraClient : public Camera {
   public:
    CameraClient(std::string name, std::shared_ptr<grpc::Channel> channel);
    AttributeMap do_command(AttributeMap command) override;
    raw_image get_image(std::string mime_type) override;
    point_cloud get_point_cloud(std::string mime_type) override;
    properties get_properties() override;

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

}  // namespace sdk
}  // namespace viam
