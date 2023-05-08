#include <pthread.h>
#include <signal.h>

#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include <viam/sdk/components/component_base.hpp>
#include <viam/sdk/module/service.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/rpc/server.hpp>
#include <viam/sdk/services/mlmodel/mlmodel.hpp>
#include <viam/sdk/services/mlmodel/server.hpp>

namespace {

namespace vs = ::viam::sdk;

const std::string service_name = "example_mlmodel";

// Robot config:
// {
//     "services" : [ {
//         "namespace" : "rdk",
//         "type" : "mlmodel",
//         "name" : "example_mlmodel",
//         "model" : "viam:example:mlmodel"
//     } ],
//     "modules" : [ {
//         "name" : "example-mlmodel-service",
//         "executable_path" : "/path/to/viam-cpp-sdk/build/install/bin/example_mlmodel"
//     } ],
//     "components" : []
// }
class ExampleMLModelService : public vs::MLModelService {
   public:
    explicit ExampleMLModelService(std::string name) : MLModelService(std::move(name)) {
        std::cout << "ExampleMLModelService: instantiated as '" << this->name() << "'" << std::endl;
    }

    infer_response infer(const infer_request& inputs) override {
        std::cout << "ExampleMLModelService: recieved `infer` invocation" << std::endl;
        return {};
    }

    struct metadata metadata() override {
        std::cout << "ExampleMLModelService: recieved `metadata` invocation" << std::endl;
        return {};
    }
};

int serve(const std::string& socket_path) {
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGINT);
    sigaddset(&sigset, SIGTERM);
    pthread_sigmask(SIG_BLOCK, &sigset, NULL);

    auto module_registration = std::make_shared<vs::ModelRegistration>(
        // NOTE: What does this string do/mean/constrain/affect?
        vs::ResourceType{"ExampleMLModelServiceModule"},

        // NOTE: This feels like an opportunity to take the type
        vs::MLModelService::subtype(),

        // NOTE: What do these strings do/mean/constrain/affect?
        vs::Model{"viam", "example", "mlmodel"},

        [](vs::Dependencies, vs::ResourceConfig resource_config) -> std::shared_ptr<vs::Resource> {
            return std::make_shared<ExampleMLModelService>(resource_config.name());
        },

        [](vs::ResourceConfig resource_config) -> std::vector<std::string> {
            return {};
        });

    vs::Registry::register_resource(module_registration);
    auto module_service = std::make_shared<vs::ModuleService_>(socket_path);

    auto server = std::make_shared<vs::Server>();
    module_service->add_model_from_registry(
        server, module_registration->subtype(), module_registration->model());

    module_service->start(server);

    std::thread server_thread([&server, &sigset]() {
        server->start();
        int sig = 0;
        auto result = sigwait(&sigset, &sig);
        server->shutdown();
    });

    server->wait();
    server_thread.join();

    return EXIT_SUCCESS;
}

}  // namespace

int main(int argc, char* argv[]) {
    const std::string usage = "usage: example_mlmodel /path/to/unix/socket";

    if (argc < 2) {
        std::cout << "ERROR: insufficient arguments\n";
        std::cout << usage << "\n";
        return EXIT_FAILURE;
    }

    return serve(argv[1]);
}
