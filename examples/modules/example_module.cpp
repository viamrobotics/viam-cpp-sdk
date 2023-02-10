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
#include "config/resource.hpp"
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
        *response->mutable_result() = request->command();

        return grpc::Status();
    }
};

int main(int argc, char** argv) {
    if (argc != 2) {
        throw "need socket path as command line argument";
    }

    Subtype generic = Generic::subtype();
    ModuleService_ my_mod(argv[1]);
    Model m("acme", "demo", "printer");
    my_mod.add_model_from_registry(generic, m);

    ComponentRegistration cr{
        {"MyModule"},
        "printer1",
        [](std::string, std::shared_ptr<grpc::Channel>) { return std::make_unique<MyModule>(); },
        [](Dependencies, Component) { return std::make_unique<MyModule>(); }};
    // CR erodkin: fix
    std::shared_ptr<ComponentRegistration> cr2 = std::make_shared<ComponentRegistration>(cr);
    Registry registry;
    registry.register_component(cr2);

    my_mod.start();
    my_mod.server.get()->Wait();

    return 0;
};
