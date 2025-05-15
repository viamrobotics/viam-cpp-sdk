// Copyright 2023 Viam Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <random>
#include <string>
#include <vector>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/moment.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/optional.hpp>
#include <boost/program_options.hpp>
#include <boost/variant/get.hpp>

#include <viam/sdk/common/instance.hpp>
#include <viam/sdk/robot/client.hpp>
#include <viam/sdk/services/mlmodel.hpp>

namespace {

namespace vsdk = ::viam::sdk;
namespace bacc = ::boost::accumulators;
namespace bf = ::boost::filesystem;
namespace bpo = ::boost::program_options;

// A template for a robot configuration which will serve the
// yamnet/classification model in the C++ SDK's tflite example
// module. It expects two interpolations - one for the path to the
// model file, and the other the path to the tflite module binary.
constexpr char kRobotConfigTemplate[] = R"(
{
  "services": [
    {
      "name": "yamnet_classification_tflite",
      "namespace": "rdk",
      "type": "mlmodel",
      "attributes": {
        "num_threads": 1,
        "model_path": "%s",
        "tensor_name_remappings": {
          "outputs": {
            "tower0/network/layer32/final_output": "categories"
          },
          "inputs": {
            "waveform_binary": "sample"
          }
        }
      },
      "model": "viam:mlmodelservice:example_mlmodelservice_tflite"
    }
  ],
  "modules": [
    {
      "name": "mlms_tflite_module",
      "executable_path": "%s"
    }
  ],
  "components": []
}
)";

}  // namespace

