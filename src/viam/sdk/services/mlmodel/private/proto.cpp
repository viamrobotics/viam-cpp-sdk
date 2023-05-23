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

#include <viam/sdk/services/mlmodel/private/proto.hpp>

#include <stack>

#include <boost/variant/get.hpp>

namespace viam {
namespace sdk {
namespace mlmodel_details {

namespace {

template <typename T>
::grpc::Status pb_value_to_tensor_t(const MLModelService::tensor_info& tensor_info,
                                    const ::google::protobuf::Value& pb,
                                    tensor_storage* ts,
                                    MLModelService::named_tensor_views* ntvs) {
    // Stage our backing vector into `ts` and obtain a reference to it.
    auto& storage_variant = *ts->emplace(ts->end(), std::vector<T>{});
    auto& storage = boost::get<std::vector<T>>(storage_variant);

    // Seed our shape vector as starting with one unknown dimension.
    // We will populate it while walking the protobuf. Also create a
    // stack of Value pointers which we will use to manage our
    // walk. Start a depth counter so we know the index of the
    // dimension we are iterating.
    typename MLModelService::tensor_view<T>::shape_type shape{};
    std::stack<const ::google::protobuf::Value*> vs{{&pb}};
    size_t depth = 0;

    while (!vs.empty()) {
        if (!vs.top()) {
            vs.pop();
            --depth;
        } else if (vs.top()->has_list_value()) {
            const auto* lv = &vs.top()->list_value();
            const auto& children = lv->values();
            if (shape.size() == depth) {
                shape.push_back(0);
            }
            if (shape[depth] == 0) {
                shape[depth] = children.size();
            } else if (shape[depth] !=
                       static_cast<typename decltype(shape)::size_type>(children.size())) {
                std::ostringstream message;
                message << "Ragged tensor '" << tensor_info.name << "' at depth " << depth;
                return {grpc::INTERNAL, message.str()};
            }
            vs.top() = nullptr;
            ++depth;
            std::for_each(
                children.rbegin(), children.rend(), [&vs](const auto& v) { vs.push(&v); });
        } else if (std::is_same<T, std::uint8_t>::value && vs.top()->has_string_value()) {
            const auto& sv = vs.top()->string_value();
            std::string decoded;
            if (!::google::protobuf::Base64Unescape(sv, &decoded)) {
                std::ostringstream message;
                message << "Failed to Base64 decode stride at depth " << depth << "in tensor '"
                        << tensor_info.name << "'";
                return {grpc::INTERNAL, message.str()};
            }
            storage.insert(storage.end(), decoded.begin(), decoded.end());
            if (shape.size() == depth) {
                shape.push_back(0);
            }
            if (shape[depth] == 0) {
                shape[depth] = decoded.size();
            } else if (shape[depth] != decoded.size()) {
                std::ostringstream message;
                message << "Ragged tensor '" << tensor_info.name << "' at depth " << depth;
                return {grpc::INTERNAL, message.str()};
            }
            vs.pop();
        } else if (vs.top()->has_number_value()) {
            storage.push_back(vs.top()->number_value());
            vs.pop();
        } else {
            std::ostringstream message;
            message << "Unsupported Struct type '" << vs.top()->kind_case() << "' in tensor '"
                    << tensor_info.name << "'";
            return {grpc::INTERNAL, message.str()};
        }
    }

    // If we have shape information from the metadata, validate our
    // discovered shape against it. Note that we are also validating
    // the shape info in the metadata. If it isn't sane, we consider
    // all input tensors invalid.
    if (!tensor_info.shape.empty()) {
        const auto make_error =
            [&name = tensor_info.name, &expected = tensor_info.shape, &actual = shape]() {
                std::ostringstream message;
                message << "After decoding tensor '" << name
                        << "', the discovered dimensions do not match the shape metadata:";
                message << "prototype [";
                for (const auto& i : expected) {
                    message << i << ", ";
                }
                message << "], actual [";
                for (const auto& i : actual) {
                    message << i << ", ";
                }
                message << "]";
                return message.str();
            };

        if (shape.size() != tensor_info.shape.size()) {
            return {grpc::INTERNAL, make_error()};
        }
        const auto compare = [](const auto& spec, const auto& real) {
            if (spec < 0 && spec != -1) {
                return false;
            }
            if (real < 1) {
                return false;
            }
            if (spec != -1 && static_cast<decltype(real)>(spec) != real) {
                return false;
            }
            return true;
        };
        if (!std::equal(tensor_info.shape.begin(),
                        tensor_info.shape.end(),
                        shape.begin(),
                        shape.end(),
                        compare)) {
            return {grpc::INTERNAL, make_error()};
        }
    }

    // Register the tensor_view over our storage into the map of named
    // tensor views.
    ntvs->emplace(
        tensor_info.name,
        MLModelService::make_tensor_view(storage.data(), storage.size(), std::move(shape)));

    return ::grpc::Status();
}

class tensor_to_pb_value_visitor : public boost::static_visitor<::grpc::Status> {
   public:
    explicit tensor_to_pb_value_visitor(::google::protobuf::Value* value)
        : value_{std::move(value)} {}

