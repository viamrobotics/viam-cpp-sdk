#pragma once

#include <grpcpp/impl/service_type.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server_builder.h>

namespace viam {
namespace cppsdk {

/// A grpc server
class Server {
   public:
    Server();
    ~Server();

    /// Starts the grpc server. This can only be called once, and will throw on
    /// repeated calls.
    void start();

    // TODO: make `register_service` take one of our types as an arg rather than a
    // grpc service type, and convert under the hood
    /// registers a grpc service.
    ///
    /// Args:
    /// 	service: the grpc service to be registered
    ///
    /// Raises:
    /// 	Must be called before starting the server; new services cannot be
    /// registered after 	the server starts. Attempting to do so will throw an
    /// error
    void register_service(grpc::Service* service);

    /// Adds a listening port to the server.
    /// Args:
    /// 	address: the address to listen at
    ///		creds: server credentials. defaults to insecure server
    /// credentials
    ///
    ///	Raises:
    ///		Must be called before starting the server. Attempting to call
    /// after
    /// the server has 		started will throw an error
    void add_listening_port(std::string address,
                            std::shared_ptr<grpc::ServerCredentials> creds = nullptr);

    void wait();
    void shutdown();

   private:
    std::unique_ptr<grpc::ServerBuilder> builder_;
    std::unique_ptr<grpc::Server> server_;
};

}  // namespace cppsdk
}  // namespace viam