int main(int argc, char* argv[]) try {
    // Every Viam C++ SDK program must have one and only one Instance object which is created before
    // any other C++ SDK objects and stays alive until all Viam C++ SDK objects are destroyed.
    viam::sdk::Instance inst;

    // By default, log messages with VIAM_SDK_LOG have "Viam C++ SDK" as the resource name.
    // Here we override it with the name of the executable, trimming the leading path if present.

    std::string exe_name{argv[0]};

    const auto last_slash_pos = exe_name.find_last_of('/');
    if (last_slash_pos != std::string::npos) {
        exe_name = exe_name.substr(last_slash_pos);
    }

    viam::sdk::LogManager::get().set_global_resource_name(exe_name);

    // Build up our command line options. The example operates in two
    // modes. In the "--generate" mode, it takes command line
    // parameters needed to satisfy the interpolation points in the
    // robot configuration and then emits a robot configuration which
    // can be pasted into a new robot on app.viam.com. Otherwise, the
    // example operates in classification mode.
    bpo::options_description options_desc("options");

    options_desc.add_options()("help", "Produce this help message\n");

    // When `--generate` is set, the example will emit a robot configuration which includes
    // the provided paths for the model file and the tflite module.
    bool opt_generating = false;
    options_desc.add_options()(
        "generate",
        bpo::bool_switch(&opt_generating),
        "Generate a robot configuration for the yamnet tflite mlmodel service modular resource\n");

    // When `--generate`ing, the `--model-path` argument should be
    // the filesystem path to the model file for the
    // yamnet/classification tflite model file from
    // https://tfhub.dev/google/lite-model/yamnet/classification/tflite/1.
    boost::optional<std::string> opt_model_path;
    options_desc.add_options()(
        "model-path",
        bpo::value(&opt_model_path),
        "Path to the yamnet/classification model file to include in the generated config\n");

    // When `--generate`ing, the `--tflite-module-path` should contain
    // the filesystem path to a Viam modular resource which implements
    // the MLModelService API and which supports tflite
    // models. Currently, the only supported module is the
    // `mlmodelservice_tflite` modular resource example from the Viam
    // C++ SDK.
    boost::optional<std::string> opt_tflite_module_path;
    options_desc.add_options()("tflite-module-path",
                               bpo::value(&opt_tflite_module_path),
                               "Path to a mlmodelservice modular resource that provides "
                               "viam:mlmodelservice:example_mlmodelservice_tflite\n");

    // In classification mode, this optional parameter should contain
    // the filesystem path to the labels file for the model. The
    // labels file can be extracted from the yammnet/classification
    // model file with `unzip`. If no labels file is provided, the
    // tool will simply dump the raw scores without labels.
    boost::optional<std::string> opt_model_label_path;
    options_desc.add_options()(
        "model-label-path",
        bpo::value(&opt_model_label_path),
        "Path to the yamnet/classification label file for interpreting the output tensor\n");

    // In classification mode, this should be the robot URL where the
    // generated configuration is currently running.
    boost::optional<std::string> opt_robot_host;
    options_desc.add_options()(
        "robot-host",
        bpo::value(&opt_robot_host),
        "Hostname of robot (e.g. foobar.zvzzzvzzvz.viam.cloud), including optional port\n");

    // In classification mode, this should be the api key for the robot
    // where the generated configuration is currently running.
    boost::optional<std::string> opt_api_key;
    options_desc.add_options()("robot-api-key",
                               bpo::value(&opt_api_key),
                               "API key for accessing the robot running at `--robot-host`\n");

    // In classification mode, this should be the api key id for the robot
    // where the generated configuration is currently running.
    boost::optional<std::string> opt_api_key_id;
    options_desc.add_options()("robot-api-key-id",
                               bpo::value(&opt_api_key_id),
                               "API key id for accessing the robot running at `--robot-host`\n");

    // Parse the command line, and print the options summary if the user requested `--help`.
    bpo::variables_map option_variables;
    bpo::store(bpo::parse_command_line(argc, argv, options_desc), option_variables);
    bpo::notify(option_variables);
    if (option_variables.count("help")) {
        std::cout << argv[0] << options_desc << std::endl;
        return EXIT_SUCCESS;
    }

    if (opt_generating) {
        // Validate that we have the right options for generation.
        if (opt_robot_host || opt_api_key || opt_api_key_id) {
            VIAM_SDK_LOG(error)
                << "With `--generate`, do not provide `--robot-{host,api-key,api-key-id}`";
            return EXIT_FAILURE;
        }

        if (!opt_model_path) {
            VIAM_SDK_LOG(error) << "With `--generate`, a `--model-path` is required";
            return EXIT_FAILURE;
        }

        const bf::path model_path(opt_model_path.get());
        if (!bf::is_regular_file(model_path)) {
            VIAM_SDK_LOG(error) << "The path `" << model_path.c_str()
                                << "` provided for `--model-path` is not an existing regular file";
            return EXIT_FAILURE;
        }

        if (!opt_tflite_module_path) {
            VIAM_SDK_LOG(error) << "With `--generate`, a `--tflite-module-path` is required";
            return EXIT_FAILURE;
        }

        const bf::path tflite_module_path(opt_tflite_module_path.get());
        if (!bf::is_regular_file(tflite_module_path)) {
            VIAM_SDK_LOG(error)
                << "The path `" << tflite_module_path.c_str()
                << "` provided for `--tflite-module-path` is not an existing regular file";
            return EXIT_FAILURE;
        }

        // Emit the interpolated robot configuration. We don't provide
        // any summary information or further instructions because it
        // is nice to be able to feed the results directly to `pbcopy`
        // or similar.
        const auto config = boost::format(kRobotConfigTemplate) % bf::absolute(model_path).c_str() %
                            bf::absolute(tflite_module_path).c_str();

        VIAM_SDK_LOG(info) << config << std::endl;

    } else {
        // Validate that we have the right options for classification mode.
        if (opt_model_path || opt_tflite_module_path) {
            VIAM_SDK_LOG(error) << "Without `--generate`, do not provide `--*path*` arguments";
            return EXIT_FAILURE;
        }

        if (!opt_robot_host) {
            VIAM_SDK_LOG(error)
                << "The `--robot-host` argument is required when connecting to a robot";
            return EXIT_FAILURE;
        }

        if (!opt_api_key || !opt_api_key_id) {
            VIAM_SDK_LOG(error)
                << "The `--robot-api-key` and the `--robot-api-key-id` argument are "
                   "required when connecting to a robot";
            return EXIT_FAILURE;
        }

        // Connect to the robot at the given URL with the provided
        // secret. Please see other examples for more details on
        // connecting to robots with the C++ SDK.
        viam::sdk::DialOptions dial_options;
        dial_options.auth_entity = opt_api_key_id.get();
        dial_options.credentials = viam::sdk::Credentials("api-key", opt_api_key.get());

        auto robot = vsdk::RobotClient::at_address(
            opt_robot_host.get(), std::chrono::seconds{0}, dial_options);

        // Obtain a handle to the MLModelService module on the robot. Note that the string
        // `yamnet_classification_tflite` is arbitrary. It just matches what was used to name the
        // service we created in the template.
        auto yamnet_service =
            robot->resource_by_name<vsdk::MLModelService>("yamnet_classification_tflite");
        if (!yamnet_service) {
            VIAM_SDK_LOG(error)
                << " did not find the `yamnet_classification_tflite` resource, cannot "
                   "continue";
            return EXIT_FAILURE;
        }

        // Create two sample signals that match the input requirements of the yamnet model. The
        // first signal is just silence, and the second is just noise.
        const std::vector<float> silence(15600, 0.0);
        const std::vector<float> noise = [&silence]() {
            std::vector<float> temp{silence};
            std::random_device seed;
            std::uniform_real_distribution<float> dist{-1.0, 1.0};
            std::generate(begin(temp), end(temp), [&dist, e = std::mt19937(seed())]() mutable {
                return dist(e);
            });
            return temp;
        }();

        // We intend to classify the `noise` signal. Swap which line is commented
        // if you would like to classify the `silence` signal instead.
        const auto& samples = noise;
        // const auto& samples = silence;

        // Populate an input tensor named `sample` as a `tensor_view`
        // over the sample data. This does not copy the data, so the
        // inputs tensor must not outlive the sample data. Note that
        // the name `sample` is per the `tensor_name_remappings` in
        // the robot configuration.
        vsdk::MLModelService::named_tensor_views inputs;
        inputs.emplace("sample",
                       vsdk::MLModelService::make_tensor_view(
                           samples.data(), samples.size(), {samples.size()}));

        // Invoke the `infer` method of the MLModelService and provide
        // it with the input tensors. The response tensors are held in
        // `result`. Note that any tensors contained in `result` are
        // views over opaque data owned by `result`. Aliases of tensor
        // data obtained through `result` must not outlive the `result` object.
        auto result = yamnet_service->infer(inputs);

        // Look up the expected output tensor by name, and fail if we
        // didn't get the expected response. Note again that the name
        // `categories` is per the `tensor_name_remappings` section of
        // the robot configuration.
        auto categories = result->find("categories");
        if (categories == result->end()) {
            VIAM_SDK_LOG(error) << "A `categories` tensor was not returned";
            return EXIT_FAILURE;
        }

        // Each value in the map pointed to by `result` is a variant
        // over different tensor types. We must know what type we
        // expect based on the model in use. In this case, we know
        // that yamnet returns a vector of floats, so we can directly
        // query the `categories` result to see if it is actually a
        // tensor view of floats. If it is, great. Otherwise, error
        // out.
        const auto* const categories_float =
            boost::get<vsdk::MLModelService::tensor_view<float>>(&categories->second);
        if (!categories_float) {
            VIAM_SDK_LOG(error)
                << "A `categories` tensor was returned, but it was not of type `float`";
            return EXIT_FAILURE;
        }

        // If no label path was provided, just dump the raw tensor
        // results. Otherwise, we do have labels, so pick the top five
        // results and print out the label and score.
        if (!opt_model_label_path) {
            for (const auto& val : *categories_float) {
                VIAM_SDK_LOG(info) << val;
            }
        } else {
            // Ensure that the label path is something we can actually read from.
            const bf::path model_label_path(opt_model_label_path.get());
            if (!bf::is_regular_file(model_label_path)) {
                VIAM_SDK_LOG(error)
                    << "The path `" << model_label_path.c_str()
                    << "` provided for `--model-label-path` is not an existing regular file";
                return EXIT_FAILURE;
            }

            // Open the labels file, or bail.
            std::ifstream labels_stream(model_label_path.c_str());
            if (!labels_stream) {
                VIAM_SDK_LOG(error)
                    << "Unable to open label path `" << model_label_path.c_str() << "`";
                return EXIT_FAILURE;
            }

            // Read the file line-by-line into a vector of strings.
            std::string label;
            std::vector<std::string> labels;
            while (std::getline(labels_stream, label)) {
                labels.emplace_back(std::move(label));
            }

            // If the tensor size doesn't match the labels file size, then the labels file
            // is probably incorrect.
            if (categories_float->size() != labels.size()) {
                VIAM_SDK_LOG(error)
                    << "Size mismatch between category scores and label files" << std::endl;
                return EXIT_FAILURE;
            }

            // Associate labels and scores and then sort on score.
            struct scored_label {
                const std::string* label;
                float score;
            };

            std::vector<scored_label> scored_labels;
            for (size_t i = 0; i != labels.size(); ++i) {
                scored_labels.push_back({&labels[i], (*categories_float)[i]});
            }

            std::sort(begin(scored_labels), end(scored_labels), [](const auto& l, const auto& r) {
                return l.score > r.score;
            });

            // Print out the top 5 (or fewer) label/score pairs.
            for (size_t i = 0; i != std::min(5UL, scored_labels.size()); ++i) {
                // TODO: Avoid hardcoding the width here.
                VIAM_SDK_LOG(info) << boost::format("%1%: %2% %|40t|%3%\n") % i %
                                          *scored_labels[i].label % scored_labels[i].score;
            }
        }

        // Run 100 rounds of inference, accumulate some descriptive
        // statistics, and report them.
        VIAM_SDK_LOG(info) << "\nMeasuring inference latency ...";
        bacc::accumulator_set<double, bacc::stats<bacc::tag::mean, bacc::tag::moment<2>>>
            accumulator;

        for (std::size_t i = 0; i != 100; ++i) {
            const auto start = std::chrono::steady_clock::now();
            static_cast<void>(yamnet_service->infer(inputs));
            const auto finish = std::chrono::steady_clock::now();
            const std::chrono::duration<double> elapsed = finish - start;
            accumulator(elapsed.count());
        }

        VIAM_SDK_LOG(info) << "Inference latency (seconds), Mean: " << bacc::mean(accumulator);
        VIAM_SDK_LOG(info) << "Inference latency (seconds), Var : " << bacc::moment<2>(accumulator);

        return EXIT_SUCCESS;
    }
} catch (const std::exception& ex) {
    std::cerr << argv[0] << ": "
              << "Failed: a std::exception was thrown: `" << ex.what() << "``\n";
    return EXIT_FAILURE;
} catch (...) {
    std::cerr << argv[0] << ": "
              << "Failed: an unknown exception was thrown\n";
    return EXIT_FAILURE;
}
