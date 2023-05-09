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

        // This metadata is modelled on the results obtained from
        // invoking `Metadata` on a instance of tflite_cpu configured
        // per the instructions and data at
        // https://github.com/viamrobotics/vision-service-examples/tree/aa4195485754151fccbfd61fbe8bed63db7f300f

        return {
            // name
            "EfficientDet Lite0 V1",

            // type
            "tflite_detector",

            // description
            "Identify which of a known set of objects might be present and provide information about their positions within the given image or a video stream.",

            // `inputs`
            {
                {
                    // name
                    "image",

                    // description
                    "Input image to be detected. The expected image is 320 x 320, with three channels (red, blue, and green) per pixel. Each value in the tensor is a single byte between 0 and 255.",

                    // data_type
                    "uint8",

                    // shape
                    {1, 320, 320, 3},

                    // associated_files
                    {},

                    // extra
                    // {}
                },
            },

            // outputs
            {
                {
                    // name
                    "location",

                    // description
                    "The locations of the detected boxes.",

                    // data_type
                    "float32",

                    // shape
                    {},

                    // associated_files
                    {}

                    // extra {
                    // fields {
                    // key: "labels"
                    // value {
                    //     string_value: "/example/labels.txt"
                    //    }
                    // }

                },

                {
                    // name
                    "category",

                    // description
                    "The categories of the detected boxes.",

                    // data_type
                    "float32",

                    // shape
                    {},

                    // associated files
                    {{
                        // name
                        "labelmap.txt",

                        // description
                        "Label of objects that this model can recognize.",

                        MLModelService::tensor_info::file::k_type_tensor_value,
                    }}

                    // extra
                    // {}
                },

                {
                    // name
                    "score",

                    // description
                    "The scores of the detected boxes.",

                    // data_type
                    "float32",

                    // shape
                    {},

                    // associated_files
                    {}

                    // extra
                    // {}

                },

                {
                    // name
                    "number of detections",

                    // description,
                    "The number of the detected boxes."

                    // data_type
                    "float32",

                    // shape
                    {},

                    // associated_files
                    {}

                    // extra
                    // {}
                }
            }
        };
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
        vs::MLModelService::static_subtype(),

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
