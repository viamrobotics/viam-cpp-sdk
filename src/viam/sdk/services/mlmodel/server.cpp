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

#include <cstdint>
#include <type_traits>
#include <viam/sdk/services/mlmodel/server.hpp>

#include <stack>
#include <vector>

#include <boost/variant/get.hpp>

#include <viam/sdk/rpc/server.hpp>
#include <viam/sdk/services/mlmodel/mlmodel.hpp>

namespace viam {
namespace sdk {

namespace {

// A tricky little bit of work to serialize floating point tensors to
// a ListValue of ListValue of ... ListValue of Value objects holding
// doubles, without recursion.
//
// This could probably be cleaned up a little more. In particular,
// it'd be nice if we didn't need the special case for bailing out and
// just naturally fell out of the loop with the top level ListValue in
// place.
class tensor_to_pb_value_visitor : public boost::static_visitor<::grpc::Status> {
   public:
    explicit tensor_to_pb_value_visitor(::google::protobuf::Value* value)
        : value_{std::move(value)} {}

    ::grpc::Status operator()(
        const typename MLModelService::tensor_view<float>::type& tensor) const {
        return tensor_to_pb_value_<float>(tensor);
    }

    ::grpc::Status operator()(
        const typename MLModelService::tensor_view<double>::type& tensor) const {
        return tensor_to_pb_value_<double>(tensor);
    }

    template <typename T>
    ::grpc::Status operator()(const T&) const {
        return {::grpc::UNIMPLEMENTED, "Only float, double, and uint8_t tensors may be serialized"};
    }

   private:
    template <typename T>
    ::grpc::Status tensor_to_pb_value_(
        const typename MLModelService::tensor_view<T>::type& tensor) const {

        // TODO: Generalize to support serializing uint8_t, which is the other
        // case we can handle.

        auto begin = tensor.shape().cbegin();
        if (begin != tensor.shape().cend()) {
            std::stack<std::unique_ptr<::google::protobuf::ListValue>> lvs;
            std::vector<size_t> ixes;
            ixes.reserve(tensor.shape().size());
            while (true) {
                if (ixes.size() == tensor.shape().size()) {
                    // The base case: working over the last
                    // index. Create Value objects holding doubles for
                    // each value in the range of the last index.
                    for (; ixes.back() != *tensor.shape().rbegin(); ++ixes.back()) {
                        auto new_value = std::make_unique<::google::protobuf::Value>();
                        new_value->set_number_value(tensor.element(ixes.begin(), ixes.end()));
                        lvs.top()->mutable_values()->AddAllocated(new_value.release());
                    }
                }
                if (ixes.empty() || (ixes.back() < tensor.shape()[ixes.size() - 1])) {
                    // The "recursive" step where we make a new list and "descend".
                    lvs.emplace(std::make_unique<::google::protobuf::ListValue>());
                    ixes.push_back(0);
                } else if (ixes.size() > 1) {
                    // The step-out case where we have exhausted a
                    // stride. Wrap up the list we created in a value node
                    // and unwind to the next set of values one level up.
                    auto list_value = std::make_unique<::google::protobuf::Value>();
                    list_value->set_allocated_list_value(lvs.top().release());
                    lvs.pop();
                    lvs.top()->mutable_values()->AddAllocated(list_value.release());
                    ixes.pop_back();
                    ++ixes.back();
                } else {
                    // If we can't recur, and we can't unwind, then we
                    // must be done. We must break so that we don't throw
                    // away our result. I'd like to find a way to make
                    // that happen more naturally.
                    break;
                }
            }
            value_->set_allocated_list_value(lvs.top().release());
            lvs.pop();
        }

        return ::grpc::Status();
    }

    ::google::protobuf::Value* value_;
    };

::grpc::Status to_infer_request(const MLModelService::tensor_info& tensor_info,
                                const ::google::protobuf::Value& pb,
                                MLModelService::infer_request* infer_request) {
    return ::grpc::Status();
}

::grpc::Status from_infer_response(
    const MLModelService::tensor_info& tensor_info,
    const MLModelService::tensor_views& tensor,
    decltype(std::declval<::google::protobuf::Struct>().mutable_fields()) fields) {
    auto emplace_result = fields->try_emplace(tensor_info.name);
    // TODO: check result of try_emplace
    return boost::apply_visitor(tensor_to_pb_value_visitor{&emplace_result.first->second}, tensor);
}

}  // namespace

MLModelServiceServer::MLModelServiceServer()
    : MLModelServiceServer(std::make_shared<ResourceManager>()) {}

MLModelServiceServer::MLModelServiceServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(std::move(manager)) {}

void MLModelServiceServer::register_server(std::shared_ptr<Server> server) {
    server->register_service(this);
}

::grpc::Status MLModelServiceServer::Infer(
    ::grpc::ServerContext* context,
    const ::viam::service::mlmodel::v1::InferRequest* request,
    ::viam::service::mlmodel::v1::InferResponse* response) {
    if (!request) {
        return {::grpc::StatusCode::INVALID_ARGUMENT, "Called [Infer] without a request"};
    };

    std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());

