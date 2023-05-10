#include <viam/sdk/components/camera/camera.hpp>

#include <google/protobuf/descriptor.h>

#include <viam/api/component/camera/v1/camera.grpc.pb.h>
#include <viam/api/component/camera/v1/camera.pb.h>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/camera/client.hpp>
#include <viam/sdk/components/camera/server.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

std::shared_ptr<ResourceServer> CameraRegistration::create_resource_server(
    std::shared_ptr<ResourceManager> manager) {
    return std::make_shared<CameraServer>(manager);
};

std::shared_ptr<Resource> CameraRegistration::create_rpc_client(
    std::string name, std::shared_ptr<grpc::Channel> chan) {
    return std::make_shared<CameraClient>(std::move(name), std::move(chan));
};

std::shared_ptr<ResourceRegistration> Camera::resource_registration() {
    const google::protobuf::DescriptorPool* p = google::protobuf::DescriptorPool::generated_pool();
    const google::protobuf::ServiceDescriptor* sd =
        p->FindServiceByName(viam::component::camera::v1::CameraService::service_full_name());
    if (!sd) {
        throw std::runtime_error("Unable to get service descriptor for the camera service");
    }
    return std::make_shared<CameraRegistration>(sd);
}

// NOLINTNEXTLINE
const std::string Camera::lazy_suffix = "+lazy";

API Camera::static_api() {
    return API(RDK, COMPONENT, "camera");
}

API Camera::dynamic_api() const {
    return static_api();
}

std::vector<double> repeated_field_to_vector(google::protobuf::RepeatedField<double> const& f) {
    std::vector<double> v(f.begin(), f.end());
    return v;
}

google::protobuf::RepeatedField<double> vector_to_repeated_field(std::vector<double> const& v) {
    google::protobuf::RepeatedField<double> rf = {v.begin(), v.end()};
    return rf;
}

Camera::raw_image Camera::from_proto(viam::component::camera::v1::GetImageResponse proto) {
    Camera::raw_image raw_image;
    std::string img_string = proto.image();
    std::vector<unsigned char> bytes(img_string.begin(), img_string.end());
    raw_image.bytes = bytes;
    raw_image.mime_type = proto.mime_type();
    return raw_image;
}

Camera::point_cloud Camera::from_proto(viam::component::camera::v1::GetPointCloudResponse proto) {
    Camera::point_cloud point_cloud;
    std::string pc_string = proto.point_cloud();
    std::vector<unsigned char> bytes(pc_string.begin(), pc_string.end());
    point_cloud.pc = bytes;
    point_cloud.mime_type = proto.mime_type();
    return point_cloud;
}

Camera::intrinsic_parameters Camera::from_proto(
    viam::component::camera::v1::IntrinsicParameters proto) {
    Camera::intrinsic_parameters params;
    // NOLINTNEXTLINE(bugprone-narrowing-conversions)
    params.width_px = proto.width_px();
    // NOLINTNEXTLINE(bugprone-narrowing-conversions)
    params.height_px = proto.height_px();
    params.focal_x_px = proto.focal_x_px();
    params.focal_y_px = proto.focal_y_px();
    params.center_x_px = proto.center_x_px();
    params.center_y_px = proto.center_y_px();
    return params;
}

Camera::distortion_parameters Camera::from_proto(
    viam::component::camera::v1::DistortionParameters proto) {
    Camera::distortion_parameters params;
    params.model = proto.model();
    params.parameters = repeated_field_to_vector(proto.parameters());
    return params;
}

Camera::properties Camera::from_proto(viam::component::camera::v1::GetPropertiesResponse proto) {
    Camera::distortion_parameters distortion_parameters;
    Camera::intrinsic_parameters intrinsic_parameters;
    Camera::properties properties;

    const viam::component::camera::v1::DistortionParameters& distortion_parameters_proto =
        proto.distortion_parameters();
    distortion_parameters = from_proto(distortion_parameters_proto);

    const viam::component::camera::v1::IntrinsicParameters& intrinsic_parameters_proto =
        proto.intrinsic_parameters();
    intrinsic_parameters = from_proto(intrinsic_parameters_proto);

    properties.distortion_parameters = distortion_parameters;
    properties.intrinsic_parameters = intrinsic_parameters;
    properties.supports_pcd = proto.supports_pcd();

    return properties;
}

viam::component::camera::v1::IntrinsicParameters Camera::to_proto(
    Camera::intrinsic_parameters params) {
    viam::component::camera::v1::IntrinsicParameters proto;
    proto.set_width_px(params.width_px);
    proto.set_height_px(params.height_px);
    proto.set_focal_x_px(params.focal_x_px);
    proto.set_focal_y_px(params.focal_y_px);
    proto.set_center_x_px(params.center_x_px);
    proto.set_center_y_px(params.center_y_px);
    return proto;
}
viam::component::camera::v1::DistortionParameters Camera::to_proto(
    Camera::distortion_parameters params) {
    viam::component::camera::v1::DistortionParameters proto;
    *proto.mutable_model() = params.model;
    *proto.mutable_parameters() = vector_to_repeated_field(params.parameters);
    return proto;
}

bool operator==(const Camera::point_cloud& lhs, const Camera::point_cloud& rhs) {
    return lhs.mime_type == rhs.mime_type && lhs.pc == rhs.pc;
}

bool operator==(const Camera::raw_image& lhs, const Camera::raw_image& rhs) {
    return lhs.mime_type == rhs.mime_type && lhs.bytes == rhs.bytes;
}

bool operator==(const Camera::intrinsic_parameters& lhs, const Camera::intrinsic_parameters& rhs) {
    return lhs.width_px == rhs.width_px && lhs.height_px == rhs.height_px &&
           lhs.focal_x_px == rhs.focal_x_px && lhs.focal_y_px == rhs.focal_y_px &&
           lhs.center_x_px == rhs.center_x_px && lhs.center_y_px == rhs.center_y_px;
}
bool operator==(const Camera::distortion_parameters& lhs,
                const Camera::distortion_parameters& rhs) {
    return lhs.model == rhs.model && lhs.parameters == rhs.parameters;
}

bool operator==(const Camera::properties& lhs, const Camera::properties& rhs) {
    return lhs.supports_pcd == rhs.supports_pcd &&
           lhs.intrinsic_parameters == rhs.intrinsic_parameters &&
           lhs.distortion_parameters == rhs.distortion_parameters;
}

namespace {
bool init() {
    Registry::register_resource(Camera::static_api(), Camera::resource_registration());
    return true;
};

// NOLINTNEXTLINE
const bool inited = init();
}  // namespace

}  // namespace sdk
}  // namespace viam
