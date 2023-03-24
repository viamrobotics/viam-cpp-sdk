#pragma once

#include <component/camera/v1/camera.pb.h>

#include <bitset>
#include <common/utils.hpp>
#include <config/resource.hpp>
#include <registry/registry.hpp>
#include <string>
#include <vector>

#include "common/proto_type.hpp"
#include "subtype/subtype.hpp"

class CameraSubtype : public ResourceSubtype {
public:
  std::shared_ptr<ResourceServerBase>
  create_resource_server(std::shared_ptr<SubtypeService> svc) override;
  std::shared_ptr<ResourceBase> create_rpc_client(std::string name, std::shared_ptr<grpc::Channel> chan) override;
  CameraSubtype(const google::protobuf::ServiceDescriptor *service_descriptor)
      : ResourceSubtype(service_descriptor){};
};

class Camera : public ComponentBase {
public:
  struct intrinsic_parameters {
    int width_px;
    int height_px;
    double focal_x_px;
    double focal_y_px;
    double center_x_px;
    double center_y_px;
  };

  struct distortion_parameters {
    std::string model;
    std::vector<double> parameters;
  };

  struct properties {
    bool supports_pcd;
    intrinsic_parameters intrinsic_parameters;
    distortion_parameters distortion_parameters;
  };

  struct point_cloud {
    std::string mime_type;
    std::vector<unsigned char> pc;
  };

  struct raw_image {
    std::string mime_type;
    std::vector<unsigned char> bytes;
  };

  inline const static std::string lazy_suffix = "+lazy";

  static std::shared_ptr<ResourceSubtype> resource_subtype();
  static Subtype subtype();
  static raw_image
  from_proto(viam::component::camera::v1::GetImageResponse proto);
  static point_cloud
  from_proto(viam::component::camera::v1::GetPointCloudResponse proto);
  static intrinsic_parameters
  from_proto(viam::component::camera::v1::IntrinsicParameters proto);
  static distortion_parameters
  from_proto(viam::component::camera::v1::DistortionParameters proto);
  static properties
  from_proto(viam::component::camera::v1::GetPropertiesResponse proto);
  static viam::component::camera::v1::DistortionParameters
      to_proto(distortion_parameters);
  static viam::component::camera::v1::IntrinsicParameters
      to_proto(intrinsic_parameters);

  virtual AttributeMap do_command(AttributeMap command) = 0;
  virtual raw_image get_image(std::string mime_type) = 0;
  virtual point_cloud get_point_cloud(std::string mime_type) = 0;
  virtual properties get_properties() = 0;
};

bool operator==(const Camera::raw_image &lhs, const Camera::raw_image &rhs);
bool operator==(const Camera::point_cloud &lhs, const Camera::point_cloud &rhs);
bool operator==(const Camera::intrinsic_parameters &lhs,
                const Camera::intrinsic_parameters &rhs);
bool operator==(const Camera::distortion_parameters &lhs,
                const Camera::distortion_parameters &rhs);
bool operator==(const Camera::properties &lhs, const Camera::properties &rhs);
