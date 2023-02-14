#include <component/arm/v1/arm.grpc.pb.h>
#include <component/generic/v1/generic.grpc.pb.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/security/server_credentials.h>
#include <robot/v1/robot.pb.h>
#include <service/slam/v1/slam.grpc.pb.h>

#include <components/component_base.hpp>
#include <csignal>
#include <iostream>
#include <memory>
#include <module/module.hpp>
#include <module/service.hpp>
#include <rpc/dial.hpp>
#include <rpc/server.hpp>

#include "components/generic.hpp"
#include "config/resource.hpp"
#include "registry/registry.hpp"
#include "resource/resource.hpp"

using viam::component::generic::v1::GenericService;

class MyModule : public GenericService::Service, public ComponentBase {
   public:
    void signal_handler(int signum);
    static int which;
    int inner_which;
    MyModule() {
        inner_which = which;
        which += 1;
    };

    MyModule(const MyModule&) = delete;
    MyModule& operator=(const MyModule&) = delete;

    ::grpc::Status DoCommand(::grpc::ServerContext* context,
                             const ::viam::component::generic::v1::DoCommandRequest* request,
                             ::viam::component::generic::v1::DoCommandResponse* response) override {
        std::cout << "Received DoCommand request for MyModule number " << inner_which << std::endl;
        for (auto& req : request->command().fields()) {
            std::cout << "request key: " << req.first.c_str()
                      << "\trequest value: " << req.second.SerializeAsString();
        }
        *response->mutable_result() = request->command();

        return grpc::Status();
    }
};

int MyModule::which = 0;
void signal_handler(int signum) {
    std::cout << "GOT INTERRUPT SIGNAL" << signum << std::endl;
    std::exit(signum);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        throw "need socket path as command line argument";
    }

    std::signal(SIGINT, signal_handler);
    Subtype generic = Generic::subtype();
    std::shared_ptr<ModuleService_> my_mod = std::make_shared<ModuleService_>(argv[1]);
    Model m("acme", "demo", "printer");
    my_mod->add_model_from_registry(generic, m);

    std::shared_ptr<ComponentRegistration> cr2 = std::make_shared<ComponentRegistration>(
        ComponentType("MyModule"), "printer2", [](std::string, std::shared_ptr<grpc::Channel>) {
            return std::make_unique<MyModule>();
        });

    std::shared_ptr<ComponentRegistration> cr = std::make_shared<ComponentRegistration>(
        ComponentType("MyModule"), "printer1", [](std::string, std::shared_ptr<grpc::Channel>) {
            return std::make_unique<MyModule>();
        });

    Registry::register_component(cr2);
    Registry::register_component(cr);

    my_mod->start();
    Server::start();
    Server::wait();
    my_mod->close();
    return 0;
};
