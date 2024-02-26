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

#include <condition_variable>
#include <fstream>
#include <iostream>
#include <mutex>
#include <pthread.h>
#include <signal.h>
#include <sstream>
#include <stdexcept>

#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include <tensorflow/lite/c/c_api.h>

#include <viam/sdk/components/component.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/module/service.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/reconfigurable.hpp>
#include <viam/sdk/resource/stoppable.hpp>
#include <viam/sdk/rpc/server.hpp>
#include <viam/sdk/services/mlmodel.hpp>

namespace {

namespace vsdk = ::viam::sdk;
constexpr char service_name[] = "example_mlmodelservice_tflite";

// An example MLModelService instance which runs TensorFlow Lite models.
//
// Configuration requires the following parameters:
//   -- `model_path`: An absolute filesystem path to a TensorFlow Lite model file.
//
// The following optional parameters are honored:
//   -- `num_threads`: Sets the number of threads to be used, where applicable.
//
//   -- `tensor_name_remappings`: A pair of string-string maps keyed
//      as `inputs` and `outputs`. Keys of the string-string maps are
//      tensor names per the model, and the mapped strings are the
//      names by which those tensors will be presented in the
//      metadata. This is useful if you have an upstream service
//      (e.g. the vision service) which expects to find specific
//      tensor names in the metadata in order to properly interact
//      with the model.
//
// Any additional configuration fields are ignored.
class MLModelServiceTFLite : public vsdk::MLModelService,
                             public vsdk::Stoppable,
                             public vsdk::Reconfigurable {
    class write_to_tflite_tensor_visitor_;

   public:
    explicit MLModelServiceTFLite(vsdk::Dependencies dependencies,
                                  vsdk::ResourceConfig configuration)
        : MLModelService(configuration.name()),
          state_(reconfigure_(std::move(dependencies), std::move(configuration))) {}

    ~MLModelServiceTFLite() final {
        // All invocations arrive via gRPC, so we know we are idle
        // here. It should be safe to tear down all state
        // automatically without needing to wait for anything more to
        // drain.
    }

    void stop(const vsdk::AttributeMap& extra) noexcept final {
        return stop();
    }

    /// @brief Stops the MLModelServiceTFLite from running.
    void stop() noexcept {
        using std::swap;
        try {
            const std::lock_guard<std::mutex> lock(state_lock_);
            if (!stopped_) {
                stopped_ = true;
                std::shared_ptr<state> state;
                swap(state_, state);
                state_ready_.notify_all();
            }
        } catch (...) {
        }
    }

    void reconfigure(const vsdk::Dependencies& dependencies,
                     const vsdk::ResourceConfig& configuration) final try {
        // Care needs to be taken during reconfiguration. The
        // framework does not offer protection against invocation
        // during reconfiguration. Keep all state in a shared_ptr
        // managed block, and allow client invocations to act against
        // current state while a new configuration is built, then swap
        // in the new state. State which is in use by existing
        // invocations will remain valid until the clients drain. If
        // reconfiguration fails, the component will `stop`.

        // Swap out the state_ member with nullptr. Existing
        // invocations will continue to operate against the state they
        // hold, and new invocations will block on the state becoming
        // populated.
        using std::swap;
        std::shared_ptr<state> state;
        {
            // Wait until we have a state in play, then take
            // ownership, so that we don't race with other
            // reconfigurations and so other invocations wait on a new
            // state.
            std::unique_lock<std::mutex> lock(state_lock_);
            state_ready_.wait(lock, [this]() { return (state_ != nullptr) && !stopped_; });
            check_stopped_inlock_();
            swap(state_, state);
        }

        state = reconfigure_(dependencies, configuration);

        // Reconfiguration worked: put the state in under the lock,
        // release the lock, and then notify any callers waiting on
        // reconfiguration to complete.
        {
            std::lock_guard<std::mutex> lock(state_lock_);
            check_stopped_inlock_();
            swap(state_, state);
        }
        state_ready_.notify_all();
    } catch (...) {
        // If reconfiguration fails for any reason, become stopped and rethrow.
        stop();
        throw;
    }

    std::shared_ptr<named_tensor_views> infer(const named_tensor_views& inputs,
                                              const vsdk::AttributeMap& extra) final {
        auto state = lease_state_();

        // We serialize access to the interpreter. We use a
        // unique_lock because we will move the lock into the shared
        // state that we return, allowing the higher level to effect a
        // direct copy out of the tflite buffers while the interpreter
        // is still locked.
        std::unique_lock<std::mutex> lock(state->interpreter_mutex);

        // Ensure that enough inputs were provided.
        if (inputs.size() < state->input_tensor_indices_by_name.size()) {
            std::ostringstream buffer;
            buffer << service_name << ": Too few inputs provided for inference";
            throw std::invalid_argument(buffer.str());
        }

        // Walk the inputs, and copy the data from each of the input
        // tensor views we were given into the associated tflite input
        // tensor buffer.
        for (const auto& kv : inputs) {
            const auto where = state->input_tensor_indices_by_name.find(kv.first);
            if (where == state->input_tensor_indices_by_name.end()) {
                std::ostringstream buffer;
                buffer << service_name << ": Tensor name `" << kv.first << "`"
                       << " is not a known input tensor name for the model";
                throw std::invalid_argument(buffer.str());
            }
            auto* const tensor =
                TfLiteInterpreterGetInputTensor(state->interpreter.get(), where->second);
            if (!tensor) {
                std::ostringstream buffer;
                buffer << service_name << ": Failed to obtain tflite input tensor for `" << kv.first
                       << "` (index " << where->second << ")";
                throw std::invalid_argument(buffer.str());
            }

            const auto tflite_status =
                boost::apply_visitor(write_to_tflite_tensor_visitor_(&kv.first, tensor), kv.second);

            if (tflite_status != TfLiteStatus::kTfLiteOk) {
                std::ostringstream buffer;
                buffer << service_name << ": input tensor `" << kv.first
                       << "` failed population: " << state->interpreter_error_data;
                throw std::invalid_argument(buffer.str());
            }
        }

        // Invoke the interpreter and return any failure information.
        const auto tflite_status = TfLiteInterpreterInvoke(state->interpreter.get());
        if (tflite_status != TfLiteStatus::kTfLiteOk) {
            std::ostringstream buffer;
            buffer << service_name
                   << ": interpreter invocation failed: " << state->interpreter_error_data;
            throw std::runtime_error(buffer.str());
        }

        // A local type that we will keep on the heap to hold
        // inference results until the caller is done with them. In
        // our case, the caller is MLModelServiceServer, which will
        // copy the data into the reply gRPC proto and then unwind. So
        // we can avoid copying the data by letting the views alias
        // the tensorflow tensor buffers and keep the interpreter lock
        // held until the gRPC work is done. Note that this means the
        // interpreter lock will remain held until the
        // inference_result_type object tracked by the shared pointer
        // we return is destroyed. Callers that want to make use of
        // the inference results without keeping the interpreter
        // locked would need to copy the data out of the views and
        // then release the return value.
        struct inference_result_type {
            std::shared_ptr<struct state> state;
            std::unique_lock<std::mutex> interpreter_lock;
            named_tensor_views views;
        };
        auto inference_result = std::make_shared<inference_result_type>();

        // Walk the outputs per our metadata and emplace an
        // appropriately typed tensor_view aliasing the interpreter
        // output tensor buffer into the inference results.
        for (const auto& output : state->metadata.outputs) {
            const auto where = state->output_tensor_indices_by_name.find(output.name);
            if (where == state->output_tensor_indices_by_name.end()) {
                continue;  // Should be impossible
            }
            const auto* const tflite_tensor =
                TfLiteInterpreterGetOutputTensor(state->interpreter.get(), where->second);
            inference_result->views.emplace(output.name,
                                            std::move(make_tensor_view_(output, tflite_tensor)));
        }

        // The views created in the loop above are only valid until
        // the interpreter lock is released, so we keep the lock held
        // by moving the unique_lock into the inference_result
        // object. The lock itself is only valid as long as the state
        // is alive, so we move that in too.
        inference_result->state = std::move(state);
        inference_result->interpreter_lock = std::move(lock);

        // Finally, construct an aliasing shared_ptr which appears to
        // the caller as a shared_ptr to views, but in fact manages
        // the lifetime of the inference_result. When the
        // inference_result object is destroyed, the lock will be
        // released and the next caller can invoke the interpreter.
        auto* const views = &inference_result->views;
        // NOLINTNEXTLINE(performance-move-const-arg): C++20
        return {std::move(inference_result), views};
    }

    struct metadata metadata(const vsdk::AttributeMap& extra) final {
        // Just return a copy of our metadata from leased state.
        return lease_state_()->metadata;
    }

   private:
    struct state;

    void check_stopped_inlock_() const {
        if (stopped_) {
            std::ostringstream buffer;
            buffer << service_name << ": service is stopped: ";
            throw std::runtime_error(buffer.str());
        }
    }

    std::shared_ptr<state> lease_state_() {
        // Wait for our state to be valid or stopped and then obtain a
        // shared_ptr to state if valid, incrementing the refcount, or
        // throws if the service is stopped. We don't need to deal
        // with interruption or shutdown because the gRPC layer will
        // drain requests during shutdown, so it shouldn't be possible
        // for callers to get stuck here.
        std::unique_lock<std::mutex> lock(state_lock_);
        state_ready_.wait(lock, [this]() { return (state_ != nullptr) && !stopped_; });
        check_stopped_inlock_();
        return state_;
    }

    static std::shared_ptr<state> reconfigure_(vsdk::Dependencies dependencies,
                                               vsdk::ResourceConfig configuration) {
        // The new state we will attempt to build. If this function
        // returns a valid state, it will be swapped in as the current
        // state of the service.
        auto state =
            std::make_shared<struct state>(std::move(dependencies), std::move(configuration));

        // Now we can begin parsing and validating the provided `configuration`.
        // Pull the model path out of the configuration.
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
            const auto remappings_attributes = remappings->second->get<vsdk::AttributeMap>();
            if (!remappings_attributes) {
                std::ostringstream buffer;
                buffer << service_name
                       << ": Optional parameter `tensor_name_remappings` must be a dictionary";
                throw std::invalid_argument(buffer.str());
            }

            const auto populate_remappings = [](const vsdk::ProtoType& source, auto& target) {
                const auto source_attributes = source.get<vsdk::AttributeMap>();
                if (!source_attributes) {
                    std::ostringstream buffer;
                    buffer << service_name
                           << ": Fields `inputs` and `outputs` of `tensor_name_remappings` must be "
                              "dictionaries";
                    throw std::invalid_argument(buffer.str());
                }
                for (const auto& kv : *source_attributes) {
                    const auto& k = kv.first;
                    const auto* const kv_string = kv.second->get<std::string>();
                    if (!kv_string) {
                        std::ostringstream buffer;
                        buffer
                            << service_name
                            << ": Fields `inputs` and `outputs` of `tensor_name_remappings` must "
                               "be dictionaries with string values";
                        throw std::invalid_argument(buffer.str());
                    }
                    target[kv.first] = *kv_string;
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

        // Configuration parsing / extraction is complete. Move on to
        // building the actual model with the provided information.

        // Try to load the provided `model_path`. The TFLite API
        // declares that if you use `TfLiteModelCreateFromFile` that
        // the file must remain unaltered during execution, but
        // reconfiguration might cause it to change on disk while
        // inference is in progress. Instead we read the file into a
        // buffer which we can use with `TfLiteModelCreate`. That
        // still requires that the buffer be kept valid, but that's
        // more easily done.
        const std::ifstream in(*model_path_string, std::ios::in | std::ios::binary);
        if (!in) {
            std::ostringstream buffer;
            buffer << service_name << ": Failed to open file for `model_path` "
                   << *model_path_string;
            throw std::invalid_argument(buffer.str());
        }
        std::ostringstream model_path_contents_stream;
        model_path_contents_stream << in.rdbuf();
        state->model_data = std::move(model_path_contents_stream.str());

        // Create an error reporter so that we can extract detailed
        // error information from TFLite when things go wrong. The
        // error state is protected by the interpreter lock.
        state->model.reset(TfLiteModelCreateWithErrorReporter(
            state->model_data.data(),
            state->model_data.size(),
            [](void* ud, const char* fmt, va_list args) {
                char buffer[4096];
                static_cast<void>(vsnprintf(buffer, sizeof(buffer), fmt, args));
                *reinterpret_cast<std::string*>(ud) = buffer;
            },
            &state->interpreter_error_data));

        // If we failed to create the model, return an error and
        // include the error data that tflite wrote to the error
        // reporter state.
        if (!state->model) {
            std::ostringstream buffer;
            buffer << service_name << ": Failed to load model from file `" << model_path_string
                   << "`: " << state->interpreter_error_data;
            throw std::invalid_argument(buffer.str());
        }

        // If present, extract and validate the number of threads to
        // use in the interpreter and create an interpreter options
        // object to carry that information.
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

        // Build the single interpreter.
        state->interpreter.reset(
            TfLiteInterpreterCreate(state->model.get(), state->interpreter_options.get()));
        if (!state->interpreter) {
            std::ostringstream buffer;
            buffer << service_name
                   << ": Failed to create tflite interpreter: " << state->interpreter_error_data;
            throw std::runtime_error(buffer.str());
        }

        // Have the interpreter allocate tensors for the model
        auto tfresult = TfLiteInterpreterAllocateTensors(state->interpreter.get());
        if (tfresult != kTfLiteOk) {
            std::ostringstream buffer;
            buffer << service_name << ": Failed to allocate tensors for tflite interpreter: "
                   << state->interpreter_error_data;
            throw std::runtime_error(buffer.str());
        }

        // Walk the input tensors now that they have been allocated
        // and extract information about tensor names, types, and
        // dimensions. Apply any tensor renamings per our
        // configuration. Stash the relevant data in our `metadata`
        // fields.
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
            }
            state->input_tensor_indices_by_name[input_info.name] = i;
            state->metadata.inputs.emplace_back(std::move(input_info));
        }

        // As above, but for output tensors.
        //
        // NOTE: The tflite C API docs state that information about
        // output tensors may not be available until after one round
        // of inference. We are ignoring that guidance for now per the
        // unknowns about how metadata will be handled in the future.
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
            }
            state->output_tensor_indices_by_name[output_info.name] = i;
            state->metadata.outputs.emplace_back(std::move(output_info));
        }

