#include <iostream>
#include <memory>
#include <pthread.h>
#include <signal.h>

#include <grpcpp/grpcpp.h>
#include <grpcpp/server_context.h>

#include <viam/api/common/v1/common.grpc.pb.h>
#include <viam/api/component/generic/v1/generic.grpc.pb.h>
#include <viam/api/robot/v1/robot.pb.h>

#include <viam/sdk/components/component_base.hpp>
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

class MyModule : public GenericService::Service, public Component {
   public:
    void reconfigure(Dependencies deps, ResourceConfig cfg) override {
        std::cout << "Calling reconfigure on MyModule" << std::endl;
        for (auto& dep : deps) {
            std::cout << "dependency: " << dep.first.to_string() << std::endl;
        }

        std::cout << "config in reconfigure: " << cfg.name() << std::endl;
    }

    Subtype instance_subtype() const override {
        return Generic::subtype();
    }

    MyModule() {
        inner_which_ = which_;
        which_ += 1;
    };

    MyModule(ResourceConfig cfg) {
        name_ = cfg.name();
        std::cout << "Creating module with name " + name_ << std::endl;
        inner_which_ = which_;
        which_ += 1;
    }

    MyModule(const MyModule&) = delete;
    MyModule& operator=(const MyModule&) = delete;

    ::grpc::Status DoCommand(::grpc::ServerContext* context,
                             const ::viam::common::v1::DoCommandRequest* request,
                             ::viam::common::v1::DoCommandResponse* response) override {
        std::cout << "Received DoCommand request for MyModule number " << inner_which_
                  << " and name " << name_ << std::endl;
        for (auto& req : request->command().fields()) {
            std::cout << "request key: " << req.first.c_str()
                      << "\trequest value: " << req.second.SerializeAsString();
        }
        *response->mutable_result() = request->command();

        return grpc::Status();
    }

   private:
    std::string name_;
    static int which_;
    int inner_which_;
};

int MyModule::which_ = 0;

int main(int argc, char** argv) {
    if (argc < 2) {
        throw "need socket path as command line argument";
    }

    // C++ modules must handle SIGINT and SIGTERM. Make sure to create a sigset
    // for SIGINT and SIGTERM that can be later awaited in a thread that cleanly
    // shuts down your module. pthread_sigmask should be called near the start
    // of main so that later threads inherit the mask.
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGINT);
    sigaddset(&sigset, SIGTERM);
    pthread_sigmask(SIG_BLOCK, &sigset, NULL);

    Subtype generic = Generic::subtype();
    Model m("acme", "demo", "printer");

    std::shared_ptr<ModelRegistration> rr = std::make_shared<ModelRegistration>(
        ResourceType("MyModule"),
        generic,
        m,
        [](Dependencies, ResourceConfig cfg) { return std::make_unique<MyModule>(cfg); },
        // Custom validation can be done by specifying a validate function like
        // this one. Validate functions can `throw` error strings that will be
        // returned to the parent through gRPC. Validate functions can also return
        // a vector of strings representing the implicit dependencies of the resource.
        [](ResourceConfig cfg) -> std::vector<std::string> {
            if (cfg.attributes()->find("invalidattribute") != cfg.attributes()->end()) {
                throw std::string(
                    "'invalidattribute' attribute not allowed for model 'acme:demo:printer'");
            }

            return {"component1"};
        });

    Registry::register_resource(rr);

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
