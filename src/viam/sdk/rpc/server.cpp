#include <viam/sdk/rpc/server.hpp>

#include <grpcpp/security/server_credentials.h>

#include <viam/sdk/registry/registry.hpp>

namespace viam {
namespace sdk {

Server::Server() : builder_(std::make_unique<grpc::ServerBuilder>()) {
    auto new_manager = std::make_shared<ResourceManager>();
    // CR erodkin: is this the best place to do this? Probably it makes more sense when we actually
    // register the service. see if we can figure out how to do that. Related: if we can do it right
    // then we don't need to manage resources separately in the module manager probably? it'll also
    // have a server and that server can track everything for them. But we'll need to test that!
    for (const auto& rr : Registry::registered_resources()) {
        auto server = rr.second->create_resource_server(new_manager, *this);
        managed_servers_.push_back(server);
    }
}

Server::~Server() {
    shutdown();
}

void Server::register_service(grpc::Service* service) {
    if (!builder_) {
        throw std::runtime_error("Cannot register a new service after the server has started");
    }

    builder_->RegisterService(service);
}

void Server::start() {
    if (server_) {
        throw std::runtime_error("Attempted to start server that was already running");
    }

    server_ = builder_->BuildAndStart();
    builder_ = nullptr;
}

void Server::add_listening_port(std::string address,
                                std::shared_ptr<grpc::ServerCredentials> creds) {
    if (!builder_) {
        throw std::runtime_error("Cannot add a listening port after server has started");
    }

    if (!creds) {
        creds = grpc::InsecureServerCredentials();
    }

    builder_->AddListeningPort(address, creds);
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
