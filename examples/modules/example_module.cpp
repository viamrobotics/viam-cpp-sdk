#include <component/arm/v1/arm.grpc.pb.h>
#include <component/generic/v1/generic.grpc.pb.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/security/server_credentials.h>
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
        ///[>response->mutable_result() = request->command();

        return grpc::Status();
    }
};

int main(int argc, char** argv) {
    if (argc != 2) {
        throw "need socket path as command line argument";
    }

    Subtype generic = Generic::subtype();
    std::shared_ptr<ModuleService_> my_mod = std::make_shared<ModuleService_>(argv[1]);
    Model m("acme", "demo", "printer");
    my_mod->add_model_from_registry(generic, m);

    ComponentRegistration cr{
        {"MyModule"},
        "printer1",
        [my_mod](std::string, std::shared_ptr<grpc::Channel>) {
            // CR erodkin: this is ugly and weird but maybe it'll work?
            std::cout << "CREATING module! Is this before or after we start the module service?"
                      << std::endl;
            std::unique_ptr<MyModule> mm = std::make_unique<MyModule>();
            // my_mod->server_builder->RegisterService(mm.get());
            // my_mod->server = std::move(my_mod->server_builder->BuildAndStart());
            // std::cout << "CREATEDD module!" << std::endl;

            return mm;

            // return std::make_unique<MyModule>();
        },
        [](Dependencies, Component) { return std::make_unique<MyModule>(); }};
    // CR erodkin: fix
    std::shared_ptr<ComponentRegistration> cr2 = std::make_shared<ComponentRegistration>(cr);
    Registry registry;
    registry.register_component(cr2);

    // grpc::ServerBuilder builder;
    // viam::component::generic::v1::GenericService::Service gs;
    // builder.RegisterService(&gs);
    // builder.AddListeningPort("unix://" + my_mod->module->addr,
    // grpc::InsecureServerCredentials());
    // std::unique_ptr<grpc::reflection::ProtoServerReflectionPlugin> reflection =
    // std::make_unique<grpc::reflection::ProtoServerReflectionPlugin>();
    // reflection->UpdateServerBuilder(&builder);
    // grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    // std::unique_ptr<grpc::Server> s = builder.BuildAndStart();

    my_mod->start();
    my_mod->server->Wait();
    // s->Wait();

    return 0;
};