    if (!rb) {
        return {::grpc::UNKNOWN, "resource not found: " + request->name()};
    }

    std::shared_ptr<MLModelService> mlms = std::dynamic_pointer_cast<MLModelService>(rb);

    // TODO: Caching for metadata. It is a complex structure with
    // dynamic allocations, so ideally we don't need to make a copy
    // for every inference. Unfortunately, we can't make the
    // `metadata` method in the base class return a `const&` because
    // on the client side it needs to synthesize a new object to
    // return on each call.
    const auto md = mlms->metadata();

    if (!request->has_input_data()) {
        return {::grpc::StatusCode::INVALID_ARGUMENT, "Called [Infer] with no input data"};
    }
    const auto& pb_input_data_fields = request->input_data().fields();

    MLModelService::infer_request infer_request;

    for (const auto& input : md.inputs) {
        const auto where = pb_input_data_fields.find(input.name);
        if (where == pb_input_data_fields.end()) {
            std::stringstream message;
            message << "Call to [Infer] missing metadata-specified input tensor '" << input.name
                    << "'";
            return {::grpc::StatusCode::INVALID_ARGUMENT, message.str()};
        }

        auto status = to_infer_request(input, where->second, &infer_request);

        if (!status.ok()) {
            return status;
        }
    }

    // TODO: Should we handle exceptions here? Or is it ok to let them
    // bubble up and have a higher layer deal with it?
    const auto infer_result = mlms->infer(infer_request);

    auto& pb_output_data_fields = *(response->mutable_output_data()->mutable_fields());
    for (const auto& output : md.outputs) {
        const auto where = infer_result.second.find(output.name);
        if (where == infer_result.second.end()) {
            std::stringstream message;
            message << "MLModelService::infer results missing metadata-mandated output tensor'"
                    << output.name << "' for [Infer] invocation";
            return {::grpc::StatusCode::INTERNAL, message.str()};
        }

        auto status = from_infer_response(output, where->second, &pb_output_data_fields);

        if (!status.ok()) {
            return status;
        }
    }

    return ::grpc::Status();
}

::grpc::Status MLModelServiceServer::Metadata(
    ::grpc::ServerContext* context,
    const ::viam::service::mlmodel::v1::MetadataRequest* request,
    ::viam::service::mlmodel::v1::MetadataResponse* response) {
    if (!request) {
        return {::grpc::StatusCode::INVALID_ARGUMENT, "Called [Metadata] without a request"};
    };

    std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return {grpc::UNKNOWN, "resource not found: " + request->name()};
    }

    std::shared_ptr<MLModelService> mlms = std::dynamic_pointer_cast<MLModelService>(rb);
    auto result = mlms->metadata();

    auto& metadata_pb = *response->mutable_metadata();
    *metadata_pb.mutable_name() = std::move(result.name);
    *metadata_pb.mutable_type() = std::move(result.type);
    *metadata_pb.mutable_description() = std::move(result.description);

    const auto pack_tensor_info = [](auto& target,
                                     const std::vector<MLModelService::tensor_info>& source) {
        target.Reserve(source.size());
        for (auto&& s : source) {
            auto& new_entry = *target.Add();
            *new_entry.mutable_name() = std::move(s.name);
            *new_entry.mutable_description() = std::move(s.description);
            *new_entry.mutable_data_type() = std::move(s.data_type);
            auto& shape = *new_entry.mutable_shape();
            shape.Reserve(s.shape.size());
            shape.Assign(s.shape.begin(), s.shape.end());
            auto& associated_files = *new_entry.mutable_associated_files();
            associated_files.Reserve(s.associated_files.size());
            for (auto&& af : s.associated_files) {
                auto& new_af = *associated_files.Add();
                *new_af.mutable_name() = std::move(af.name);
                *new_af.mutable_description() = std::move(af.description);
                switch (af.label_type) {
                    case MLModelService::tensor_info::file::k_type_tensor_value:
                        new_af.set_label_type(
                            ::viam::service::mlmodel::v1::LABEL_TYPE_TENSOR_VALUE);
                        break;
                    case MLModelService::tensor_info::file::k_type_tensor_axis:
                        new_af.set_label_type(::viam::service::mlmodel::v1::LABEL_TYPE_TENSOR_AXIS);
                        break;
                    default:
                        break;
                        // XXX ACM TODO
                }
            }
            // XXX ACM TODO: `extra` field
        }
    };

    pack_tensor_info(*metadata_pb.mutable_input_info(), result.inputs);
    pack_tensor_info(*metadata_pb.mutable_output_info(), result.outputs);

    return ::grpc::Status();
}

}  // namespace sdk
}  // namespace viam
