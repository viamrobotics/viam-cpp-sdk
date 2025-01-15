#include <viam/sdk/tests/mocks/mock_robot.hpp>

#include <vector>

#include <grpcpp/support/status.h>

#include <common/v1/common.pb.h>
#include <robot/v1/robot.pb.h>

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/resource/stoppable.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace robot {
using namespace viam::sdk;
using common::v1::Pose;
using common::v1::PoseInFrame;
using common::v1::ResourceName;
using viam::robot::v1::FrameSystemConfig;
using viam::robot::v1::Operation;

namespace {
std::vector<Name> registered_models_for_resource(const std::shared_ptr<Resource>& resource) {
    std::string resource_type;
    std::string resource_subtype;
    std::vector<Name> resource_names;
    for (const auto& kv : Registry::registered_models()) {
        const std::shared_ptr<const ModelRegistration> reg = kv.second;
        if (reg->api() == resource->api()) {
            resource_type = reg->api().resource_type();
            resource_subtype = reg->api().resource_subtype();
        } else {
            continue;
        }

        if (resource_subtype.empty()) {
            resource_subtype = resource->name();
        }

        resource_names.push_back({{kRDK, resource_type, resource_subtype}, "", resource->name()});
    }
    return resource_names;
}
}  // namespace

pose default_pose(int offset) {
    pose pose;
    pose.coordinates.x = 1 + offset;
    pose.coordinates.y = 2 + offset;
    pose.coordinates.z = 3 + offset;
    pose.orientation.o_x = 2 + offset;
    pose.orientation.o_y = 3 + offset;
    pose.orientation.o_z = 4 + offset;
    pose.theta = 20 + offset;
    return pose;
}

Pose default_proto_pose(int offset = 0) {
    return to_proto(default_pose(offset));
}

std::vector<RobotClient::operation> mock_operations_response() {
    RobotClient::operation op;
    op.id = "abc";
    op.method = "123";
    op.session_id = "jkl";
    RobotClient::operation op1;
    op1.id = "def";
    op1.method = "456";
    op1.session_id = "xyz";
    std::vector<RobotClient::operation> resp;
    resp.push_back(op);
    resp.push_back(op1);
    return resp;
}

std::vector<Operation> mock_proto_operations_response() {
    viam::robot::v1::Operation op;
    *op.mutable_id() = "abc";
    *op.mutable_method() = "123";
    *op.mutable_session_id() = "jkl";
    viam::robot::v1::Operation op1;
    *op1.mutable_id() = "def";
    *op1.mutable_method() = "456";
    *op1.mutable_session_id() = "xyz";
    std::vector<viam::robot::v1::Operation> resp;
    resp.push_back(op);
    resp.push_back(op1);
    return resp;
}

pose_in_frame mock_transform_response() {
    return {"arm", default_pose()};
}

PoseInFrame mock_proto_transform_response() {
    PoseInFrame response;
    *response.mutable_reference_frame() = "arm";
    *response.mutable_pose() = default_proto_pose();
    return response;
}

std::vector<Name> mock_resource_names_response() {
    Name camera = Name({kRDK, kComponent, "camera"}, "", "mock_camera");
    Name motor = Name({kRDK, kComponent, "motor"}, "", "mock_motor");
    Name generic = Name({kRDK, kComponent, "generic"}, "", "mock_generic");

    return {std::move(camera), std::move(motor), std::move(generic)};
}

std::vector<ResourceName> mock_proto_resource_names_response() {
    std::vector<ResourceName> vec;
    ResourceName camera;
    *camera.mutable_namespace_() = kRDK;
    *camera.mutable_type() = kComponent;
    *camera.mutable_name() = "mock_camera";
    *camera.mutable_subtype() = "camera";

    ResourceName generic;
    *generic.mutable_namespace_() = kRDK;
    *generic.mutable_type() = kComponent;
    *generic.mutable_name() = "mock_generic";
    *generic.mutable_subtype() = "generic";

    ResourceName motor;
    *motor.mutable_namespace_() = kRDK;
    *motor.mutable_type() = kComponent;
    *motor.mutable_name() = "mock_motor";
    *motor.mutable_subtype() = "motor";

    vec.push_back(camera);
    vec.push_back(motor);
    vec.push_back(generic);
    return vec;
}

std::vector<RobotClient::frame_system_config> mock_config_response() {
    RobotClient::frame_system_config config;
    WorldState::transform t;
    t.reference_frame = "some-reference-frame";
    pose_in_frame pif("reference0", default_pose());
    t.pose_in_observer_frame = pif;
    config.frame = t;
    config.kinematics = {{"fake-key", 1.0}};

    RobotClient::frame_system_config config1;
    WorldState::transform t1;
    t1.reference_frame = "another-reference-frame";
    pose_in_frame pif1("reference1", default_pose(1));
    t1.pose_in_observer_frame = pif1;
    config1.frame = t1;
    config1.kinematics = {{"new-fake-key", 2.0}};

    std::vector<RobotClient::frame_system_config> response;
    response.push_back(config);
    response.push_back(config1);
    return response;
}

std::vector<FrameSystemConfig> mock_proto_config_response() {
    FrameSystemConfig config;
    common::v1::Transform t;
    *t.mutable_reference_frame() = "some-reference-frame";
    Pose pose = default_proto_pose();
    PoseInFrame pif;
    *pif.mutable_reference_frame() = "reference0";
    *pif.mutable_pose() = pose;
    *t.mutable_pose_in_observer_frame() = pif;
    *config.mutable_frame() = t;
    google::protobuf::Struct s;
    google::protobuf::Value v;
    v.set_number_value(1);
    google::protobuf::MapPair<std::string, google::protobuf::Value> pair("fake-key", v);
    s.mutable_fields()->insert(pair);
    *config.mutable_kinematics() = s;

    FrameSystemConfig config1;
    common::v1::Transform t1;
    *t1.mutable_reference_frame() = "another-reference-frame";
    Pose pose1 = default_proto_pose(1);
    PoseInFrame pif1;
    *pif1.mutable_reference_frame() = "reference1";
    *pif1.mutable_pose() = pose1;
    *t1.mutable_pose_in_observer_frame() = pif1;
    *config1.mutable_frame() = t1;
    google::protobuf::Struct s1;
    google::protobuf::Value v1;
    v1.set_number_value(2);
    google::protobuf::MapPair<std::string, google::protobuf::Value> pair1("new-fake-key", v1);
    s1.mutable_fields()->insert(pair1);
    *config1.mutable_kinematics() = s1;

    std::vector<FrameSystemConfig> response;
    response.push_back(config);
    response.push_back(config1);
    return response;
}

MockRobotService::MockRobotService(const std::shared_ptr<ResourceManager>& manager, Server& server)
    : ResourceServer(manager) {
    server.register_service(this);
    // register all managed resources with the appropriate resource servers.
    for (const auto& resource : manager->resources()) {
        server.add_resource(resource.second);
    }
}

std::vector<ResourceName> MockRobotService::generate_metadata_() {
    std::vector<ResourceName> metadata;
    for (const auto& key_and_val : resource_manager()->resources()) {
        for (const Name& name : registered_models_for_resource(key_and_val.second)) {
            metadata.push_back(to_proto(name));
        }
    }
    return metadata;
}

::grpc::Status MockRobotService::ResourceNames(::grpc::ServerContext*,
                                               const viam::robot::v1::ResourceNamesRequest* request,
                                               viam::robot::v1::ResourceNamesResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [ResourceNames] without a request");
    }

    auto* p = response->mutable_resources();
    for (const ResourceName& name : generate_metadata_()) {
        *p->Add() = name;
    }

    return ::grpc::Status();
}