        return state;
    }

    // Converts from tflites type enumeration into the model service
    // type enumeration or throws if there is no such conversion.
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

    // All of the meaningful internal state of the service is held in
    // a separate state object so we can keep our current state alive
    // while building a new one during reconfiguration, and then
    // atomically swap it in on success. Existing invocations will
    // continue to work against the old state, and new invocations
    // will pick up the new state.
    struct state {
        explicit state(vsdk::Dependencies dependencies, vsdk::ResourceConfig configuration)
            : dependencies(std::move(dependencies)), configuration(std::move(configuration)) {}

        // The dependencies and configuration we were given at
        // construction / reconfiguration.
        vsdk::Dependencies dependencies;
        vsdk::ResourceConfig configuration;

        // This data must outlive any interpreters created from the
        // model we build against model data.
        std::string model_data;

        // Technically, we don't need to keep the model after we create an interpreter,
        // but it may be useful to do so in case we ever want to pool interpreters.
        std::unique_ptr<TfLiteModel, decltype(&TfLiteModelDelete)> model{nullptr,
                                                                         &TfLiteModelDelete};

        // Similarly, keep the options we built around for potential
        // re-use.
        std::unique_ptr<TfLiteInterpreterOptions, decltype(&TfLiteInterpreterOptionsDelete)>
            interpreter_options{nullptr, &TfLiteInterpreterOptionsDelete};

        // Metadata about input and output tensors that was extracted
        // during configuration. Callers need this in order to know
        // how to interact with the service.
        struct MLModelService::metadata metadata;

        // Tensor renamings as extracted from our configuration. The
        // keys are the names of the tensors per the model, the values
        // are the names of the tensors clients expect to see / use
        // (e.g. a vision service component expecting a tensor named
        // `image`).
        std::unordered_map<std::string, std::string> input_name_remappings;
        std::unordered_map<std::string, std::string> output_name_remappings;

        // Maps from string names of tensors to the numeric
        // value. Note that the keys here are the renamed tensors, if
        // applicable.
        std::unordered_map<std::string, int> input_tensor_indices_by_name;
        std::unordered_map<std::string, int> output_tensor_indices_by_name;

        // Serializes access to the interpreter and the interpreter error data.
        std::mutex interpreter_mutex;

        // The configured error reporter will overwrite this string.
        std::string interpreter_error_data;

        // The interpreter itself.
        std::unique_ptr<TfLiteInterpreter, decltype(&TfLiteInterpreterDelete)> interpreter{
            nullptr, &TfLiteInterpreterDelete};
    };

    // A visitor that can populate a TFLiteTensor given a MLModelService::tensor_view.
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

    // Creates a tensor_view which views a tflite tensor buffer. It dispatches on the
    // type and delegates to the templated version below.
    MLModelService::tensor_views make_tensor_view_(const MLModelService::tensor_info& info,
                                                   const TfLiteTensor* const tflite_tensor) {
        const auto tflite_tensor_type = TfLiteTensorType(tflite_tensor);
        switch (tflite_tensor_type) {
            case kTfLiteInt8: {
                return make_tensor_view_t_<std::int8_t>(info, tflite_tensor);
            }
            case kTfLiteUInt8: {
                return make_tensor_view_t_<std::uint8_t>(info, tflite_tensor);
            }
            case kTfLiteInt16: {
                return make_tensor_view_t_<std::int16_t>(info, tflite_tensor);
            }
            case kTfLiteUInt16: {
                return make_tensor_view_t_<std::uint16_t>(info, tflite_tensor);
            }
            case kTfLiteInt32: {
                return make_tensor_view_t_<std::int32_t>(info, tflite_tensor);
            }
            case kTfLiteUInt32: {
                return make_tensor_view_t_<std::uint32_t>(info, tflite_tensor);
            }
            case kTfLiteInt64: {
                return make_tensor_view_t_<std::int64_t>(info, tflite_tensor);
            }
            case kTfLiteUInt64: {
                return make_tensor_view_t_<std::uint64_t>(info, tflite_tensor);
            }
            case kTfLiteFloat32: {
                return make_tensor_view_t_<float>(info, tflite_tensor);
            }
            case kTfLiteFloat64: {
                return make_tensor_view_t_<double>(info, tflite_tensor);
            }
            default: {
                std::ostringstream buffer;
                buffer << service_name
                       << ": Model returned unsupported tflite data type: " << tflite_tensor_type;
                throw std::invalid_argument(buffer.str());
            }
        }
    }

    // The type specific version of the above function, it just
    // reinterpret_casts the tensor buffer into an MLModelService
    // tensor view and applies the necessary shape info.
    template <typename T>
    MLModelService::tensor_views make_tensor_view_t_(const MLModelService::tensor_info& info,
                                                     const TfLiteTensor* const tflite_tensor) {
        const auto* const tensor_data = reinterpret_cast<const T*>(TfLiteTensorData(tflite_tensor));
        const auto tensor_size_bytes = TfLiteTensorByteSize(tflite_tensor);
        const auto tensor_size_t = tensor_size_bytes / sizeof(T);
        // TODO: We are just feeding back out what we cached in the
        // metadata for shape. Should this instead be re-querying the
        // output tensor NumDims / DimN after each invocation in case
        // the shape is dynamic? The possibility of a dynamically
        // sized extent is why we represent the dimensions as signed
        // quantities in the tensor metadata. But an actual tensor has
        // a real extent. How would tflite ever communicate that to us
        // differently given that we use the same API to obtain
        // metadata as we would here?
        std::vector<std::size_t> shape;
        shape.reserve(info.shape.size());
        for (const auto s : info.shape) {
            shape.push_back(static_cast<std::size_t>(s));
        }
        return MLModelService::make_tensor_view(tensor_data, tensor_size_t, std::move(shape));
    }

    // The mutex and condition variable needed to track our state
    // across concurrent reconfiguration and invocation.
    std::mutex state_lock_;
    std::condition_variable state_ready_;
    std::shared_ptr<state> state_;
    bool stopped_ = false;
};

