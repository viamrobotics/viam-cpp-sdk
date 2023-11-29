#include <viam/sdk/components/generic/client.hpp>

#include <utility>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/generic/v1/generic.grpc.pb.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {

GenericClient::GenericClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : Generic(std::move(name)),
      stub_(viam::component::generic::v1::GenericService::NewStub(channel)),
      channel_(std::move(channel)){};

AttributeMap GenericClient::do_command(AttributeMap command) {
    viam::common::v1::DoCommandRequest req;
    viam::common::v1::DoCommandResponse resp;
    grpc::ClientContext ctx;
    set_client_ctx_authority(ctx);

    const google::protobuf::Struct proto_command = map_to_struct(command);
    *req.mutable_command() = proto_command;
    *req.mutable_name() = this->name();
    stub_->DoCommand(&ctx, req, &resp);
    return struct_to_map(resp.result());
}

void GenericClient::stop(const AttributeMap& extra) {}

std::vector<GeometryConfig> GenericClient::get_geometries() {
    viam::common::v1::GetGeometriesRequest req;
    viam::common::v1::GetGeometriesResponse resp;
    grpc::ClientContext ctx;
    set_client_ctx_authority(ctx);

    *req.mutable_name() = this->name();

    stub_->GetGeometries(&ctx, req, &resp);
    return GeometryConfig::from_proto(resp);
}

}  // namespace sdk
}  // namespace viam