::grpc::Status MockRobotService::StopAll(::grpc::ServerContext*,
                                         const ::viam::robot::v1::StopAllRequest* request,
                                         ::viam::robot::v1::StopAllResponse*) {
    const ResourceName r;
    std::unordered_map<std::string, ProtoStruct> extra;
    for (const auto& ex : request->extra()) {
        const google::protobuf::Struct& struct_ = ex.params();
        const ProtoStruct value_map = from_proto(struct_);
        const std::string name = ex.name().SerializeAsString();
        extra.emplace(name, value_map);
    }

    grpc::StatusCode status = grpc::StatusCode::OK;
    std::string status_message;

    for (const auto& r : resource_manager()->resources()) {
        const std::shared_ptr<Resource> resource = r.second;
        const ResourceName rn = to_proto(resource->get_resource_name());
        const std::string rn_ = rn.SerializeAsString();
        if (extra.find(rn_) != extra.end()) {
            try {
                Stoppable::stop_if_stoppable(resource, extra.at(rn_));
            } catch (const std::runtime_error& err) {
                try {
                    status_message = err.what();
                    Stoppable::stop_if_stoppable(resource);
                } catch (std::runtime_error& err) {
                    status_message = err.what();
                    status = grpc::UNKNOWN;
                } catch (...) {
                    status_message = "unknown error";
                    status = grpc::UNKNOWN;
                }
            }
        } else {
            try {
                Stoppable::stop_if_stoppable(resource);
            } catch (std::runtime_error& err) {
                status_message = err.what();
                status = grpc::UNKNOWN;
            } catch (...) {
                status_message = "unknown error";
                status = grpc::UNKNOWN;
            }
        }
    }

    return grpc::Status(status, status_message);
}

