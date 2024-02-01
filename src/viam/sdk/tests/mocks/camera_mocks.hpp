#pragma once

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/camera/v1/camera.grpc.pb.h>
#include <viam/api/component/camera/v1/camera.pb.h>

#include <viam/sdk/components/camera/camera.hpp>
#include <viam/sdk/components/camera/client.hpp>
#include <viam/sdk/components/camera/server.hpp>

namespace viam {
namespace sdktests {
namespace camera {

using namespace viam::sdk;

class MockCamera : public Camera {
   public:
    AttributeMap do_command(AttributeMap command) override;
    raw_image get_image(std::string mime_type, const sdk::AttributeMap& extra) override;
    image_collection get_images() override;
    point_cloud get_point_cloud(std::string mime_type, const sdk::AttributeMap& extra) override;
    std::vector<GeometryConfig> get_geometries(const sdk::AttributeMap& extra) override;
    properties get_properties() override;
    static std::shared_ptr<MockCamera> get_mock_camera();
    MockCamera(std::string name) : Camera(std::move(name)){};

   private:
    Camera::intrinsic_parameters intrinsic_parameters_;
    Camera::distortion_parameters distortion_parameters_;
    Camera::properties camera_properties_;
    Camera::raw_image image_;
    Camera::image_collection images_;
    Camera::point_cloud pc_;
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> map_;
    std::vector<GeometryConfig> geometries_;
};

Camera::raw_image fake_raw_image();
Camera::image_collection fake_raw_images();
Camera::point_cloud fake_point_cloud();
Camera::intrinsic_parameters fake_intrinsic_parameters();
Camera::distortion_parameters fake_distortion_parameters();
Camera::properties fake_properties();

}  // namespace camera
}  // namespace sdktests
}  // namespace viam
