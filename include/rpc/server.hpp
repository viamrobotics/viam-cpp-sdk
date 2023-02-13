#include <grpcpp/impl/service_type.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server_builder.h>

/// A grpc server
class Server {
   public:
    // CR erodkin: do we need a way to create at all?
    Server();

    /// Starts the grpc server. This can only be called once, and will throw on repeated calls.
    static void start();

    // // CR erodkin: add a ticket number here.
    // CR erodkin: I think the architecture we're going for here is going to be a bit lackluster
    // insofar as we'll be limited to single instances for each type? Confirm, figure out how to
    // resolve. Probably as a minimum can just test with two printers instead of one?
    // TODO: make `register_service` take one of our types as an arg rather than a grpc service
    // type, and convert under the hood
    /// registers a grpc service.
    ///
    /// Args:
    /// 	service: the grpc service to be registered
    ///
    /// Raises:
    /// 	Must be called before starting the server; new services cannot be registered after
    /// 	the server starts. Attempting to do so will throw an error
    static void register_service(grpc::Service* service);

    /// Adds a listening port to the server.
    /// Args:
    /// 	address: the address to listen at
    ///		creds: server credentials. defaults to insecure server credentials
    ///
    ///	Raises:
    ///		Must be called before starting the server. Attempting to call after the server has
    ///		started will throw an error
    static void add_listening_port(std::string address,
                                   std::shared_ptr<grpc::ServerCredentials> creds = nullptr);

    // CR erodkin: probably this isn't great
    static grpc::Server* server_ptr();
    static std::unique_ptr<grpc::Server> start_();
    // CR erodkin: do we need this? figure out
    static void wait();
    static void shutdown();

   private:
    static std::unique_ptr<grpc::ServerBuilder> builder;
    static std::unique_ptr<grpc::Server> server;
};

