#include "viam/api/service/motion/v1/motion.grpc.pb.h"
#include "viam/api/service/motion/v1/motion.pb.h"
#include "viam/sdk/common/utils.hpp"
#include <viam/sdk/services/motion/client.hpp>
#include <viam/sdk/services/motion/motion.hpp>
#include <viam/sdk/services/motion/server.hpp>

#include <common/v1/common.pb.h>

namespace viam {
namespace sdk {

MotionRegistration::MotionRegistration(
    const google::protobuf::ServiceDescriptor* service_descriptor)
    : ResourceRegistration(service_descriptor){};

std::shared_ptr<ResourceServer> MotionRegistration::create_resource_server(
    std::shared_ptr<ResourceManager> manager) {
    return std::make_shared<MotionServer>(manager);
};

std::shared_ptr<Resource> MotionRegistration::create_rpc_client(
    std::string name, std::shared_ptr<grpc::Channel> chan) {
    return std::make_shared<MotionClient>(std::move(name), std::move(chan));
};

Motion::Motion(std::string name) : Service(std::move(name)){};

service::motion::v1::Constraints Motion::constraints::to_proto() const {
    service::motion::v1::Constraints proto;
    for (const auto& lc : linear_constraints) {
        service::motion::v1::LinearConstraint proto_lc;
        proto_lc.set_line_tolerance_mm(lc.line_tolerance_mm);
        proto_lc.set_orientation_tolerance_degs(lc.orientation_tolerance_degs);
        *proto.mutable_linear_constraint()->Add() = std::move(proto_lc);
    }

    for (const auto& oc : orientation_constraints) {
        service::motion::v1::OrientationConstraint proto_oc;
        proto_oc.set_orientation_tolerance_degs(oc.orientation_tolerance_degs);
        *proto.mutable_orientation_constraint()->Add() = std::move(proto_oc);
    }

    for (const auto& cs : collision_specifications) {
        service::motion::v1::CollisionSpecification proto_cs;
        for (const auto& allow : cs.allows) {
            service::motion::v1::CollisionSpecification::AllowedFrameCollisions proto_allow;
            *proto_allow.mutable_frame1() = allow.frame1;
            *proto_allow.mutable_frame2() = allow.frame2;
            *proto_cs.mutable_allows()->Add() = std::move(proto_allow);
        }
        *proto.mutable_collision_specification()->Add() = std::move(proto_cs);
    }

    return proto;
}

Motion::constraints Motion::constraints::from_proto(const service::motion::v1::Constraints& proto) {
    std::vector<Motion::linear_constraint> lcs;
    for (const auto& proto_lc : proto.linear_constraint()) {
        Motion::linear_constraint lc;
        lc.orientation_tolerance_degs = proto_lc.orientation_tolerance_degs();
        lc.line_tolerance_mm = proto_lc.line_tolerance_mm();
        lcs.push_back(lc);
    }

    std::vector<Motion::orientation_constraint> ocs;
    for (const auto& proto_oc : proto.orientation_constraint()) {
        Motion::orientation_constraint oc;
        oc.orientation_tolerance_degs = proto_oc.orientation_tolerance_degs();
        ocs.push_back(oc);
    }

    std::vector<Motion::collision_specification> css;
    for (const auto& proto_cs : proto.collision_specification()) {
        std::vector<Motion::collision_specification::allowed_frame_collisions> allows;
        for (const auto& proto_allow : proto_cs.allows()) {
            Motion::collision_specification::allowed_frame_collisions allow;
            allow.frame1 = proto_allow.frame1();
            allow.frame2 = proto_allow.frame2();
            allows.push_back(allow);
        }
        Motion::collision_specification cs;
        cs.allows = allows;
        css.push_back(cs);
    }

    Motion::constraints constraints;
    constraints.linear_constraints = lcs;
    constraints.orientation_constraints = ocs;
    constraints.collision_specifications = css;

    return constraints;
}

API Motion::static_api() {
    return {kRDK, kService, "motion"};
}

API Motion::dynamic_api() const {
    return static_api();
}

std::shared_ptr<ResourceRegistration> Motion::resource_registration() {
    const google::protobuf::DescriptorPool* p = google::protobuf::DescriptorPool::generated_pool();
    const google::protobuf::ServiceDescriptor* sd =
        p->FindServiceByName(service::motion::v1::MotionService::service_full_name());
    if (!sd) {
        throw std::runtime_error("Unable to get service descriptor for the motion service");
    }
    return std::make_shared<MotionRegistration>(sd);
}

service::motion::v1::MotionConfiguration motion_configuration::to_proto() const {
    service::motion::v1::MotionConfiguration proto;

    for (const auto& name : vision_services) {
        *proto.mutable_vision_services()->Add() = name.to_proto();
    }

    if (position_polling_frequency_hz) {
        proto.set_position_polling_frequency_hz(*position_polling_frequency_hz);
    }

    if (obstacle_polling_frequency_hz) {
        proto.set_obstacle_polling_frequency_hz(*obstacle_polling_frequency_hz);
    }

    if (plan_deviation_m) {
        proto.set_plan_deviation_m(*plan_deviation_m);
    }

    if (linear_m_per_sec) {
        proto.set_linear_m_per_sec(*linear_m_per_sec);
    }

    if (angular_degs_per_sec) {
        proto.set_angular_degs_per_sec(*angular_degs_per_sec);
    }

    return proto;
}

motion_configuration motion_configuration::from_proto(
    const service::motion::v1::MotionConfiguration& proto) {
    motion_configuration mc;

    for (const auto& proto_name : proto.vision_services()) {
        mc.vision_services.push_back(Name::from_proto(proto_name));
    }

    if (proto.has_position_polling_frequency_hz()) {
        *mc.position_polling_frequency_hz = proto.position_polling_frequency_hz();
    }

    if (proto.has_obstacle_polling_frequency_hz()) {
        *mc.obstacle_polling_frequency_hz = proto.obstacle_polling_frequency_hz();
    }

    if (proto.has_plan_deviation_m()) {
        *mc.plan_deviation_m = proto.plan_deviation_m();
    }

    if (proto.has_linear_m_per_sec()) {
        *mc.linear_m_per_sec = proto.linear_m_per_sec();
    }

    if (proto.has_angular_degs_per_sec()) {
        *mc.angular_degs_per_sec = proto.angular_degs_per_sec();
    }

    return mc;
}

namespace {
bool init() {
    Registry::register_resource(Motion::static_api(), Motion::resource_registration());
    return true;
}

// NOLINTNEXTLINE
const bool inited = init();
}  // namespace

}  // namespace sdk
}  // namespace viam
