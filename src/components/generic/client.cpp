#include <components/generic/client.hpp>

#include <utility>

#include <common/v1/common.pb.h>
#include <component/generic/v1/generic.grpc.pb.h>

#include <common/proto_type.hpp>
#include <config/resource.hpp>
#include <robot/client.hpp>

AttributeMap GenericClient::do_command(AttributeMap command) {
    viam::common::v1::DoCommandRequest req;
    viam::common::v1::DoCommandResponse resp;
    grpc::ClientContext ctx;

    google::protobuf::Struct proto_command = map_to_struct(command);
    *req.mutable_command() = proto_command;
    *req.mutable_name() = name_;
    stub_->DoCommand(&ctx, req, &resp);
    return struct_to_map(resp.result());
};

std::shared_ptr<GenericClient> GenericClient::from_robot(std::shared_ptr<RobotClient> robot,
                                                         std::string name) {
    ResourceName r;
    *r.mutable_namespace_() = RDK;
    *r.mutable_type() = COMPONENT;
    *r.mutable_subtype() = "generic";
    *r.mutable_name() = std::move(name);

    auto resource = robot->resource_by_name(std::move(r), ResourceType("generic"));
    return std::dynamic_pointer_cast<GenericClient>(resource);
}

GenericClient::GenericClient(std::string name, std::shared_ptr<grpc::Channel> channel_)
    : channel_(channel_), stub_(viam::component::generic::v1::GenericService::NewStub(channel_)) {
    name_ = std::move(name);
    type_ = {"generic"};
}

GenericClient::GenericClient(std::string name) : channel_(nullptr), stub_(nullptr) {
    name_ = std::move(name);
    type_ = {"generic"};
};
