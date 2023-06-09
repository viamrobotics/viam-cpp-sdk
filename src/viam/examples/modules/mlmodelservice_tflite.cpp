#include <pthread.h>
#include <signal.h>

#include <fstream>
#include <mutex>
#include <sstream>
#include <stdexcept>

#include <boost/algorithm/string/join.hpp>
#include <boost/variant/get.hpp>
#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include <tensorflow/lite/c/c_api.h>

#include <viam/sdk/components/component.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/module/service.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/rpc/server.hpp>
#include <viam/sdk/services/mlmodel/mlmodel.hpp>
#include <viam/sdk/services/mlmodel/server.hpp>

namespace {

namespace vs = ::viam::sdk;

constexpr char service_name[] = "mlmodelservice_tflite";

class MLModelServiceTFLite : public vs::MLModelService {
    class write_to_tflite_tensor_visitor_;
    class read_from_tflite_tensor_visitor_;

   public:
    explicit MLModelServiceTFLite(vs::Dependencies dependencies, vs::ResourceConfig configuration)
        : MLModelService(configuration.name()) {
        std::cout << "MLModelServiceTFLite instantiated as '" << this->name() << "'" << std::endl;
        reconfigure(std::move(dependencies), std::move(configuration));
    }

    void reconfigure(vs::Dependencies dependencies, vs::ResourceConfig configuration) override {
        using std::swap;
        std::cout << "MLModelServiceTFLite: recieved `reconfigure` invocation" << std::endl;
        std::shared_ptr<state> state;
        {
            std::scoped_lock<std::mutex> lock(state_lock_);
            swap(state_, state);
        }
        try {
            state = reconfigure_(std::move(dependencies), std::move(configuration));
        } catch (...) {
            std::scoped_lock<std::mutex> lock(state_lock_);
            swap(state_, state);
            state_ready_.notify_all();
            throw;
        }
        {
            std::scoped_lock<std::mutex> lock(state_lock_);
            swap(state_, state);
        }
        state_ready_.notify_all();
    }

