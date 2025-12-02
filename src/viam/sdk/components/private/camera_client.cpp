#include <viam/sdk/components/private/camera_client.hpp>

#include <algorithm>
#include <memory>
#include <string>
#include <utility>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/camera/v1/camera.grpc.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/camera.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {
namespace impl {

using sdk::from_proto;
using sdk::to_proto;

std::string format_to_MIME_string(viam::component::camera::v1::Format format) {
    switch (format) {
        case viam::component::camera::v1::FORMAT_RAW_RGBA:
            return "image/vnd.viam.rgba";
        case viam::component::camera::v1::FORMAT_RAW_DEPTH:
            return "image/vnd.viam.dep";
        case viam::component::camera::v1::FORMAT_JPEG:
            return "image/jpeg";
        case viam::component::camera::v1::FORMAT_PNG:
            return "image/png";
        default:
            return "";
    }
}

Camera::raw_image from_proto(const viam::component::camera::v1::GetImageResponse& proto) {
    Camera::raw_image raw_image;
    std::string img_string = proto.image();
    const std::vector<unsigned char> bytes(img_string.begin(), img_string.end());
    raw_image.bytes = bytes;
    raw_image.mime_type = proto.mime_type();
    raw_image.source_name = "";
    raw_image.annotations = from_proto(proto.annotations());
    return raw_image;
}

Camera::image_collection from_proto(const viam::component::camera::v1::GetImagesResponse& proto) {
    Camera::image_collection image_collection;
    std::vector<Camera::raw_image> images;
    for (const auto& img : proto.images()) {
        Camera::raw_image raw_image;
        std::string img_string = img.image();
        const std::vector<unsigned char> bytes(img_string.begin(), img_string.end());
        raw_image.bytes = bytes;
        // TODO(RSDK-11733): This is a temporary fix to support handling both the format and mime
        // type. We will remove this once we remove the format field from the proto.
        if (!img.mime_type().empty()) {
            raw_image.mime_type = img.mime_type();
        } else {
            raw_image.mime_type = format_to_MIME_string(img.format());
        }
        raw_image.source_name = img.source_name();
        raw_image.annotations = from_proto(img.annotations());
        images.push_back(raw_image);
    }
    image_collection.images = std::move(images);
    image_collection.metadata = from_proto(proto.response_metadata());
    return image_collection;
}

Camera::point_cloud from_proto(const viam::component::camera::v1::GetPointCloudResponse& proto) {
    Camera::point_cloud point_cloud;
    std::string pc_string = proto.point_cloud();
    const std::vector<unsigned char> bytes(pc_string.begin(), pc_string.end());
    point_cloud.pc = bytes;
    point_cloud.mime_type = proto.mime_type();
    return point_cloud;
}

Camera::intrinsic_parameters from_proto(
    const viam::component::camera::v1::IntrinsicParameters& proto) {
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

Camera::distortion_parameters from_proto(
    const viam::component::camera::v1::DistortionParameters& proto) {
    Camera::distortion_parameters params;
    params.model = proto.model();
    params.parameters = {proto.parameters().begin(), proto.parameters().end()};
    return params;
}

Camera::properties from_proto(const viam::component::camera::v1::GetPropertiesResponse& proto) {
    return {proto.supports_pcd(),
            from_proto(proto.intrinsic_parameters()),
            from_proto(proto.distortion_parameters()),
            {proto.mime_types().begin(), proto.mime_types().end()},
            (proto.frame_rate())};
}

CameraClient::CameraClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : Camera(std::move(name)),
      stub_(viam::component::camera::v1::CameraService::NewStub(channel)),
      channel_(std::move(channel)) {}

ProtoStruct CameraClient::do_command(const ProtoStruct& command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = to_proto(command); })
        .invoke([](auto& response) { return from_proto(response.result()); });
};

Camera::raw_image CameraClient::get_image(std::string mime_type, const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetImage)
        .with(extra,
              [&](auto& request) {
                  *request.mutable_mime_type() = Camera::normalize_mime_type(mime_type);
              })
        .invoke([](auto& response) { return from_proto(response); });
};

Camera::image_collection CameraClient::get_images(std::vector<std::string> filter_source_names,
                                                  const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetImages)
        .with(extra,
              [&](auto& request) {
                  if (!filter_source_names.empty()) {
                      // in newer gRPC versions we would be able to call `Add` or `Assign` on an
                      // iterator range rather than element-wise copy
                      request.mutable_filter_source_names()->Reserve(filter_source_names.size());
                      for (auto& source_name : filter_source_names) {
                          request.add_filter_source_names(std::move(source_name));
                      }
                  }
              })
        .invoke([](auto& response) { return from_proto(response); });
};

Camera::point_cloud CameraClient::get_point_cloud(std::string mime_type, const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetPointCloud)
        .with(extra, [&](auto& request) { *request.mutable_mime_type() = mime_type; })
        .invoke([](auto& response) { return from_proto(response); });
};

std::vector<GeometryConfig> CameraClient::get_geometries(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetGeometries)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response); });
};

Camera::properties CameraClient::get_properties() {
    return make_client_helper(this, *stub_, &StubType::GetProperties).invoke([](auto& response) {
        return from_proto(response);
    });
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
