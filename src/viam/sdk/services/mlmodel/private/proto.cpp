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

#include <memory>
#include <stack>
#include <stdexcept>
#include <type_traits>
#include <utility>

#include <absl/strings/escaping.h>
#include <boost/variant/get.hpp>

namespace viam {
namespace sdk {
namespace mlmodel_details {

namespace {

namespace gp = ::google::protobuf;

template <typename T>
::grpc::Status pb_value_to_tensor_t(const MLModelService::tensor_info& tensor_info,
                                    const gp::Value& pb,
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
    std::stack<const gp::Value*> vs{{&pb}};
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
        } else if (std::is_same<T, std::uint8_t>::value &&
                   (vs.top()->kind_case() == gp::Value::kStringValue)) {
            const auto& sv = vs.top()->string_value();
            std::string decoded;
            if (!absl::Base64Unescape(sv, &decoded)) {
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
        } else if (vs.top()->kind_case() == gp::Value::kNumberValue) {
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
            // We will try to fix up a flat buffer when there should
            // have been structure, because RDK does that for vision
            // service inputs, but we won't get into the weeds of
            // trying to make sense of any more complex dimensional
            // mixups.
            if (shape.size() > 1) {
                return {grpc::INTERNAL, make_error()};
            }

            // If there are multiple free extents, the situation is
            // hopeless.
            const auto num_unbounded =
                std::count(tensor_info.shape.begin(), tensor_info.shape.end(), -1);
            if (num_unbounded > 1) {
                return {grpc::INTERNAL, make_error()};
            }

            // Compute divisor, and invert if it is negative.
            auto ti_product = std::accumulate(
                tensor_info.shape.begin(), tensor_info.shape.end(), 1, std::multiplies<int>());
            if (num_unbounded != 0) {
                ti_product = -ti_product;
            }

            // If we have no unbounded extents, then we can reshape as
            // long as the total number of elements in the flat buffer
            // is equal to the product of the expected extents. If we
            // do have an unbounded extent, then it only works if the
            // number of elements in the flat buffer is a multiple of
            // the product of the expected extents, in which case we
            // should replace the unknown.
            if (num_unbounded == 0) {
                if (shape[0] != static_cast<size_t>(ti_product)) {
                    return {grpc::INTERNAL, make_error()};
                }
                shape.clear();
                std::transform(tensor_info.shape.begin(),
                               tensor_info.shape.end(),
                               std::back_inserter(shape),
                               [](int s) { return static_cast<size_t>(s); });
            } else {
                if (shape[0] % ti_product != 0) {
                    return {grpc::INTERNAL, make_error()};
                }
                const auto extent = shape[0] / ti_product;
                shape.clear();
                std::transform(
                    tensor_info.shape.begin(),
                    tensor_info.shape.end(),
                    std::back_inserter(shape),
                    [&extent](int s) { return (s == -1) ? extent : static_cast<size_t>(s); });
            }
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
    explicit tensor_to_pb_value_visitor(gp::Value* value) : value_{std::move(value)} {}

    // A tricky little bit of work to serialize floating point tensors to
    // a ListValue of ListValue of ... ListValue of Value objects holding
    // doubles, without recursion.
    //
    // This could probably be cleaned up a little more. In particular,
    // it'd be nice if we didn't need the special case for bailing out and
    // just naturally fell out of the loop with the top level ListValue in
    // place.
    template <typename T>
    ::grpc::Status operator()(const T& tensor) const {
        static_assert(!std::is_same<T, std::uint8_t>::value);
        if (tensor.shape().empty()) {
            return ::grpc::Status();
        }

        std::stack<std::unique_ptr<gp::ListValue>> lvs;
        std::vector<size_t> ixes;
        ixes.reserve(tensor.shape().size());
        while (true) {
            if (ixes.size() == tensor.shape().size()) {
                // The base case: working over the last index. Create
                // Value objects holding doubles for each value in the
                // range of the last index.
                for (; ixes.back() != *tensor.shape().rbegin(); ++ixes.back()) {
                    auto new_value = std::make_unique<gp::Value>();
                    new_value->set_number_value(tensor.element(ixes.begin(), ixes.end()));
                    lvs.top()->mutable_values()->AddAllocated(new_value.release());
                }
            }

            if (ixes.empty() || (ixes.back() < tensor.shape()[ixes.size() - 1])) {
                // The "recursive" step where we make a new list and "descend".
                ixes.push_back(0);
                lvs.emplace(std::make_unique<gp::ListValue>());
            } else if (ixes.size() > 1) {
                // The step-out case where we have exhausted a
                // stride. Wrap up the list we created in a value node
                // and unwind to the next set of values one level up.
                auto list_value = std::make_unique<gp::Value>();
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
    ::grpc::Status operator()(const MLModelService::tensor_view<std::uint8_t>& tensor) const {
        if (tensor.shape().empty()) {
            return ::grpc::Status();
        }

        std::stack<std::unique_ptr<gp::ListValue>> lvs;
        std::vector<size_t> ixes;
        ixes.reserve(tensor.shape().size());

        while (true) {
            if (ixes.size() == tensor.shape().size()) {
                // Base64 encode the entire last dimension stride into a string.
                const auto* const bytes_begin = &tensor.element(ixes.begin(), ixes.end());
                ixes.back() = tensor.shape().back();
                std::string encoded;
                absl::Base64Escape({reinterpret_cast<const char*>(bytes_begin), ixes.back()},
                                   &encoded);
                if (lvs.empty()) {
                    value_->set_string_value(std::move(encoded));
                } else {
                    auto new_value = std::make_unique<gp::Value>();
                    new_value->set_string_value(std::move(encoded));
                    lvs.top()->mutable_values()->AddAllocated(new_value.release());
                }
            }

            if (ixes.empty() || (ixes.back() < tensor.shape()[ixes.size() - 1])) {
                // The "recursive" step where we make a new list and "descend".
                ixes.push_back(0);
                if ((tensor.shape().size() > 1) && (ixes.size() < tensor.shape().size())) {
                    lvs.emplace(std::make_unique<gp::ListValue>());
                }
            } else if (ixes.size() > 1) {
                // The step-out case where we have exhausted a
                // stride. Wrap up the list we created in a value node
                // and unwind to the next set of values one level
                // up. We must create a special case for the deepest
                // index since they are managed differently.
                if (ixes.size() != tensor.shape().size()) {
                    auto list_value = std::make_unique<gp::Value>();
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

    gp::Value* value_;
};

class copy_sdk_tensor_to_api_tensor_visitor : public boost::static_visitor<void> {
   public:
    explicit copy_sdk_tensor_to_api_tensor_visitor(::viam::service::mlmodel::v1::FlatTensor* target)
        : target_(target) {}

    template <typename T>
    void operator()(const T& tensor_view) const {
        target_->mutable_shape()->Clear();
        target_->mutable_shape()->Add(tensor_view.shape().begin(), tensor_view.shape().end());
        dispatch_(tensor_view);
    }

   private:
    void dispatch_(const MLModelService::tensor_view<std::int8_t>& t) const {
        target_->mutable_int8_tensor()->mutable_data()->clear();
        target_->mutable_int8_tensor()->mutable_data()->assign(t.begin(), t.end());
    }

    void dispatch_(const MLModelService::tensor_view<std::uint8_t>& t) const {
        target_->mutable_uint8_tensor()->mutable_data()->clear();
        target_->mutable_uint8_tensor()->mutable_data()->assign(t.begin(), t.end());
    }

    void dispatch_(const MLModelService::tensor_view<std::int16_t>& t) const {
        // TODO: LE specific. More work is needed here to support BE,
        // if that is ever relevant. Specifically, on a BE platform
        // after doing the assignment, each hi/low pair of each uint32
        // must be swapped.
        //
        // Unfortuanate: we need to do the zero filling for all bytes.
        //
        // Tricky: The last zero is important if there is an odd
        // number of shorts.
        const int num32s =
            static_cast<int>((t.size() + 1) * sizeof(std::int16_t) / sizeof(std::uint32_t));
        target_->mutable_int16_tensor()->mutable_data()->Clear();
        target_->mutable_int16_tensor()->mutable_data()->Resize(num32s, 0);
        std::memcpy(target_->mutable_int16_tensor()->mutable_data()->mutable_data(),
                    t.begin(),
                    t.size() * sizeof(std::int16_t));
    }

    void dispatch_(const MLModelService::tensor_view<std::uint16_t>& t) const {
        // TODO: LE specific. See above comment.
        const int num32s =
            static_cast<int>((t.size() + 1) * sizeof(std::uint16_t) / sizeof(std::uint32_t));
        target_->mutable_uint16_tensor()->mutable_data()->Clear();
        target_->mutable_uint16_tensor()->mutable_data()->Resize(num32s, 0);
        std::memcpy(target_->mutable_uint16_tensor()->mutable_data()->mutable_data(),
                    t.begin(),
                    t.size() * sizeof(std::uint16_t));
    }

    void dispatch_(const MLModelService::tensor_view<std::int32_t>& t) const {
        target_->mutable_int32_tensor()->mutable_data()->Clear();
        target_->mutable_int32_tensor()->mutable_data()->Add(t.begin(), t.end());
    }

    void dispatch_(const MLModelService::tensor_view<std::uint32_t>& t) const {
        target_->mutable_uint32_tensor()->mutable_data()->Clear();
        target_->mutable_uint32_tensor()->mutable_data()->Add(t.begin(), t.end());
    }

    void dispatch_(const MLModelService::tensor_view<std::int64_t>& t) const {
        target_->mutable_int64_tensor()->mutable_data()->Clear();
        target_->mutable_int64_tensor()->mutable_data()->Add(t.begin(), t.end());
    }

    void dispatch_(const MLModelService::tensor_view<std::uint64_t>& t) const {
        target_->mutable_uint64_tensor()->mutable_data()->Clear();
        target_->mutable_uint64_tensor()->mutable_data()->Add(t.begin(), t.end());
    }

    void dispatch_(const MLModelService::tensor_view<float>& t) const {
        target_->mutable_float_tensor()->mutable_data()->Clear();
        target_->mutable_float_tensor()->mutable_data()->Add(t.begin(), t.end());
    }

    void dispatch_(const MLModelService::tensor_view<double>& t) const {
        target_->mutable_double_tensor()->mutable_data()->Clear();
        target_->mutable_double_tensor()->mutable_data()->Add(t.begin(), t.end());
    }

    ::viam::service::mlmodel::v1::FlatTensor* target_;
};

template <typename T>
MLModelService::tensor_views make_sdk_tensor_from_api_tensor_t(const T* data,
                                                               std::size_t size,
                                                               std::vector<std::size_t>&& shape,
                                                               tensor_storage* ts) {
    if (!data || (size == 0) || shape.empty()) {
        std::ostringstream message;
        message << "Empty or zero length data or shape";
        throw std::invalid_argument(message.str());
    }

    if (ts) {
        auto& storage_variant = *ts->emplace(ts->end(), std::vector<T>{});
        auto& storage = boost::get<std::vector<T>>(storage_variant);
        storage.reserve(size);
        storage.assign(data, data + size);
        data = storage.data();
    }

    // Figure out how many elements we ought to have per the provided
    // shape information, checking for overflow along the way.
    size_t shape_accum = 1;
    for (const auto& s : shape) {
        const auto next_shape_accum = shape_accum * s;
        if (next_shape_accum < shape_accum) {
            std::ostringstream message;
            // TODO: Provide the shape
            message << "Provided shape information exceeds bounds of size_t when linearized";
            throw std::length_error(message.str());
        }
        shape_accum = next_shape_accum;
    }

    // We need to handle the special case of an odd number of 16-bit
    // elements because that will arrive appearing to have one more
    // element than the shape would indicate, and we don't want to
    // fail that case below.
    constexpr bool is16bit =
        std::is_same<T, std::int16_t>::value || std::is_same<T, std::uint16_t>::value;
    if (is16bit && (shape_accum == (size - 1))) {
        size -= 1;
    }

    if (size != shape_accum) {
        std::ostringstream message;
        // TODO: Provide the shape and details
        message << "Number of provided data elements does not match provided shape";
        throw std::invalid_argument(message.str());
    }

    return MLModelService::make_tensor_view(data, size, std::move(shape));
}

}  // namespace

::grpc::Status pb_value_to_tensor(const MLModelService::tensor_info& tensor_info,
                                  const gp::Value& pb,
                                  tensor_storage* ts,
                                  MLModelService::named_tensor_views* ntvs) {
    if (tensor_info.data_type == MLModelService::tensor_info::data_types::k_int8) {
        return pb_value_to_tensor_t<std::int8_t>(tensor_info, pb, ts, ntvs);
    }
    if (tensor_info.data_type == MLModelService::tensor_info::data_types::k_uint8) {
        return pb_value_to_tensor_t<std::uint8_t>(tensor_info, pb, ts, ntvs);
    }
    if (tensor_info.data_type == MLModelService::tensor_info::data_types::k_int16) {
        return pb_value_to_tensor_t<std::int16_t>(tensor_info, pb, ts, ntvs);
    }
    if (tensor_info.data_type == MLModelService::tensor_info::data_types::k_uint16) {
        return pb_value_to_tensor_t<std::uint16_t>(tensor_info, pb, ts, ntvs);
    }
    if (tensor_info.data_type == MLModelService::tensor_info::data_types::k_int32) {
        return pb_value_to_tensor_t<std::int32_t>(tensor_info, pb, ts, ntvs);
    }
    if (tensor_info.data_type == MLModelService::tensor_info::data_types::k_uint32) {
        return pb_value_to_tensor_t<std::uint32_t>(tensor_info, pb, ts, ntvs);
    }
    if (tensor_info.data_type == MLModelService::tensor_info::data_types::k_int64) {
        return pb_value_to_tensor_t<std::int64_t>(tensor_info, pb, ts, ntvs);
    }
    if (tensor_info.data_type == MLModelService::tensor_info::data_types::k_uint64) {
        return pb_value_to_tensor_t<std::uint64_t>(tensor_info, pb, ts, ntvs);
    }
    if (tensor_info.data_type == MLModelService::tensor_info::data_types::k_float32) {
        return pb_value_to_tensor_t<float>(tensor_info, pb, ts, ntvs);
    }
    if (tensor_info.data_type == MLModelService::tensor_info::data_types::k_float64) {
        return pb_value_to_tensor_t<double>(tensor_info, pb, ts, ntvs);
    }
    std::ostringstream message;
    message << "Called [Infer] with unsupported tensor `data_type` of `"
            << static_cast<std::underlying_type<MLModelService::tensor_info::data_types>::type>(
                   tensor_info.data_type)
            << "`";
    return {::grpc::StatusCode::INVALID_ARGUMENT, message.str()};
}

::grpc::Status tensor_to_pb_value(const MLModelService::tensor_views& tensor, gp::Value* value) {
    return boost::apply_visitor(tensor_to_pb_value_visitor{value}, tensor);
}

void copy_sdk_tensor_to_api_tensor(const MLModelService::tensor_views& source,
                                   ::viam::service::mlmodel::v1::FlatTensor* target) {
    boost::apply_visitor(copy_sdk_tensor_to_api_tensor_visitor{target}, source);
}

MLModelService::tensor_views make_sdk_tensor_from_api_tensor(
    const ::viam::service::mlmodel::v1::FlatTensor& api_tensor, tensor_storage* storage) {
    std::vector<std::size_t> shape;
    shape.assign(api_tensor.shape().begin(), api_tensor.shape().end());
    if (api_tensor.has_int8_tensor()) {
        return make_sdk_tensor_from_api_tensor_t(
            reinterpret_cast<const std::int8_t*>(api_tensor.int8_tensor().data().data()),
            api_tensor.int8_tensor().data().size(),
            std::move(shape),
            storage);
    } else if (api_tensor.has_uint8_tensor()) {
        return make_sdk_tensor_from_api_tensor_t(
            reinterpret_cast<const std::uint8_t*>(api_tensor.uint8_tensor().data().data()),
            api_tensor.uint8_tensor().data().size(),
            std::move(shape),
            storage);
    } else if (api_tensor.has_int16_tensor()) {
        // TODO: be deswizzle
        return make_sdk_tensor_from_api_tensor_t(
            reinterpret_cast<const std::int16_t*>(api_tensor.int16_tensor().data().data()),
            2 * api_tensor.int16_tensor().data().size(),
            std::move(shape),
            storage);
    } else if (api_tensor.has_uint16_tensor()) {
        // TODO: be deswizzle
        return make_sdk_tensor_from_api_tensor_t(
            reinterpret_cast<const std::uint16_t*>(api_tensor.uint16_tensor().data().data()),
            2 * api_tensor.uint16_tensor().data().size(),
            std::move(shape),
            storage);
    } else if (api_tensor.has_int32_tensor()) {
        return make_sdk_tensor_from_api_tensor_t(api_tensor.int32_tensor().data().data(),
                                                 api_tensor.int32_tensor().data().size(),
                                                 std::move(shape),
                                                 storage);
    } else if (api_tensor.has_uint32_tensor()) {
        return make_sdk_tensor_from_api_tensor_t(api_tensor.uint32_tensor().data().data(),
                                                 api_tensor.uint32_tensor().data().size(),
                                                 std::move(shape),
                                                 storage);

    } else if (api_tensor.has_int64_tensor()) {
        return make_sdk_tensor_from_api_tensor_t(api_tensor.int64_tensor().data().data(),
                                                 api_tensor.int64_tensor().data().size(),
                                                 std::move(shape),
                                                 storage);

    } else if (api_tensor.has_uint64_tensor()) {
        return make_sdk_tensor_from_api_tensor_t(api_tensor.uint64_tensor().data().data(),
                                                 api_tensor.uint64_tensor().data().size(),
                                                 std::move(shape),
                                                 storage);

    } else if (api_tensor.has_float_tensor()) {
        return make_sdk_tensor_from_api_tensor_t(api_tensor.float_tensor().data().data(),
                                                 api_tensor.float_tensor().data().size(),
                                                 std::move(shape),
                                                 storage);

    } else if (api_tensor.has_double_tensor()) {
        return make_sdk_tensor_from_api_tensor_t(api_tensor.double_tensor().data().data(),
                                                 api_tensor.double_tensor().data().size(),
                                                 std::move(shape),
                                                 storage);
    }
    throw std::invalid_argument("Unsupported tensor data type");
}

}  // namespace mlmodel_details
}  // namespace sdk
}  // namespace viam