int serve(const std::string& socket_path) try {
    // Create a new model registration for the service.
    auto module_registration = std::make_shared<vsdk::ModelRegistration>(
        // Identify that this resource offers the MLModelService API
        vsdk::API::get<vsdk::MLModelService>(),

        // Declare a model triple for this service.
        vsdk::Model{"viam", "mlmodelservice", "example_mlmodelservice_tflite"},

        // Define the factory for instances of the resource.
        [](vsdk::Dependencies deps, vsdk::ResourceConfig config) {
            return std::make_shared<MLModelServiceTFLite>(std::move(deps), std::move(config));
        });

    // Register the newly created registration with the Registry.
    vsdk::Registry::register_model(module_registration);

    // Construct the module service and tell it where to place the socket path.
    auto module_service = std::make_shared<vsdk::ModuleService>(socket_path);

    // Add the server as providing the API and model declared in the
    // registration.
    module_service->add_model_from_registry(module_registration->api(),
                                            module_registration->model());

    // Start the module service.
    module_service->serve();

    return EXIT_SUCCESS;
} catch (const std::exception& ex) {
    std::cout << "ERROR: A std::exception was thrown from `serve`: " << ex.what() << std::endl;
    return EXIT_FAILURE;
} catch (...) {
    std::cout << "ERROR: An unknown exception was thrown from `serve`" << std::endl;
    return EXIT_FAILURE;
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