    std::shared_ptr<named_tensor_views> infer(const named_tensor_views& inputs) override {
        // std::cout << "MLModelServiceTFLite: recieved `infer` invocation with " << inputs.size()
        // << "inputs" << std::endl;
        std::shared_ptr<state> state;
        {
            std::unique_lock<std::mutex> lock(state_lock_);
            while (!state_) {
                state_ready_.wait(lock);
            }
            state = state_;
        }

        std::scoped_lock<std::mutex> lock(state->interpreter_mutex);

        if (inputs.empty()) {
            std::ostringstream buffer;
            buffer << service_name << ": No inputs provided for inference";
            throw std::invalid_argument(buffer.str());
        }

        for (const auto& kv : inputs) {
            const auto where = state->input_tensor_indices_by_name.find(kv.first);
            if (where == state->input_tensor_indices_by_name.end()) {
                std::ostringstream buffer;
                buffer << service_name << ": No found index for input tensor name `" << kv.first
                       << "`";
                throw std::invalid_argument(buffer.str());
            }
            auto* const tensor =
                TfLiteInterpreterGetInputTensor(state->interpreter.get(), where->second);
            if (!tensor) {
                std::ostringstream buffer;
                buffer << service_name << ": Failed obtain tflite input tensor for `" << kv.first
                       << "` (index " << where->second << ")";
                throw std::invalid_argument(buffer.str());
            }

            const auto tflite_status =
                boost::apply_visitor(write_to_tflite_tensor_visitor_(&kv.first, tensor), kv.second);

            if (tflite_status != TfLiteStatus::kTfLiteOk) {
                std::ostringstream buffer;
                buffer << service_name << ": input tensor `" << kv.first
                       << "` failed population: " << state->error_reporter_data;
                throw std::invalid_argument(buffer.str());
            }

            // std::cout << "XXX Populated input tensor " << kv.first << std::endl;
        }

        const auto tflite_status = TfLiteInterpreterInvoke(state->interpreter.get());
        if (tflite_status != TfLiteStatus::kTfLiteOk) {
            std::ostringstream buffer;
            buffer << service_name
                   << ": interpreter invocation failed: " << state->error_reporter_data;
            throw std::runtime_error(buffer.str());
        }

        struct inference_result_type {
            using tensor_storage_types =
                boost::mpl::transform_view<MLModelService::base_types,
                                           std::vector<boost::mpl::placeholders::_1>>;
            using tensor_storage =
                std::vector<boost::make_variant_over<tensor_storage_types>::type>;

            tensor_storage storage;
            named_tensor_views views;
        };

        auto inference_result = std::make_shared<inference_result_type>();

        for (const auto& output : state->metadata.outputs) {
            // std::cout << "XXX ACM ENCODING OUTPUT " << output.name << std::endl;
            const auto where = state->output_tensor_indices_by_name.find(output.name);
            if (where == state->output_tensor_indices_by_name.end()) {
                // std::cout << "XXX ACM IMPOSSABLE!" << std::endl;
                continue;  // Should be impossible
            }
            const auto* const tflite_tensor =
                TfLiteInterpreterGetOutputTensor(state->interpreter.get(), where->second);
            const auto tflite_tensor_type = TfLiteTensorType(tflite_tensor);

            switch (tflite_tensor_type) {
                case kTfLiteInt8: {
                    inference_result->storage.emplace_back(std::vector<std::int8_t>{});
                    break;
                }
                case kTfLiteUInt8: {
                    inference_result->storage.emplace_back(std::vector<std::uint8_t>{});
                    break;
                }
                case kTfLiteInt16: {
                    inference_result->storage.emplace_back(std::vector<std::int16_t>{});
                    break;
                }
                case kTfLiteUInt16: {
                    inference_result->storage.emplace_back(std::vector<std::uint16_t>{});
                    break;
                }
                case kTfLiteInt32: {
                    inference_result->storage.emplace_back(std::vector<std::int32_t>{});
                    break;
                }
                case kTfLiteUInt32: {
                    inference_result->storage.emplace_back(std::vector<std::uint32_t>{});
                    break;
                }
                case kTfLiteInt64: {
                    inference_result->storage.emplace_back(std::vector<std::int64_t>{});
                    break;
                }
                case kTfLiteUInt64: {
                    inference_result->storage.emplace_back(std::vector<std::uint64_t>{});
                    break;
                }
                case kTfLiteFloat32: {
                    inference_result->storage.emplace_back(std::vector<float>{});
                    break;
                }
                case kTfLiteFloat64: {
                    inference_result->storage.emplace_back(std::vector<double>{});
                    break;
                }
                default: {
                    std::ostringstream buffer;
                    buffer << service_name << ": Model returned unsupported tflite data type: "
                           << tflite_tensor_type;
                    throw std::invalid_argument(buffer.str());
                }
            }

            auto tensor_view =
                boost::apply_visitor(read_from_tflite_tensor_visitor_{&output, tflite_tensor},
                                     inference_result->storage.back());

            inference_result->views.emplace(output.name, std::move(tensor_view));
        }

        auto* const views = &inference_result->views;
        return {std::move(inference_result), views};
    }

    struct metadata metadata() override {
        // std::cout << "MLModelServiceTFLite: recieved `metadata` invocation" << std::endl;
        std::shared_ptr<state> state;
        {
            std::unique_lock<std::mutex> lock(state_lock_);
            while (!state_) {
                state_ready_.wait(lock);
            }
            state = state_;
        }
        return state->metadata;
    }

   private:
    struct state;

