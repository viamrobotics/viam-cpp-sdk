#include <components/generic/server.hpp>
#include <rpc/server.hpp>
#include <components/generic/generic.hpp>

::grpc::Status GenericServer::DoCommand(::grpc::ServerContext* context,
                                        const ::viam::common::v1::DoCommandRequest* request,
                                        ::viam::common::v1::DoCommandResponse* response) {
    std::shared_ptr<ResourceBase> rb = sub_svc->resource(request->name());
    if (rb == nullptr) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Generic> generic = std::dynamic_pointer_cast<Generic>(rb);

    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> command = struct_to_map(request->command());
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> result = generic->do_command(request->name(), command);

    *response->mutable_result() = map_to_struct(result);

    return grpc::Status();
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

