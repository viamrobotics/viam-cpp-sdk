#include <viam/sdk/rpc/server.hpp>

#include <boost/log/trivial.hpp>
#include <grpcpp/security/server_credentials.h>

#include <viam/sdk/registry/registry.hpp>

namespace viam {
namespace sdk {

Server::Server() : builder_(std::make_unique<grpc::ServerBuilder>()) {
    std::cout << "creating a new server! this should only happen once!" << std::endl;
    auto new_manager = std::make_shared<ResourceManager>();
    // CR erodkin: is this the best place to do this? Probably it makes more sense when we actually
    // register the service. see if we can figure out how to do that. Related: if we can do it right
    // then we don't need to manage resources separately in the module manager probably? it'll also
    // have a server and that server can track everything for them. But we'll need to test that!
    //
    // CR erodkin: is it possible we have a duplicate in the registered resources somehow and that's
    // what is causing the issue? Look into that possibility and/or the above CR as a separate
    // implementation
    for (const auto& rr : Registry::registered_resources()) {
        auto server = rr.second->create_resource_server(new_manager, *this);
        managed_servers_.emplace(server->api(), std::move(server));
    }
}

Server::~Server() {
    shutdown();
}

void Server::register_service(grpc::Service* service) {
    if (!builder_) {
        throw std::runtime_error("Cannot register a new service after the server has started");
    }
    std::cout << "registering a service! " << std::endl;

    try {
        builder_->RegisterService(service);
    } catch (const std::runtime_error& e) {
        std::cout << "caught an error!\n" << std::flush;
        BOOST_LOG_TRIVIAL(warning) << "Failed to register service: " << e.what();
    } catch (...) {
        std::cout << "caught an error!\n" << std::flush;
        BOOST_LOG_TRIVIAL(warning) << "Failed to register service with unknown error";
    }
}

void Server::add_resource(std::shared_ptr<Resource> resource) {
    if (!builder_) {
        throw std::runtime_error("Cannot add a new resource after the server has started");
    }

    auto api = resource->api();
    if (managed_servers_.find(api) == managed_servers_.end()) {
        std::ostringstream buffer;
        buffer << "Attempted to add resource with API: " << api
               << " but no matching resource server as found";
        throw std::runtime_error(buffer.str());
    }
    auto resource_server = managed_servers_.at(std::move(api));
    resource_server->resource_manager()->add(resource->name(), std::move(resource));
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
