#include <iostream>
#include <memory>
#include <pthread.h>
#include <signal.h>

#include <boost/log/trivial.hpp>
#include <grpcpp/grpcpp.h>
#include <grpcpp/server_context.h>

#include <viam/api/common/v1/common.grpc.pb.h>
#include <viam/api/component/generic/v1/generic.grpc.pb.h>
#include <viam/api/robot/v1/robot.pb.h>

#include <viam/sdk/components/component.hpp>
#include <viam/sdk/components/generic/generic.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/module/module.hpp>
#include <viam/sdk/module/service.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/rpc/dial.hpp>
#include <viam/sdk/rpc/server.hpp>

using viam::component::generic::v1::GenericService;
using namespace viam::sdk;

// Printer is a modular resource that can print its own ID upon receiving a
// DoCommand request or reconfiguring. Printer IDs increase by 1 for each
// Printer constructed.
class Printer : public GenericService::Service, public Component {
   public:
    void reconfigure(Dependencies deps, ResourceConfig cfg) override {
        std::cout << "Calling reconfigure on Printer with id " << id_ << " and name " << name_
                  << std::endl;
        for (auto& dep : deps) {
            std::cout << "dependency: " << dep.first.to_string() << std::endl;
        }

        std::cout << "attributes in reconfigure cfg: " << cfg.attributes() << std::endl;
    }

    API dynamic_api() const override {
        return Generic::static_api();
    }

    Printer() {
        id_ = next_id_;
        next_id_ += 1;
    };

    Printer(ResourceConfig cfg) {
        name_ = cfg.name();
        std::cout << "Creating Printer with name " + name_ << std::endl;

        id_ = next_id_;
        next_id_ += 1;
    }

    Printer(const Printer&) = delete;
    Printer& operator=(const Printer&) = delete;

    ::grpc::Status DoCommand(::grpc::ServerContext* context,
                             const ::viam::common::v1::DoCommandRequest* request,
                             ::viam::common::v1::DoCommandResponse* response) override {
        std::cout << "Received DoCommand request for Printer with id " << id_ << " and name "
                  << name_ << std::endl;
        for (const auto& req : request->command().fields()) {
            std::cout << "request key: " << req.first.c_str()
                      << "\trequest value: " << req.second.SerializeAsString();
        }
        *response->mutable_result() = request->command();

        return grpc::Status();
    }

   private:
    std::string name_;
    static int next_id_;
    int id_;
};

int Printer::next_id_ = 0;

int main(int argc, char** argv) {
    if (argc < 2) {
        throw "need socket path as command line argument";
    }

    // Use set_logger_severity_from_args to set the boost trivial logger's
    // severity depending on commandline arguments.
    set_logger_severity_from_args(argc, argv);
    BOOST_LOG_TRIVIAL(debug) << "Starting module with debug level logging";

    // C++ modules must handle SIGINT and SIGTERM. Make sure to create a sigset
    // for SIGINT and SIGTERM that can be later awaited in a thread that cleanly
    // shuts down your module. pthread_sigmask should be called near the start
    // of main so that later threads inherit the mask.
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGINT);
    sigaddset(&sigset, SIGTERM);
    pthread_sigmask(SIG_BLOCK, &sigset, NULL);

    API generic = Generic::static_api();
    Model m("acme", "demo", "printer");

    std::shared_ptr<ModelRegistration> mr = std::make_shared<ModelRegistration>(
        ResourceType("Printer"),
        generic,
        m,
        [](Dependencies, ResourceConfig cfg) { return std::make_unique<Printer>(cfg); },
        // Custom validation can be done by specifying a validate function like
        // this one. Validate functions can `throw` exceptions that will be
        // returned to the parent through gRPC. Validate functions can also return
        // a vector of strings representing the implicit dependencies of the resource.
        [](ResourceConfig cfg) -> std::vector<std::string> {
            if (cfg.attributes()->find("invalidattribute") != cfg.attributes()->end()) {
                throw std::runtime_error(
                    "'invalidattribute' attribute not allowed for model 'acme:demo:printer'");
            }

            return {"component1"};
        });

    Registry::register_model(mr);

    // The `ModuleService_` must outlive the Server, so the declaration order
    // here matters.
    auto my_mod = std::make_shared<ModuleService_>(argv[1]);
    auto server = std::make_shared<Server>();

    my_mod->add_model_from_registry(server, generic, m);
    my_mod->start(server);

    std::thread server_thread([&server, &sigset]() {
        server->start();
        int sig = 0;
        auto result = sigwait(&sigset, &sig);
        server->shutdown();
    });

    server->wait();
    server_thread.join();

    return 0;
};