    static std::shared_ptr<state> reconfigure_(vs::Dependencies dependencies,
                                               vs::ResourceConfig configuration) {
        auto state =
            std::make_shared<struct state>(std::move(dependencies), std::move(configuration));

        const auto& attributes = state->configuration.attributes();
        auto model_path = attributes->find("model_path");
        if (model_path == attributes->end()) {
            std::ostringstream buffer;
            buffer << service_name
                   << ": Required parameter `model_path` not found in configuration";
            throw std::invalid_argument(buffer.str());
        }
        const auto* const model_path_string = model_path->second->get<std::string>();
        if (!model_path_string || model_path_string->empty()) {
            std::ostringstream buffer;
            buffer << service_name
                   << ": Required non-empty string parameter `model_path` is either not a string "
                      "or is an empty string";
            throw std::invalid_argument(buffer.str());
        }

        // Process any tensor name remappings provided in the config.
        auto remappings = attributes->find("tensor_name_remappings");
        if (remappings != attributes->end()) {
            const auto remappings_attributes = remappings->second->get<vs::AttributeMap>();
            if (!remappings_attributes) {
                std::ostringstream buffer;
                buffer
                    << service_name
                    << ": Optional parameter `tensor_name_remappings` must be a dictionary";
                throw std::invalid_argument(buffer.str());
            }

            const auto populate_remappings = [](const vs::ProtoType& source, auto& target) {
                const auto source_attributes = source.get<vs::AttributeMap>();
                if (!source_attributes) {
                    std::ostringstream buffer;
                    buffer << service_name
                           << ": Fields `inputs` and `outputs` of `tensor_name_remappings` must be dictionaries";
                    throw std::invalid_argument(buffer.str());
                }
                for (const auto& kv : *source_attributes) {
                    const auto& k = kv.first;
                    const auto* const kv_string = kv.second->get<std::string>();
                    if (!kv_string) {
                        std::ostringstream buffer;
                        buffer << service_name
                               << ": Fields `inputs` and `outputs` of `tensor_name_remappings` must "
                                  "be dictionaries with string values";
                        throw std::invalid_argument(buffer.str());
                    }
                    target[kv.first] = *kv_string;
                    std::cout << "Remapped " << kv.first << " to " << *kv_string << std::endl;
                }
            };

            const auto inputs_where = remappings_attributes->find("inputs");
            if (inputs_where != remappings_attributes->end()) {
                populate_remappings(*inputs_where->second, state->input_name_remappings);
            }
            const auto outputs_where = remappings_attributes->find("outputs");
            if (outputs_where != remappings_attributes->end()) {
                populate_remappings(*outputs_where->second, state->output_name_remappings);
            }
        }

        // The TFLite API declares that if you use
        // `TfLiteModelCreateFromFile` that the file must remain
        // unaltered during execution. That's not a guarantee I'm
        // willing to provide, so instead we read the file into a
        // buffer which we can use with `TfLiteModelCreate`. That
        // still requires that the buffer be kept valid, but that's
        // more easily done.
        std::ifstream in(*model_path_string, std::ios::in | std::ios::binary);
        if (!in) {
            std::ostringstream buffer;
            buffer << service_name << ": Failed to open file for `model_path` "
                   << model_path_string;
            throw std::invalid_argument(buffer.str());
        }
        std::ostringstream model_path_contents_stream;
        model_path_contents_stream << in.rdbuf();
        state->model_data = std::move(model_path_contents_stream.str());

        state->model.reset(TfLiteModelCreateWithErrorReporter(
            state->model_data.data(),
            state->model_data.size(),
            [](void* ud, const char* fmt, va_list args) {
                char buffer[4096];
                static_cast<void>(vsnprintf(buffer, sizeof(buffer), fmt, args));
                *reinterpret_cast<std::string*>(ud) = buffer;
            },
            &state->error_reporter_data));

        if (!state->model) {
            std::ostringstream buffer;
            buffer << service_name << ": Failed to load model from file `" << model_path_string
                   << "`: " << state->error_reporter_data;
            throw std::invalid_argument(buffer.str());
        }

        auto num_threads = attributes->find("num_threads");
        if (num_threads != attributes->end()) {
            const auto* num_threads_double = num_threads->second->get<double>();
            if (!num_threads_double || !std::isnormal(*num_threads_double) ||
                (*num_threads_double < 0) ||
                (*num_threads_double >= std::numeric_limits<std::int32_t>::max()) ||
                (std::trunc(*num_threads_double) != *num_threads_double)) {
                std::ostringstream buffer;
                buffer << service_name
                       << ": Value for field `num_threads` is not a positive integer: "
                       << *num_threads_double;
                throw std::invalid_argument(buffer.str());
            }

            state->interpreter_options.reset(TfLiteInterpreterOptionsCreate());
            TfLiteInterpreterOptionsSetNumThreads(state->interpreter_options.get(),
                                                  static_cast<int32_t>(*num_threads_double));
        }

        state->interpreter.reset(
            TfLiteInterpreterCreate(state->model.get(), state->interpreter_options.get()));
        if (!state->interpreter) {
            std::ostringstream buffer;
            buffer << service_name
                   << ": Failed to create tflite interpreter: " << state->error_reporter_data;
            throw std::runtime_error(buffer.str());
        }

        auto tfresult = TfLiteInterpreterAllocateTensors(state->interpreter.get());
        if (tfresult != kTfLiteOk) {
            std::ostringstream buffer;
            buffer << service_name << ": Failed to allocate tensors for tflite interpreter: "
                   << state->error_reporter_data;
            throw std::runtime_error(buffer.str());
        }

        decltype(state->metadata.inputs) temp_inputs;
        decltype(state->input_tensor_indices_by_name) temp_itibn;
        auto num_input_tensors = TfLiteInterpreterGetInputTensorCount(state->interpreter.get());
        for (decltype(num_input_tensors) i = 0; i != num_input_tensors; ++i) {
            const auto* const tensor = TfLiteInterpreterGetInputTensor(state->interpreter.get(), i);

            auto ndims = TfLiteTensorNumDims(tensor);
            if (ndims == -1) {
                std::ostringstream buffer;
                buffer << service_name
                       << ": Unable to determine input tensor shape at configuration time, "
                          "inference not possible";
                throw std::runtime_error(buffer.str());
            }

            MLModelService::tensor_info input_info;
            const auto* name = TfLiteTensorName(tensor);
            const auto input_name_remapping = state->input_name_remappings.find(name);
            if (input_name_remapping != state->input_name_remappings.end()) {
                input_info.name = input_name_remapping->second;
            } else {
                input_info.name = name;
            }
            input_info.data_type =
                service_data_type_from_tflite_data_type_(TfLiteTensorType(tensor));
            for (decltype(ndims) j = 0; j != ndims; ++j) {
                input_info.shape.push_back(TfLiteTensorDim(tensor, j));
                // std::cout << "XXX ACM ADDED DIM " << input_info.shape.back()
                //<< " to input tensor `" << name << "` (" << i << ")" << std::endl;
            }
            temp_itibn[input_info.name] = i;
            temp_inputs.emplace_back(std::move(input_info));
        }
        state->metadata.inputs = std::move(temp_inputs);
        state->input_tensor_indices_by_name = std::move(temp_itibn);

        // NOTE: The tflite C API docs state that information about
        // output tensors may not be available until after one round
        // of inference. We are ignoring that guidance for now per the
        // unknowns about how metadata will be handled in the future.
        decltype(state->metadata.outputs) temp_outputs;
        decltype(state->output_tensor_indices_by_name) temp_otibn;
        auto num_output_tensors = TfLiteInterpreterGetOutputTensorCount(state->interpreter.get());
        const auto* const output_tensor_ixes =
            TfLiteInterpreterOutputTensorIndices(state->interpreter.get());
        for (decltype(num_output_tensors) i = 0; i != num_output_tensors; ++i) {
            const auto* const tensor =
                TfLiteInterpreterGetOutputTensor(state->interpreter.get(), i);

            auto ndims = TfLiteTensorNumDims(tensor);
            if (ndims == -1) {
                std::ostringstream buffer;
                buffer << service_name
                       << ": Unable to determine output tensor shape at configuration time, "
                          "inference not possible";
                throw std::runtime_error(buffer.str());
            }

            MLModelService::tensor_info output_info;
            const auto* name = TfLiteTensorName(tensor);
            const auto output_name_remapping = state->output_name_remappings.find(name);
            if (output_name_remapping != state->output_name_remappings.end()) {
                output_info.name = output_name_remapping->second;
            } else {
                output_info.name = name;
            }
            output_info.data_type =
                service_data_type_from_tflite_data_type_(TfLiteTensorType(tensor));
            for (decltype(ndims) j = 0; j != ndims; ++j) {
                output_info.shape.push_back(TfLiteTensorDim(tensor, j));
                // std::cout << "XXX ACM ADDED DIM " << output_info.shape.back()
                //<< " to output tensor `" << output_info.name << "` (" << i << ", "
                //<< output_tensor_ixes[i] << ")" << std::endl;
            }
            temp_otibn[output_info.name] = i;
            temp_outputs.emplace_back(std::move(output_info));
        }
        state->metadata.outputs = std::move(temp_outputs);
        state->output_tensor_indices_by_name = std::move(temp_otibn);

        return state;
    }

