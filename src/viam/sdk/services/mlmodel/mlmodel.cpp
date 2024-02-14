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

#include <viam/sdk/services/mlmodel/mlmodel.hpp>

#include <viam/sdk/common/utils.hpp>

namespace viam {
namespace sdk {

API MLModelService::api() const {
    return API::get<MLModelService>();
}

API API::traits<MLModelService>::api() {
    return API(kRDK, kService, "mlmodel");
}

boost::optional<MLModelService::tensor_info::data_types>
MLModelService::tensor_info::string_to_data_type(const std::string& str) {
    // TODO: I'm guessing at the string names here, because the scope
    // doesn't actually specify them.
    if (str == "int8") {
        return data_types::k_int8;
    }
    if (str == "uint8") {
        return data_types::k_uint8;
    }
    if (str == "int16") {
        return data_types::k_int16;
    }
    if (str == "uint16") {
        return data_types::k_uint16;
    }
    if (str == "int32") {
        return data_types::k_int32;
    }
    if (str == "uint32") {
        return data_types::k_uint32;
    }
    if (str == "int64") {
        return data_types::k_int64;
    }
    if (str == "uint64") {
        return data_types::k_uint64;
    }
    if (str == "float32") {
        return data_types::k_float32;
    }
    if (str == "float64") {
        return data_types::k_float64;
    }
    return {};
}

const char* MLModelService::tensor_info::data_type_to_string(const data_types data_type) {
    // TODO: I'm guessing at the string names here, because the scope
    // doesn't actually specify them.
    if (data_type == data_types::k_int8) {
        return "int8";
    }
    if (data_type == data_types::k_uint8) {
        return "uint8";
    }
    if (data_type == data_types::k_int16) {
        return "int16";
    }
    if (data_type == data_types::k_uint16) {
        return "uint16";
    }
    if (data_type == data_types::k_int32) {
        return "int32";
    }
    if (data_type == data_types::k_uint32) {
        return "uint32";
    }
    if (data_type == data_types::k_int64) {
        return "int64";
    }
    if (data_type == data_types::k_uint64) {
        return "uint64";
    }
    if (data_type == data_types::k_float32) {
        return "float32";
    }
    if (data_type == data_types::k_float64) {
        return "float64";
    }
    return nullptr;
}

MLModelService::tensor_info::data_types MLModelService::tensor_info::tensor_views_to_data_type(
    const tensor_views& view) {
    class visitor : public boost::static_visitor<data_types> {
       public:
        data_types operator()(const MLModelService::tensor_view<std::int8_t>& t) const {
            return data_types::k_int8;
        }

        data_types operator()(const MLModelService::tensor_view<std::uint8_t>& t) const {
            return data_types::k_uint8;
        }

        data_types operator()(const MLModelService::tensor_view<std::int16_t>& t) const {
            return data_types::k_int16;
        }

        data_types operator()(const MLModelService::tensor_view<std::uint16_t>& t) const {
            return data_types::k_uint16;
        }

        data_types operator()(const MLModelService::tensor_view<std::int32_t>& t) const {
            return data_types::k_int32;
        }

        data_types operator()(const MLModelService::tensor_view<std::uint32_t>& t) const {
            return data_types::k_uint32;
        }

        data_types operator()(const MLModelService::tensor_view<std::int64_t>& t) const {
            return data_types::k_int64;
        }

        data_types operator()(const MLModelService::tensor_view<std::uint64_t>& t) const {
            return data_types::k_uint64;
        }

        data_types operator()(const MLModelService::tensor_view<float>& t) const {
            return data_types::k_float32;
        }

        data_types operator()(const MLModelService::tensor_view<double>& t) const {
            return data_types::k_float64;
        }
    };
    return boost::apply_visitor(visitor(), view);
}

MLModelService::MLModelService(std::string name) : Service(std::move(name)) {}

}  // namespace sdk
}  // namespace viam
