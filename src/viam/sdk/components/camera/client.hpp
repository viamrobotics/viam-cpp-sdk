/// @file components/camera/client.hpp
///
/// @brief Implements a gRPC client for the `Camera` component.
#pragma once

#include <grpcpp/channel.h>

#include <viam/api/common/v1/common.pb.h>
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
    raw_image get_image(std::string mime_type, const AttributeMap& extra) override;
    image_collection get_images() override;
    point_cloud get_point_cloud(std::string mime_type, const AttributeMap& extra) override;
    properties get_properties() override;
    std::vector<GeometryConfig> get_geometries(const AttributeMap& extra) override;

    // the `extra` param is frequently unnecessary but needs to be supported. Ideally, we'd
    // like to live in a world where implementers of derived classes don't need to go out of
    // their way to support two versions of a method (an `extra` version and a non-`extra`
    // version), and users don't need to pass an unnecessary parameters to all method calls.
    //
    // To do this, we define in the parent resource class a non-virtual version of the methods
    // that calls the virtual method and passes a `nullptr` by default in place of the `extra`
    // param. In order to access these versions of the methods within the client code, however,
    // we need to include these `using` lines.
    using Camera::get_geometries;
    using Camera::get_image;
    using Camera::get_point_cloud;

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
