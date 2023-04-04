#include <common/v1/common.grpc.pb.h>
#include <component/generic/v1/generic.grpc.pb.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/server_context.h>
#include <robot/v1/robot.pb.h>

#include <components/component_base.hpp>
#include <components/generic/generic.hpp>
#include <config/resource.hpp>
#include <iostream>
#include <memory>
#include <module/module.hpp>
#include <module/service.hpp>
#include <registry/registry.hpp>
#include <resource/resource.hpp>
#include <rpc/dial.hpp>
#include <rpc/server.hpp>

using viam::component::generic::v1::GenericService;

class MyModule : public GenericService::Service, public ComponentBase {
   public:
    void reconfigure(Dependencies deps, Resource cfg) override {
        std::cout << "Calling reconfigure on MyModule" << std::endl;
        for (auto& dep : deps) {
            std::cout << "dependency: " << dep.first.to_string() << std::endl;
        }

        std::cout << "config in reconfigure: " << cfg.name << std::endl;
    }

    std::string name;
    static int which;
    int inner_which;
    MyModule() {
        inner_which = which;
        which += 1;
    };

    MyModule(Resource cfg) {
        name = cfg.name;
        std::cout << "Creating module with name " + name << std::endl;
        inner_which = which;
        which += 1;
    }

    MyModule(const MyModule&) = delete;
    MyModule& operator=(const MyModule&) = delete;

    ::grpc::Status DoCommand(::grpc::ServerContext* context,
                             const ::viam::common::v1::DoCommandRequest* request,
                             ::viam::common::v1::DoCommandResponse* response) override {
        std::cout << "Received DoCommand request for MyModule number " << inner_which
                  << " and name " << name << std::endl;
        for (auto& req : request->command().fields()) {
            std::cout << "request key: " << req.first.c_str()
                      << "\trequest value: " << req.second.SerializeAsString();
        }
        *response->mutable_result() = request->command();

        return grpc::Status();
    }
};

int MyModule::which = 0;
std::shared_ptr<ModuleService_> my_mod;

int main(int argc, char** argv) {
    if (argc != 2) {
        throw "need socket path as command line argument";
    }

    Subtype generic = Generic::subtype();
    my_mod = std::make_shared<ModuleService_>(argv[1]);
    Model m("acme", "demo", "printer");
    std::shared_ptr<ModelRegistration> rr = std::make_shared<ModelRegistration>(
        ResourceType("MyModule"),
        generic,
        m,
        [](Dependencies, Resource cfg) { return std::make_unique<MyModule>(cfg); },
        // Custom validation can be done by specifying a validate function like
        // this one. Validate functions can `throw` error strings that will be
        // returned to the parent through gRPC. Validate functions can also return
        // a vector of strings representing the implicit dependencies of the resource.
        [](Resource cfg) -> std::vector<std::string> {
            if (cfg.attributes->find("invalidattribute") != cfg.attributes->end()) {
                throw std::string(
                    "'invalidattribute' attribute not allowed for model 'acme:demo:printer'");
            }

            return {"component1"};
        });

    Registry::register_resource(rr);
    my_mod->add_model_from_registry(generic, m);

    my_mod->start();
    Server::start();
    Server::wait();
    return 0;
};