    ::grpc::Status operator()(const MLModelService::tensor_view<std::int8_t>& tensor) const {
        return tensor_to_pb_t<std::int8_t>(tensor);
    }
    ::grpc::Status operator()(const MLModelService::tensor_view<std::uint8_t>& tensor) const {
        return tensor_to_pb_(tensor);
    }
    ::grpc::Status operator()(const MLModelService::tensor_view<std::int16_t>& tensor) const {
        return tensor_to_pb_t<std::int16_t>(tensor);
    }
    ::grpc::Status operator()(const MLModelService::tensor_view<std::uint16_t>& tensor) const {
        return tensor_to_pb_t<std::uint16_t>(tensor);
    }
    ::grpc::Status operator()(const MLModelService::tensor_view<std::int32_t>& tensor) const {
        return tensor_to_pb_t<std::int32_t>(tensor);
    }
    ::grpc::Status operator()(const MLModelService::tensor_view<std::uint32_t>& tensor) const {
        return tensor_to_pb_t<std::uint32_t>(tensor);
    }
    ::grpc::Status operator()(const MLModelService::tensor_view<std::int64_t>& tensor) const {
        return tensor_to_pb_t<std::int64_t>(tensor);
    }
    ::grpc::Status operator()(const MLModelService::tensor_view<std::uint64_t>& tensor) const {
        return tensor_to_pb_t<std::uint64_t>(tensor);
    }
    ::grpc::Status operator()(const MLModelService::tensor_view<float>& tensor) const {
        return tensor_to_pb_t<float>(tensor);
    }
    ::grpc::Status operator()(const MLModelService::tensor_view<double>& tensor) const {
        return tensor_to_pb_t<double>(tensor);
    }

