#pragma once

#include <bitset>
#include <string>
#include <vector>

#include <component/base/v1/base.pb.h>

#include <common/proto_type.hpp>
#include <common/utils.hpp>
#include <config/resource.hpp>
#include <registry/registry.hpp>
#include <subtype/subtype.hpp>
#include <viam/common/v1/common.pb.h>

class BaseSubtype : public ResourceSubtype {
   public:
    std::shared_ptr<ResourceServerBase> create_resource_server(
        std::shared_ptr<SubtypeService> svc) override;
    std::shared_ptr<ResourceBase> create_rpc_client(std::string name,
                                                    std::shared_ptr<grpc::Channel> chan) override;
    BaseSubtype(const google::protobuf::ServiceDescriptor* service_descriptor)
        : ResourceSubtype(service_descriptor){};
};

class Base : public ComponentBase {
   public:
    static std::shared_ptr<ResourceSubtype> resource_subtype();
    static Subtype subtype();

    virtual void move_straight(int64_t distance_mm, double mm_per_sec) = 0;
    virtual void spin(double angle_deg, double degs_per_sec) = 0;
    virtual void stop() = 0;
    virtual void set_power(const viam::common::v1::Vector3& linear,
                           const viam::common::v1::Vector3& angular, ) = 0;
    virtual void set_velocity(const viam::common::v1::Vector3& linear,
                              const viam::common::v1::Vector3& angular, ) = 0;
    virtual bool is_moving() = 0;

    virtual AttributeMap do_command(AttributeMap command) = 0;

   protected:
    explicit Base(std::string name) : ComponentBase(std::move(name)){};
};
