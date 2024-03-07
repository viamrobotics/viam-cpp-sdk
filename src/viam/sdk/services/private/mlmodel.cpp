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

#include <viam/sdk/services/private/mlmodel.hpp>

#include <memory>
#include <stack>
#include <stdexcept>
#include <type_traits>
#include <utility>

#include <absl/strings/escaping.h>
#include <boost/variant/get.hpp>

#include <viam/sdk/common/exception.hpp>

namespace viam {
namespace sdk {
namespace impl {
namespace mlmodel {

namespace {

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
        throw Exception(message.str());
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
            throw Exception(message.str());
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
        throw Exception(message.str());
    }

    return MLModelService::make_tensor_view(data, size, std::move(shape));
}

}  // namespace

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
            std::size_t{2} * api_tensor.int16_tensor().data().size(),
            std::move(shape),
            storage);
    } else if (api_tensor.has_uint16_tensor()) {
        // TODO: be deswizzle
        return make_sdk_tensor_from_api_tensor_t(
            reinterpret_cast<const std::uint16_t*>(api_tensor.uint16_tensor().data().data()),
            std::size_t{2} * api_tensor.uint16_tensor().data().size(),
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
    throw Exception(ErrorCondition::k_not_supported, "Unsupported tensor data type");
}

}  // namespace mlmodel
}  // namespace impl
}  // namespace sdk
}  // namespace viam