   private:
    // A tricky little bit of work to serialize floating point tensors to
    // a ListValue of ListValue of ... ListValue of Value objects holding
    // doubles, without recursion.
    //
    // This could probably be cleaned up a little more. In particular,
    // it'd be nice if we didn't need the special case for bailing out and
    // just naturally fell out of the loop with the top level ListValue in
    // place.
    template <typename T>
    ::grpc::Status tensor_to_pb_t(const MLModelService::tensor_view<T>& tensor) const {
        static_assert(!std::is_same<T, std::uint8_t>::value);
        if (tensor.shape().empty()) {
            return ::grpc::Status();
        }

        std::stack<std::unique_ptr<::google::protobuf::ListValue>> lvs;
        std::vector<size_t> ixes;
        ixes.reserve(tensor.shape().size());
        while (true) {
            if (ixes.size() == tensor.shape().size()) {
                // The base case: working over the last index. Create
                // Value objects holding doubles for each value in the
                // range of the last index.
                for (; ixes.back() != *tensor.shape().rbegin(); ++ixes.back()) {
                    auto new_value = std::make_unique<::google::protobuf::Value>();
                    new_value->set_number_value(tensor.element(ixes.begin(), ixes.end()));
                    lvs.top()->mutable_values()->AddAllocated(new_value.release());
                }
            }

            if (ixes.empty() || (ixes.back() < tensor.shape()[ixes.size() - 1])) {
                // The "recursive" step where we make a new list and "descend".
                ixes.push_back(0);
                lvs.emplace(std::make_unique<::google::protobuf::ListValue>());
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
        return ::grpc::Status();
    }

    // A similar tricky bit of work, but for the special case of
    // `byte` tensors where we must honor golang protobuf convention
    // of storing byte arrays as Base64 encoded strings.
    ::grpc::Status tensor_to_pb_(const MLModelService::tensor_view<std::uint8_t>& tensor) const {
        if (tensor.shape().empty()) {
            return ::grpc::Status();
        }

        std::stack<std::unique_ptr<::google::protobuf::ListValue>> lvs;
        std::vector<size_t> ixes;
        ixes.reserve(tensor.shape().size());

        while (true) {
            if (ixes.size() == tensor.shape().size()) {
                // Base64 encode the entire last dimension stride into a string.
                const auto* const bytes_begin = &tensor.element(ixes.begin(), ixes.end());
                ixes.back() = tensor.shape().back();
                const auto* const bytes_end = bytes_begin + ixes.back();
                const google::protobuf::StringPiece bytes{
                    reinterpret_cast<const char*>(bytes_begin),
                    std::size_t(bytes_end - bytes_begin)};
                std::string encoded;
                google::protobuf::Base64Escape(bytes, &encoded);
                if (lvs.empty()) {
                    value_->set_string_value(std::move(encoded));
                } else {
                    auto new_value = std::make_unique<::google::protobuf::Value>();
                    new_value->set_string_value(std::move(encoded));
                    lvs.top()->mutable_values()->AddAllocated(new_value.release());
                }
            }

            if (ixes.empty() || (ixes.back() < tensor.shape()[ixes.size() - 1])) {
                // The "recursive" step where we make a new list and "descend".
                ixes.push_back(0);
                if ((tensor.shape().size() > 1) && (ixes.size() < tensor.shape().size())) {
                    lvs.emplace(std::make_unique<::google::protobuf::ListValue>());
                }
            } else if (ixes.size() > 1) {
                // The step-out case where we have exhausted a
                // stride. Wrap up the list we created in a value node
                // and unwind to the next set of values one level
                // up. We must create a special case for the deepest
                // index since they are managed differently.
                if (ixes.size() != tensor.shape().size()) {
                    auto list_value = std::make_unique<::google::protobuf::Value>();
                    list_value->set_allocated_list_value(lvs.top().release());
                    lvs.pop();
                    lvs.top()->mutable_values()->AddAllocated(list_value.release());
                }
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

        if (!lvs.empty()) {
            value_->set_allocated_list_value(lvs.top().release());
            lvs.pop();
        }

        return ::grpc::Status();
    }

    ::google::protobuf::Value* value_;
};

}  // namespace

::grpc::Status pb_value_to_tensor(const MLModelService::tensor_info& tensor_info,
                                  const ::google::protobuf::Value& pb,
                                  tensor_storage* ts,
                                  MLModelService::named_tensor_views* ntvs) {
    if (tensor_info.data_type == MLModelService::tensor_info::data_types::k_int8) {
        return pb_value_to_tensor_t<std::int8_t>(tensor_info, pb, ts, ntvs);
    } else if (tensor_info.data_type == MLModelService::tensor_info::data_types::k_uint8) {
        return pb_value_to_tensor_t<std::uint8_t>(tensor_info, pb, ts, ntvs);
    } else if (tensor_info.data_type == MLModelService::tensor_info::data_types::k_int16) {
        return pb_value_to_tensor_t<std::int16_t>(tensor_info, pb, ts, ntvs);
    } else if (tensor_info.data_type == MLModelService::tensor_info::data_types::k_uint16) {
        return pb_value_to_tensor_t<std::uint16_t>(tensor_info, pb, ts, ntvs);
    } else if (tensor_info.data_type == MLModelService::tensor_info::data_types::k_int32) {
        return pb_value_to_tensor_t<std::int32_t>(tensor_info, pb, ts, ntvs);
    } else if (tensor_info.data_type == MLModelService::tensor_info::data_types::k_uint32) {
        return pb_value_to_tensor_t<std::uint32_t>(tensor_info, pb, ts, ntvs);
    } else if (tensor_info.data_type == MLModelService::tensor_info::data_types::k_int64) {
        return pb_value_to_tensor_t<std::int64_t>(tensor_info, pb, ts, ntvs);
    } else if (tensor_info.data_type == MLModelService::tensor_info::data_types::k_uint64) {
        return pb_value_to_tensor_t<std::uint64_t>(tensor_info, pb, ts, ntvs);
    } else if (tensor_info.data_type == MLModelService::tensor_info::data_types::k_float32) {
        return pb_value_to_tensor_t<float>(tensor_info, pb, ts, ntvs);
    } else if (tensor_info.data_type == MLModelService::tensor_info::data_types::k_float64) {
        return pb_value_to_tensor_t<double>(tensor_info, pb, ts, ntvs);
    } else {
        std::ostringstream message;
        message << "Called [Infer] with unsupported tensor `data_type` of `"
                << static_cast<std::underlying_type<MLModelService::tensor_info::data_types>::type>(
                       tensor_info.data_type)
                << "`";
        return {::grpc::StatusCode::INVALID_ARGUMENT, message.str()};
    }
}

::grpc::Status tensor_to_pb_value(const MLModelService::tensor_views& tensor,
                                  ::google::protobuf::Value* value) {
    return boost::apply_visitor(tensor_to_pb_value_visitor{value}, tensor);
}

}  // namespace mlmodel_details
}  // namespace sdk
}  // namespace viam
