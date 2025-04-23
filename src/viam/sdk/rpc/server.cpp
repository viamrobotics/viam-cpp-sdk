#include <viam/sdk/rpc/server.hpp>

#include <sstream>

#include <grpcpp/impl/service_type.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server_builder.h>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/rpc/message_sizes.hpp>

namespace viam {
namespace sdk {

Server::Server() : builder_(std::make_unique<grpc::ServerBuilder>()) {
    builder_->SetMaxReceiveMessageSize(kMaxMessageSize);
    builder_->SetMaxSendMessageSize(kMaxMessageSize);
    builder_->SetMaxMessageSize(kMaxMessageSize);
    for (const auto& rr : Registry::get().registered_resource_servers()) {
        auto new_manager = std::make_shared<ResourceManager>();
        auto server = rr.second->create_resource_server(new_manager, *this);
        managed_servers_.emplace(rr.first, std::move(server));
    }
}

Server::~Server() {
    shutdown();
}

std::shared_ptr<ResourceServer> Server::lookup_resource_server(const API& api) {
    if (managed_servers_.find(api) == managed_servers_.end()) {
        return nullptr;
    }

    return managed_servers_.at(api);
}

void Server::register_service(grpc::Service* service) {
    if (!builder_) {
        throw Exception("Cannot register a new service after the server has started");
    }

    builder_->RegisterService(service);
}

namespace {
bool deadline_exceeded(const std::chrono::system_clock::time_point& deadline) {
    std::cout << "deadline is " << deadline.time_since_epoch().count() << " and now is "
              << std::chrono::system_clock::now().time_since_epoch().count() << "\n\n";
    return deadline < std::chrono::system_clock::now();  // deadline is before now
}
}  // namespace

void Server::add_resource(std::shared_ptr<Resource> resource) {
    add_resource(std::move(resource), nullptr);
}

void Server::add_resource(std::shared_ptr<Resource> resource,
                          std::chrono::system_clock::time_point* deadline) {
    if (deadline && deadline_exceeded(*deadline)) {
        throw Exception(ErrorCondition::k_general, "Deadline expired");
    }
    auto api = resource->api();
    if (managed_servers_.find(api) == managed_servers_.end()) {
        std::ostringstream buffer;
        buffer << "Attempted to add resource with API: " << api
               << " but no matching resource server was found";
        throw Exception(ErrorCondition::k_resource_not_found, buffer.str());
    }
    auto resource_server = managed_servers_.at(api);
    auto name = resource->name();
    resource_server->resource_manager()->add(std::move(name), std::move(resource));
}

void Server::start() {
    if (server_) {
        throw Exception("Attempted to start server that was already running");
    }

    server_ = builder_->BuildAndStart();
    builder_ = nullptr;
}

void Server::add_listening_port(const std::string& address,
                                std::shared_ptr<grpc::ServerCredentials> creds) {
    if (!builder_) {
        throw Exception("Cannot add a listening port after server has started");
    }

    if (!creds) {
        creds = grpc::InsecureServerCredentials();
    }

    builder_->AddListeningPort(address, std::move(creds));
}

void Server::wait() {
    if (server_) {
        server_->Wait();
    }
}

void Server::shutdown() {
    if (server_) {
        server_->Shutdown();
    }
}

}  // namespace sdk
}  // namespace viam
