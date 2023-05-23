#include <pthread.h>
#include <signal.h>

#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include <viam/sdk/components/component.hpp>
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

    std::shared_ptr<named_tensor_views> infer(const named_tensor_views& inputs) override {
        std::cout << "ExampleMLModelService: recieved `infer` invocation" << std::endl;

        static constexpr std::array<float, 4> location_data = {0.1, 0.1, 0.75, 0.75};
        static constexpr std::array<float, 1> category_data = {0};
        static constexpr std::array<float, 1> score_data = {.99};
        static constexpr std::array<float, 1> num_dets_data = {1};

        auto location_tensor =
            make_tensor_view(location_data.data(), location_data.size(), {1, 1, 4});

        auto category_tensor = make_tensor_view(category_data.data(), category_data.size(), {1, 1});

        auto score_tensor = make_tensor_view(score_data.data(), score_data.size(), {1, 1});

        auto num_dets_tensor = make_tensor_view(num_dets_data.data(), num_dets_data.size(), {1});

        using namespace std::literals::string_literals;
        named_tensor_views tensors{{"location"s, std::move(location_tensor)},
                                   {"category"s, std::move(category_tensor)},
                                   {"score"s, std::move(score_tensor)},
                                   {"n_detections"s, std::move(num_dets_tensor)}};

        return std::make_shared<named_tensor_views>(std::move(tensors));
    }

    struct metadata metadata() override {
        std::cout << "ExampleMLModelService: recieved `metadata` invocation" << std::endl;

        // This metadata is modelled on the results obtained from
        // invoking `Metadata` on a instance of tflite_cpu configured
        // per the instructions and data at
        // https://github.com/viamrobotics/vision-service-examples/tree/aa4195485754151fccbfd61fbe8bed63db7f300f

        return {// `name`
                "C++ SDK Example MLModel - Faking EfficientDet Lite0 V1",

                // `type`
                "fake_tflite_detector",

                // `description`
                "Identify which of a known set of objects might be present and provide "
                "information "
                "about their positions within the given image or a video stream.",

                // `inputs`
                {
                    {// `name`
                     "image",

                     // `description`
                     "Input image to be detected.",

                     // `data_type`
                     tensor_info::data_types::k_uint8,

                     // `shape`
                     {1, 320, 320, 3},

                     // `associated_files`
                     {},

                     // `extra`
                     {}},
                },

                // `outputs`
                {{// `name`
                  "location",

                  // `description`
                  "The locations of the detected boxes.",

                  // `data_type`
                  tensor_info::data_types::k_float32,

                  // `shape`
                  //
                  // TODO(RSDK-3033): Once tflite_cpu reports output
                  // shape metadata, fill this in.
                  {},

                  // `associated_files`
                  {},

                  // `extra`
                  std::make_shared<vs::AttributeMap::element_type>(
                      std::initializer_list<vs::AttributeMap::element_type::value_type>{
                          {"labels", std::make_shared<vs::ProtoType>("/example/labels.txt")}})},

                 {// `name`
                  "category",

                  // `description`
                  "The categories of the detected boxes.",

                  // `data_type`
                  tensor_info::data_types::k_float32,

                  // `shape`
                  //
                  // TODO(RSDK-3033): Once tflite_cpu reports output
                  // shape metadata, fill this in.
                  {},

                  // `associated files`
                  {{
                      // `name`
                      "labelmap.txt",

                      // `description`
                      "Label of objects that this model can recognize.",

                      MLModelService::tensor_info::file::k_label_type_tensor_value,
                  }},

                  // `extra`
                  {}},

                 {// `name`
                  "score",

                  // `description`
                  "The scores of the detected boxes.",

                  // `data_type`
                  tensor_info::data_types::k_float32,

                  // `shape`
                  //
                  // TODO(RSDK-3033): Once tflite_cpu reports output
                  // shape metadata, fill this in.
                  {},

                  // `associated_files`
                  {},

                  // `extra`
                  {}},

                 {// `name`
                  "n_detections",

                  // `description`
                  "The number of the detected boxes.",

                  // `data_type`
                  tensor_info::data_types::k_float32,

                  // `shape`
                  //
                  // TODO(RSDK-3033): Once tflite_cpu reports output
                  // shape metadata, fill this in.
                  {},

                  // `associated_files`
                  {},

                  // `extra`
                  {}}}};
    }
};

int serve(const std::string& socket_path) {
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGINT);
    sigaddset(&sigset, SIGTERM);
    pthread_sigmask(SIG_BLOCK, &sigset, NULL);

    auto module_registration = std::make_shared<vs::ModelRegistration>(
        vs::ResourceType{"ExampleMLModelServiceModule"},
        vs::MLModelService::static_api(),
        vs::Model{"viam", "example", "mlmodel"},
        [](vs::Dependencies, vs::ResourceConfig resource_config) -> std::shared_ptr<vs::Resource> {
            return std::make_shared<ExampleMLModelService>(resource_config.name());
        },
        [](vs::ResourceConfig resource_config) -> std::vector<std::string> { return {}; });

    vs::Registry::register_model(module_registration);
    auto module_service = std::make_shared<vs::ModuleService_>(socket_path);

    auto server = std::make_shared<vs::Server>();
    module_service->add_model_from_registry(
        server, module_registration->api(), module_registration->model());

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
