#include <component/arm/v1/arm.grpc.pb.h>
#include <component/generic/v1/generic.grpc.pb.h>
#include <grpcpp/grpcpp.h>
#include <robot/v1/robot.pb.h>

#include <components/component_base.hpp>
#include <memory>
#include <module/module.hpp>
#include <module/service.hpp>
#include <rpc/dial.hpp>

#include "components/generic.hpp"
#include "registry/registry.hpp"
#include "resource/resource.hpp"

using viam::component::generic::v1::GenericService;

class MyModule : public GenericService::Service, public ComponentBase {
   public:
    MyModule(){};

    MyModule(const MyModule&) = delete;
    MyModule& operator=(const MyModule&) = delete;

    ::grpc::Status DoCommand(::grpc::ServerContext* context,
                             const ::viam::component::generic::v1::DoCommandRequest* request,
                             ::viam::component::generic::v1::DoCommandResponse* response) override {
        std::cout << "Got some requests, let's see what they are!" << std::endl;
        for (auto& req : request->command().fields()) {
            std::cout << "request key: " << req.first
                      << "\trequest value: " << req.second.SerializeAsString();
        }

        return grpc::Status();
    }
};

int main(int argc, char** argv) {
    std::cout << "we begin!\n";
    if (argc != 2) {
        throw "need socket path as command line argument";
    }

    std::cout << "making moduleservice!\n";
    ModuleService_ my_mod(argv[1]);
    std::cout << "making model!\n";
    Model m("acme", "demo", "printer");
    my_mod.add_model_from_registry(GENERIC_SUBTYPE, m);

    std::cout << "making component registration!\n";
    ComponentRegistration cr{
        {"my_module"}, "make", [](std::string, std::shared_ptr<grpc::Channel>) {
            return std::make_unique<MyModule>();
        }};
    Registry registry;
    std::cout << "registering component!\n";
    registry.register_component(cr);
    std::cout << "going to start!\n";

    my_mod.start();
    std::cout << "starting!\n";
    my_mod.server.get()->Wait();
    std::cout << "We're done!";

    return 0;
};
