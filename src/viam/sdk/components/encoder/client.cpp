#include <viam/sdk/components/encoder/client.hpp>

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/encoder/v1/encoder.grpc.pb.h>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/encoder/encoder.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {

EncoderClient::EncoderClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : Encoder(std::move(name)),
      stub_(viam::component::encoder::v1::EncoderService::NewStub(channel)),
      channel_(std::move(channel)){};

Encoder::position EncoderClient::get_position(const AttributeMap& extra,
                                              position_type position_type) {
    viam::component::encoder::v1::GetPositionRequest request;
    viam::component::encoder::v1::GetPositionResponse response;

    grpc::ClientContext ctx;
    set_client_ctx_authority(ctx);

    *request.mutable_name() = this->name();
    request.set_position_type(to_proto(position_type));
    *request.mutable_extra() = map_to_struct(extra);

    const grpc::Status status = stub_->GetPosition(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return from_proto(response);
}

void EncoderClient::reset_position(const AttributeMap& extra) {
    viam::component::encoder::v1::ResetPositionRequest request;
    viam::component::encoder::v1::ResetPositionResponse response;

    grpc::ClientContext ctx;
    set_client_ctx_authority(ctx);

    *request.mutable_name() = this->name();
    *request.mutable_extra() = map_to_struct(extra);

    const grpc::Status status = stub_->ResetPosition(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
}

Encoder::properties EncoderClient::get_properties(const AttributeMap& extra) {
    viam::component::encoder::v1::GetPropertiesRequest request;
    viam::component::encoder::v1::GetPropertiesResponse response;

    grpc::ClientContext ctx;
    set_client_ctx_authority(ctx);

    *request.mutable_name() = this->name();
    *request.mutable_extra() = map_to_struct(extra);

    const grpc::Status status = stub_->GetProperties(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return from_proto(response);
}

std::vector<GeometryConfig> EncoderClient::get_geometries(const AttributeMap& extra) {
    viam::common::v1::GetGeometriesRequest req;
    viam::common::v1::GetGeometriesResponse resp;
    grpc::ClientContext ctx;
    set_client_ctx_authority(ctx);

    *req.mutable_name() = this->name();
    *req.mutable_extra() = map_to_struct(extra);

    stub_->GetGeometries(&ctx, req, &resp);
    return GeometryConfig::from_proto(resp);
};

AttributeMap EncoderClient::do_command(AttributeMap command) {
    viam::common::v1::DoCommandRequest request;
    viam::common::v1::DoCommandResponse response;

    grpc::ClientContext ctx;
    set_client_ctx_authority(ctx);

    const google::protobuf::Struct proto_command = map_to_struct(command);
    *request.mutable_command() = proto_command;
    *request.mutable_name() = this->name();

    const grpc::Status status = stub_->DoCommand(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return struct_to_map(response.result());
}

}  // namespace sdk
}  // namespace viam
