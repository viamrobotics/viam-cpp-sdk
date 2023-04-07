#include <rpc/server.hpp>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/security/server_credentials.h>

Server::Server() : builder_(std::make_unique<grpc::ServerBuilder>()) {}

Server::~Server() {
    shutdown();
}

void Server::register_service(grpc::Service* service) {
    if (!builder_) {
        throw "Cannot register a new service after the server has started";
    }

    builder_->RegisterService(service);
}

void Server::start() {
    if (server_) {
        throw "Attempted to start server that was already running";
    }

    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    server_ = std::move(builder_->BuildAndStart());
    builder_ = nullptr;
}

void Server::add_listening_port(std::string address,
                                std::shared_ptr<grpc::ServerCredentials> creds) {
    if (!builder_) {
        throw "Cannot add a listening port after server has started";
    }

    if (!creds) {
        creds = grpc::InsecureServerCredentials();
    }

    builder_->AddListeningPort(address, creds);
}

void Server::wait() {
    if (server_)
        server_->Wait();
}

void Server::shutdown() {
    if (server_) {
        server_->Shutdown();
    }
}