    static MLModelService::tensor_info::data_types service_data_type_from_tflite_data_type_(
        TfLiteType type) {
        switch (type) {
            case kTfLiteInt8: {
                return MLModelService::tensor_info::data_types::k_int8;
            }
            case kTfLiteUInt8: {
                return MLModelService::tensor_info::data_types::k_uint8;
            }
            case kTfLiteInt16: {
                return MLModelService::tensor_info::data_types::k_int16;
            }
            case kTfLiteUInt16: {
                return MLModelService::tensor_info::data_types::k_uint16;
            }
            case kTfLiteInt32: {
                return MLModelService::tensor_info::data_types::k_int32;
            }
            case kTfLiteUInt32: {
                return MLModelService::tensor_info::data_types::k_uint32;
            }
            case kTfLiteInt64: {
                return MLModelService::tensor_info::data_types::k_int64;
            }
            case kTfLiteUInt64: {
                return MLModelService::tensor_info::data_types::k_uint64;
            }
            case kTfLiteFloat32: {
                return MLModelService::tensor_info::data_types::k_float32;
            }
            case kTfLiteFloat64: {
                return MLModelService::tensor_info::data_types::k_float64;
            }
            default: {
                std::ostringstream buffer;
                buffer << service_name << ": Model contains unsupported tflite data type" << type;
                throw std::invalid_argument(buffer.str());
            }
        }
    }

