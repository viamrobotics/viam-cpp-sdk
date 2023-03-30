#include <components/generic/client.hpp>

#include <utility>

#include <common/v1/common.pb.h>
#include <component/generic/v1/generic.grpc.pb.h>

#include <common/proto_type.hpp>
#include <config/resource.hpp>

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

GenericClient::GenericClient(std::string name, std::shared_ptr<grpc::Channel> channel_)
    : channel_(channel_),
      stub_(viam::component::generic::v1::GenericService::NewStub(channel_)),
      name_(name){};

GenericClient::GenericClient(std::string name) : channel_(nullptr), stub_(nullptr), name_(name){};
