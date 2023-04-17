#include <components/generic/server.hpp>

#include <components/generic/generic.hpp>
#include <rpc/server.hpp>

namespace viam {
namespace cppsdk {

::grpc::Status GenericServer::DoCommand(::grpc::ServerContext* context,
                                        const ::viam::common::v1::DoCommandRequest* request,
                                        ::viam::common::v1::DoCommandResponse* response) {
    if (request == nullptr) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [DoCommand] without a request");
    };

    std::shared_ptr<ResourceBase> rb = sub_svc->resource(request->name());
    if (rb == nullptr) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Generic> generic = std::dynamic_pointer_cast<Generic>(rb);
    AttributeMap result = generic->do_command(struct_to_map(request->command()));

    *response->mutable_result() = map_to_struct(result);

    return ::grpc::Status();
}

void GenericServer::register_server(std::shared_ptr<Server> server) {
    viam::component::generic::v1::GenericService::Service* generic =
        static_cast<viam::component::generic::v1::GenericService::Service*>(this);
    try {
        server->register_service(generic);
    } catch (std::exception& exc) {
        throw exc;
    }
}

std::shared_ptr<SubtypeService> GenericServer::get_sub_svc() {
    return sub_svc;
}

}  // namespace cppsdk
}  // namespace viam