    struct state {
        explicit state(vs::Dependencies dependencies, vs::ResourceConfig configuration)
            : dependencies(std::move(dependencies)), configuration(std::move(configuration)) {}

        vs::Dependencies dependencies;
        vs::ResourceConfig configuration;

        // The configured error reporter will overwrite this string.
        std::string error_reporter_data;

        // This data must outlive any interpreters created from the model
        // we build against model data.x
        std::string model_data;

        // Technically, we don't need to keep the model after we create an interpreter,
        // but it may be useful to do so in case we ever want to pool interpreters.
        std::unique_ptr<TfLiteModel, decltype(&TfLiteModelDelete)> model{nullptr,
                                                                         &TfLiteModelDelete};

        // Similarly, keep these around for potential re-use.
        std::unique_ptr<TfLiteInterpreterOptions, decltype(&TfLiteInterpreterOptionsDelete)>
            interpreter_options{nullptr, &TfLiteInterpreterOptionsDelete};

        // Our single interpreter, and the lock protecting it.
        std::mutex interpreter_mutex;
        std::unique_ptr<TfLiteInterpreter, decltype(&TfLiteInterpreterDelete)> interpreter{
            nullptr, &TfLiteInterpreterDelete};

        struct MLModelService::metadata metadata;

        std::unordered_map<std::string, std::string> input_name_remappings;
        std::unordered_map<std::string, std::string> output_name_remappings;

        std::unordered_map<std::string, int> input_tensor_indices_by_name;
        std::unordered_map<std::string, int> output_tensor_indices_by_name;
    };

    class write_to_tflite_tensor_visitor_ : public boost::static_visitor<TfLiteStatus> {
       public:
        write_to_tflite_tensor_visitor_(const std::string* name, TfLiteTensor* tflite_tensor)
            : name_(name), tflite_tensor_(tflite_tensor) {}

