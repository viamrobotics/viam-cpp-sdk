#include <viam/sdk/services/generic/server.hpp>

#include <viam/sdk/common/service_helper.hpp>
#include <viam/sdk/rpc/server.hpp>
#include <viam/sdk/services/generic/generic.hpp>

namespace viam {
namespace sdk {

GenericServiceServer::GenericServiceServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(std::move(manager)){};

::grpc::Status GenericServiceServer::DoCommand(
    ::grpc::ServerContext* context,
    const ::viam::common::v1::DoCommandRequest* request,
    ::viam::common::v1::DoCommandResponse* response) noexcept {
    return make_service_helper<GenericService>(
        "GenericServiceServer::DoCommand", this, request)([&](auto& helper, auto& generic) {
        const AttributeMap result = generic->do_command(struct_to_map(request->command()));
        *response->mutable_result() = map_to_struct(result);
    });
}

API GenericServiceServer::api() const {
    return API::get<GenericService>();
}

}  // namespace sdk
}  // namespace viam