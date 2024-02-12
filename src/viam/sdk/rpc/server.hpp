/// @file rpc/server.hpp
///
/// @brief Defines the `Server` class.
#pragma once

#include <grpcpp/impl/service_type.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server_builder.h>

#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/resource/resource_api.hpp>
#include <viam/sdk/resource/resource_server_base.hpp>

namespace viam {

// needed for later `friend` declaration.
namespace sdktests {
class TestServer;
}

namespace sdk {

/// @class Server server.hpp "rpc/server.hpp"
/// @brief Defines gRPC `Server` functionality.
class Server {
   public:
    Server();
    ~Server();

    /// @brief Starts the grpc server. Can only be called once.
    /// @throws `std::runtime_error` if the server was already `start`ed.
    /// repeated calls.
    void start();

    /// @brief Registers a gRPC service.
    /// @param service The gRPC service to be registered.
    /// @throws `std::runtime_error` if called after the server has been `start`ed.
    void register_service(grpc::Service* service);

    /// @brief Returns reference to managed resource server.
    /// @param api The api of the managed resource server.
    /// @returns The requested resource server, or nullptr if it doesn't exist.
    std::shared_ptr<ResourceServer> lookup_resource_server(const API& api);

    /// @brief Adds a specific managed resource to the associated resource server
    /// @param resource The resource to add
    /// @throws `std::runtime_error` if a matching `ResourceServer` doesn't exist in the server.
    void add_resource(const std::shared_ptr<Resource>& resource);

    /// @brief Adds a listening port to the server.
    /// @param address The address to listen at.
    /// @param creds The server credentials; defaults to a insecure server credentials.
    /// @throws `std::runtime_error` if called after the server has been `start`ed.
    void add_listening_port(const std::string& address,
                            std::shared_ptr<grpc::ServerCredentials> creds = nullptr);

    /// @brief waits on server close, only returning when the server is closed.
    void wait();

    /// @brief Shutdown the gRPC server.
    void shutdown();

    // friend declaration to allow TestServer to access the private server_ field.
    friend class ::viam::sdktests::TestServer;

   private:
    std::unordered_map<API, std::shared_ptr<ResourceServer>> managed_servers_;
    std::unique_ptr<grpc::ServerBuilder> builder_;
    std::unique_ptr<grpc::Server> server_;
};

}  // namespace sdk
}  // namespace viam
