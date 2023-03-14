#include <components/generic/server.hpp>
#include <rpc/server.hpp>

::grpc::Status GenericServer::DoCommand(
    ::grpc::ServerContext* context,
    const ::viam::common::v1::DoCommandRequest* request,
    ::viam::common::v1::DoCommandResponse* response) {
    std::shared_ptr<ResourceBase> rb = sub_svc->resource(request->name());
    if (rb == nullptr) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<viam::component::generic::v1::GenericService::Service> generic =
        std::dynamic_pointer_cast<viam::component::generic::v1::GenericService::Service>(rb);
    return generic->DoCommand(context, request, response);
}

void GenericServer::register_server() {
    viam::component::generic::v1::GenericService::Service* generic =
        static_cast<viam::component::generic::v1::GenericService::Service*>(this);
    try {
        Server::register_service(generic);
    } catch (std::exception& exc) {
        throw exc;
    }
}

