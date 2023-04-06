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
    *req.mutable_name() = this->name();
    stub_->DoCommand(&ctx, req, &resp);
    return struct_to_map(resp.result());
};

