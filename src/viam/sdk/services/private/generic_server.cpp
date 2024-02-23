#include <viam/sdk/services/private/generic_server.hpp>

#include <viam/sdk/common/service_helper.hpp>
#include <viam/sdk/rpc/server.hpp>
#include <viam/sdk/services/generic.hpp>

namespace viam {
namespace sdk {

GenericServiceServer::GenericServiceServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(std::move(manager)){};

::grpc::Status GenericServiceServer::DoCommand(
    ::grpc::ServerContext*,
    const ::viam::common::v1::DoCommandRequest* request,
    ::viam::common::v1::DoCommandResponse* response) noexcept {
    return make_service_helper<GenericService>(
        "GenericServiceServer::DoCommand", this, request)([&](auto&, auto& generic) {
        const AttributeMap result = generic->do_command(struct_to_map(request->command()));
        *response->mutable_result() = map_to_struct(result);
    });
}

}  // namespace sdk
}  // namespace viam
