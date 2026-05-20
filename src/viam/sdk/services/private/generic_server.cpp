#include <viam/sdk/services/private/generic_server.hpp>

#include <viam/sdk/common/private/service_helper.hpp>
#include <viam/sdk/rpc/server.hpp>
#include <viam/sdk/services/generic.hpp>

namespace viam {
namespace sdk {
namespace impl {

GenericServiceServer::GenericServiceServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(std::move(manager)) {}

::grpc::Status GenericServiceServer::DoCommand(
    ::grpc::ServerContext* context,
    const ::viam::common::v1::DoCommandRequest* request,
    ::viam::common::v1::DoCommandResponse* response) noexcept {
    return make_service_helper<GenericService>(
        "GenericServiceServer::DoCommand", this, context, request)([&](auto&, auto& generic) {
        const ProtoStruct result = generic->do_command(from_proto(request->command()));
        *response->mutable_result() = to_proto(result);
    });
}

::grpc::Status GenericServiceServer::GetStatus(
    ::grpc::ServerContext* context,
    const ::viam::common::v1::GetStatusRequest* request,
    ::viam::common::v1::GetStatusResponse* response) noexcept {
    return make_service_helper<GenericService>(
        "GenericServiceServer::GetStatus", this, context, request)([&](auto&, auto& generic) {
        const ProtoStruct result = generic->get_status();
        *response->mutable_result() = to_proto(result);
    });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
