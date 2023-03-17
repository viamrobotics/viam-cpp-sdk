#pragma once

#include <common/utils.hpp>
#include <component/camera/v1/camera.pb.h>
#include <registry/registry.hpp>
#include <resource/resource.hpp>
#include <string>
#include <vector>
#include <bitset>

#include "common/proto_type.hpp"
#include "subtype/subtype.hpp"


 class CameraSubtype : public ResourceSubtype {
   public:
    std::shared_ptr<ResourceServerBase> create_resource_server(
        std::shared_ptr<SubtypeService> svc) override;
    CameraSubtype(const google::protobuf::ServiceDescriptor* service_descriptor)
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

        intrinsic_parameters from_proto(viam::component::camera::v1::IntrinsicParameters proto) {
            width_px =  proto.width_px();
            height_px = proto.height_px();
            focal_x_px = proto.focal_x_px();
            focal_y_px = proto.focal_y_px();
            center_x_px = proto.center_x_px();
            center_y_px = proto.center_y_px();
            return *this;
        }

        viam::component::camera::v1::IntrinsicParameters to_proto() {
            viam::component::camera::v1::IntrinsicParameters proto;
             proto.set_width_px(this->width_px);
             proto.set_height_px(this->height_px);
             proto.set_focal_x_px(this->focal_x_px);
             proto.set_focal_y_px(this->focal_y_px);
             proto.set_center_x_px(this->center_x_px);
             proto.set_center_y_px(this->center_y_px);
            return proto;
        } 

        bool operator==(const intrinsic_parameters& rhs) {
            return width_px == rhs.width_px 
            and height_px == rhs.height_px
            and focal_x_px == rhs.focal_x_px
            and focal_y_px == rhs.focal_y_px
            and center_x_px == rhs.center_x_px
            and center_y_px == rhs.center_y_px;
        }
    }; 

    struct distortion_parameters {
        std::string model;
        std::vector<double> parameters;

        std::vector<double> repeatedfield_to_vector(google::protobuf::RepeatedField<double> const & f) {
              std::vector<double> v(f.begin(), f.end());
              return v;
            }

        google::protobuf::RepeatedField<double> vector_to_repeatedfield(std::vector<double> const & v) {
            google::protobuf::RepeatedField<double> rf = {v.begin(), v.end()};
            return rf;
        }

         distortion_parameters from_proto(viam::component::camera::v1::DistortionParameters proto) {
            model = proto.model();
            parameters = repeatedfield_to_vector(proto.parameters());
            return *this;
        }

          viam::component::camera::v1::DistortionParameters to_proto() {
            viam::component::camera::v1::DistortionParameters proto;
             *proto.mutable_model() = this->model;
             *proto.mutable_parameters() =  vector_to_repeatedfield(this->parameters);
            return proto;
        }

        bool operator==(const distortion_parameters& rhs) {
            return model == rhs.model 
            and parameters == rhs.parameters;
        }
    }; 

    struct properties {
        bool supports_pcd;
        intrinsic_parameters intrinsic_parameters;
        distortion_parameters distortion_parameters;

        bool operator==(const properties& rhs) {
            return supports_pcd == rhs.supports_pcd
            and intrinsic_parameters ==  rhs.intrinsic_parameters
            and distortion_parameters == rhs.distortion_parameters;
        }
    };

    struct point_cloud {
        std::string mime_type;
        std::vector<unsigned char> pc;

        bool operator==(const point_cloud& rhs) {
            return mime_type == rhs.mime_type 
            and pc == rhs.pc;

        }
    };

    struct raw_image {
        std::string mime_type;
        std::vector<unsigned char> bytes;

        bool operator==(const raw_image& rhs) {
            return mime_type == rhs.mime_type 
            and bytes == rhs.bytes;
        }
    }; 

    std::string lazy_suffix = "+lazy";

    static std::shared_ptr<ResourceSubtype> resource_subtype();
    static Subtype subtype();
    virtual std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> do_command(std::string name, std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> command) = 0;
    virtual raw_image get_image(std::string name, std::string mime_type) = 0; 
    virtual point_cloud get_point_cloud(std::string name, std::string mime_type) = 0;
    virtual properties get_properties(std::string name) = 0;

};