std::shared_ptr<Resource> MockRobotService::resource_by_name(const Name& name) {
    std::shared_ptr<Resource> r;
    const std::lock_guard<std::mutex> lock(lock_);
    auto resources = resource_manager()->resources();
    if (resources.find(name.name()) != resources.end()) {
        r = resources.at(name.name());
    }

    return r;
}

::grpc::Status MockRobotService::FrameSystemConfig(
    ::grpc::ServerContext* context,
    const ::viam::robot::v1::FrameSystemConfigRequest*,
    ::viam::robot::v1::FrameSystemConfigResponse* response) {
    auto client_md = context->client_metadata();
    if (auto client_info = client_md.find("viam_client"); client_info == client_md.end()) {
        return ::grpc::Status(::grpc::StatusCode::FAILED_PRECONDITION,
                              "viam_client info not properly set in metadata");
    }
    auto* configs = response->mutable_frame_system_configs();
    for (const auto& c : mock_proto_config_response()) {
        *configs->Add() = c;
    }
    return ::grpc::Status();
}

::grpc::Status MockRobotService::TransformPose(::grpc::ServerContext* context,
                                               const ::viam::robot::v1::TransformPoseRequest*,
                                               ::viam::robot::v1::TransformPoseResponse* response) {
    auto client_md = context->client_metadata();
    if (auto client_info = client_md.find("viam_client"); client_info == client_md.end()) {
        return ::grpc::Status(::grpc::StatusCode::FAILED_PRECONDITION,
                              "viam_client info not properly set in metadata");
    }
    *response->mutable_pose() = mock_proto_transform_response();
    return ::grpc::Status();
}

::grpc::Status MockRobotService::GetOperations(::grpc::ServerContext* context,
                                               const ::viam::robot::v1::GetOperationsRequest*,
                                               ::viam::robot::v1::GetOperationsResponse* response) {
    auto client_md = context->client_metadata();
    if (auto client_info = client_md.find("viam_client"); client_info == client_md.end()) {
        return ::grpc::Status(::grpc::StatusCode::FAILED_PRECONDITION,
                              "viam_client info not properly set in metadata");
    }
    auto* ops = response->mutable_operations();
    for (auto& op : mock_proto_operations_response()) {
        *ops->Add() = op;
    }
    return ::grpc::Status();
}

}  // namespace robot
}  // namespace sdktests
}  // namespace viam
