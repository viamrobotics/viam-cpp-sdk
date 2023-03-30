#include <components/camera/camera.hpp>

#include <google/protobuf/descriptor.h>

#include <component/camera/v1/camera.grpc.pb.h>
#include <component/camera/v1/camera.pb.h>

#include <common/utils.hpp>
#include <components/camera/client.hpp>
#include <components/camera/server.hpp>
#include <registry/registry.hpp>
#include <resource/resource.hpp>

std::shared_ptr<ResourceServerBase> CameraSubtype::create_resource_server(
    std::shared_ptr<SubtypeService> svc) {
    return std::make_shared<CameraServer>(svc);
};

std::shared_ptr<ResourceBase> CameraSubtype::create_rpc_client(
    std::string name, std::shared_ptr<grpc::Channel> chan) {
    auto camera_client = std::make_shared<CameraClient>(name, chan);
    return camera_client;
};

std::shared_ptr<ResourceSubtype> Camera::resource_subtype() {
    const google::protobuf::DescriptorPool* p = google::protobuf::DescriptorPool::generated_pool();
    const google::protobuf::ServiceDescriptor* sd =
        p->FindServiceByName(viam::component::camera::v1::CameraService::service_full_name());
    if (sd == nullptr) {
        throw std::runtime_error("Unable to get service descriptor for the camera service");
    }
    return std::make_shared<CameraSubtype>(sd);
}

Subtype Camera::subtype() {
    return Subtype(RDK, COMPONENT, "camera");
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
    params.width_px = proto.width_px();
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

    viam::component::camera::v1::DistortionParameters distortion_parameters_proto =
        proto.distortion_parameters();
    distortion_parameters = from_proto(distortion_parameters_proto);

    viam::component::camera::v1::IntrinsicParameters intrinsic_parameters_proto =
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
    Registry::register_subtype(Camera::subtype(), Camera::resource_subtype());
    return true;
};

bool inited = init();
}  // namespace
