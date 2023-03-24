#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/security/server_credentials.h>

#include <rpc/server.hpp>

void Server::register_service(grpc::Service *service) {
  if (server != nullptr) {
    throw "Cannot register a new service after the server has started";
  }

  builder->RegisterService(service);
}

void Server::start() {
  if (server != nullptr) {
    throw "Attempted to start server that was already running";
  }
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  server = std::move(builder->BuildAndStart());
}

void Server::add_listening_port(
    std::string address, std::shared_ptr<grpc::ServerCredentials> creds) {
  if (creds == nullptr) {
    creds = grpc::InsecureServerCredentials();
  }

  if (server != nullptr) {
    throw "Cannot add a listening port after server has started";
  }

  builder->AddListeningPort(address, creds);
}

std::unique_ptr<grpc::ServerBuilder> Server::builder =
    std::make_unique<grpc::ServerBuilder>();
std::unique_ptr<grpc::Server> Server::server;

void Server::wait() { server->Wait(); }

void Server::shutdown() {
  if (server != nullptr) {
    server->Shutdown();
  }
}