        template <typename T>
        TfLiteStatus operator()(const T& mlmodel_tensor) const {
            const auto expected_size = TfLiteTensorByteSize(tflite_tensor_);
            const auto* const mlmodel_data_begin =
                reinterpret_cast<const unsigned char*>(mlmodel_tensor.data());
            const auto* const mlmodel_data_end = reinterpret_cast<const unsigned char*>(
                mlmodel_tensor.data() + mlmodel_tensor.size());
            const auto mlmodel_data_size =
                static_cast<size_t>(mlmodel_data_end - mlmodel_data_begin);
            if (expected_size != mlmodel_data_size) {
                std::ostringstream buffer;
                buffer << service_name << ": tensor `" << *name_
                       << "` was expected to have byte size " << expected_size << " but "
                       << mlmodel_data_size << " bytes were provided";
                throw std::invalid_argument(buffer.str());
            }
            return TfLiteTensorCopyFromBuffer(tflite_tensor_, mlmodel_data_begin, expected_size);
        }

       private:
        const std::string* name_;
        TfLiteTensor* tflite_tensor_;
    };

    class read_from_tflite_tensor_visitor_
        : public boost::static_visitor<MLModelService::tensor_views> {
       public:
        read_from_tflite_tensor_visitor_(const MLModelService::tensor_info* info,
                                         const TfLiteTensor* tflite_tensor)
            : info_(info), tflite_tensor_(tflite_tensor) {}

        template <typename T>
        MLModelService::tensor_views operator()(T& storage) const {
            try {
                const auto* const tensor_data = TfLiteTensorData(tflite_tensor_);
                const auto tensor_size_bytes = TfLiteTensorByteSize(tflite_tensor_);
                const auto* const tensor_data_begin =
                    reinterpret_cast<const typename T::value_type*>(tensor_data);
                const auto* const tensor_data_end =
                    tensor_data_begin + (tensor_size_bytes / sizeof(typename T::value_type));
                storage.assign(tensor_data_begin, tensor_data_end);
                std::vector<std::size_t> shape;
                // TODO: Not very clean
                for (const auto s : info_->shape) {
                    shape.push_back(static_cast<std::size_t>(s));
                }
                // std::ostringstream buffer;
                // buffer << "OUTPUT TENSOR `" << info_->name << "`"
                //        << "[";
                // for (const auto s : info_->shape) {
                //     buffer << s << ", ";
                // }
                // buffer << "] / " << storage.size() << ": ";
                // for (const auto v : storage) {
                //     buffer << v << ", ";
                // }
                // std::cout << buffer.str() << std::endl;

                return MLModelService::make_tensor_view(
                    storage.data(), storage.size(), std::move(shape));
            } catch (const std::exception& ex) {
                std::ostringstream buffer;
                buffer << service_name << ": failed to adapt tensor result: " << ex.what();
                throw std::runtime_error(buffer.str());
            }
        }

       private:
        const MLModelService::tensor_info* info_;
        const TfLiteTensor* tflite_tensor_;
    };

    std::mutex state_lock_;
    std::condition_variable state_ready_;
    std::shared_ptr<state> state_;
};

int serve(const std::string& socket_path) {
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGINT);
    sigaddset(&sigset, SIGTERM);
    pthread_sigmask(SIG_BLOCK, &sigset, NULL);

    auto module_registration = std::make_shared<vs::ModelRegistration>(
        vs::ResourceType{"MLModelServiceTFLiteModule"},
        vs::MLModelService::static_api(),
        vs::Model{"viam", "mlmodelservice", "tflite"},
        [](vs::Dependencies deps, vs::ResourceConfig config) {
            return std::make_shared<MLModelServiceTFLite>(std::move(deps), std::move(config));
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
    const std::string usage = "usage: mlmodelservice_tflite /path/to/unix/socket";

    if (argc < 2) {
        std::cout << "ERROR: insufficient arguments\n";
        std::cout << usage << "\n";
        return EXIT_FAILURE;
    }

    return serve(argv[1]);
}
