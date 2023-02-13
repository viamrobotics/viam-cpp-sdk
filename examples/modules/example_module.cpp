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
#include <rpc/server.hpp>

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

    static bool init(MyModule* mm) {
        std::cout << "INITING!!" << std::endl;
        Server::register_service(mm);
        return true;
    };
};

static MyModule MY_MODULE;

static bool init = MyModule::init(&MY_MODULE);

int main(int argc, char** argv) {
    if (argc != 2) {
        throw "need socket path as command line argument";
    }

    Subtype generic = Generic::subtype();
    std::shared_ptr<ModuleService_> my_mod = std::make_shared<ModuleService_>(argv[1]);
    Model m("acme", "demo", "printer");
    my_mod->add_model_from_registry(generic, m);

    ComponentRegistration cr{{"MyModule"},
                             "printer1",
                             [my_mod](std::string, std::shared_ptr<grpc::Channel>) {
                                 return std::make_unique<MyModule>();
                             },
                             [](Dependencies, Component) { return std::make_unique<MyModule>(); }};
    // CR erodkin: fix
    std::shared_ptr<ComponentRegistration> cr2 = std::make_shared<ComponentRegistration>(cr);
    Registry registry;
    registry.register_component(cr2);

    my_mod->start();
    Server::start();
    std::cout << "Waiting for server" << std::endl;
    Server::wait();
    // my_mod->server->Wait();
    //  s->Wait();

    return 0;
};
