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

#pragma once

#include <boost/mpl/joint_view.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/transform_view.hpp>
#include <boost/variant/variant.hpp>
#include <xtensor/xadapt.hpp>

#include <viam/api/service/mlmodel/v1/mlmodel.grpc.pb.h>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/services/service.hpp>

namespace viam {
namespace sdk {

class MLModelServiceRegistration : public ResourceRegistration {
   public:
    explicit MLModelServiceRegistration(
        const google::protobuf::ServiceDescriptor* service_descriptor);

    std::shared_ptr<ResourceServer> create_resource_server(
        std::shared_ptr<ResourceManager> manager) override;

    std::shared_ptr<Resource> create_rpc_client(std::string name,
                                                std::shared_ptr<grpc::Channel> channel) override;
};

/// @class MLModelService mlmodel.hpp "services/mlmodel/mlmodel.hpp"
/// @brief Represents a machine trained learning model instance
///
/// This class acts as an abstract base class to be used by any driver
/// implementing an MLModel. It is also used as the base class for the
/// gRPC client for communicating with a remote MLModel instance.
class MLModelService : public Service {
   private:
    template <typename T>
    struct make_tensor_view_ {
        using shape_t = std::vector<std::size_t>;

        using xt_no_ownership_t = decltype(xt::no_ownership());

        using type = decltype(xt::adapt(std::declval<const T*>(),
                                        std::declval<std::size_t>(),
                                        std::declval<xt_no_ownership_t>(),
                                        std::declval<shape_t>()));
    };

   public:
    static std::shared_ptr<ResourceRegistration> resource_registration();
    API api() const override;

    template <typename T>
    using tensor_view = typename make_tensor_view_<T>::type;

    template <typename T>
    static tensor_view<T> make_tensor_view(const T* data,
                                           std::size_t size,
                                           typename tensor_view<T>::shape_type shape) {
        return xt::adapt(std::move(data), std::move(size), xt::no_ownership(), std::move(shape));
    }

    // Now that we have a factory for our tensor view types, use mpl
    // to produce a variant over tensor views over the primitive types
    // we care about, which are the signed and unsigned fixed width
    // integral types and the two floating point types.
    using signed_integral_base_types =
        boost::mpl::list<std::int8_t, std::int16_t, std::int32_t, std::int64_t>;

    using unsigned_integral_base_types =
        boost::mpl::transform_view<signed_integral_base_types,
                                   std::make_unsigned<boost::mpl::placeholders::_1>>;

    using integral_base_types =
        boost::mpl::joint_view<signed_integral_base_types, unsigned_integral_base_types>;

    using fp_base_types = boost::mpl::list<float, double>;

    using base_types = boost::mpl::joint_view<integral_base_types, fp_base_types>;

    using tensor_view_types =
        boost::mpl::transform_view<base_types, make_tensor_view_<boost::mpl::placeholders::_1>>;

    // Union the tensor views for the various base types.
    using tensor_views = boost::make_variant_over<tensor_view_types>::type;

    // Our parameters to and from the model come as named tensor_views.
    using named_tensor_views = std::unordered_map<std::string, tensor_views>;

    /// @brief Runs the model against the input tensors and returns inference results as tensors.
    ///
    /// @param `inputs`: The input tensors on which to run inference.
    ///
    /// @returns The results of the inference as a shared pointer to `named_tensor_views`. The data
    /// viewed by the views is only valid for the lifetime of the returned shared_ptr.
    inline std::shared_ptr<named_tensor_views> infer(const named_tensor_views& inputs) {
        return infer(inputs, {});
    }

    /// @brief Runs the model against the input tensors and returns inference results as tensors.
    ///
    /// @param `inputs`: The input tensors on which to run inference.
    ///
    /// @param `extra`: Any additional arguments to the method.
    ///
    /// @returns The results of the inference as a shared pointer to `named_tensor_views`. The data
    /// viewed by the views is only valid for the lifetime of the returned shared_ptr.
    virtual std::shared_ptr<named_tensor_views> infer(const named_tensor_views& inputs,
                                                      const AttributeMap& extra) = 0;

    struct tensor_info {
        struct file {
            std::string name;
            std::string description;

            enum : std::uint8_t {
                k_label_type_tensor_value = 0,
                k_label_type_tensor_axis = 1,
            } label_type;
        };

        std::string name;
        std::string description;

        enum class data_types : std::uint8_t {
            k_int8 = 0,
            k_uint8 = 1,
            k_int16 = 2,
            k_uint16 = 3,
            k_int32 = 4,
            k_uint32 = 5,
            k_int64 = 6,
            k_uint64 = 7,
            k_float32 = 8,
            k_float64 = 9,
        } data_type;

        std::vector<int> shape;
        std::vector<file> associated_files;

        AttributeMap extra;

        static boost::optional<data_types> string_to_data_type(const std::string& str);
        static const char* data_type_to_string(data_types data_type);

        static data_types tensor_views_to_data_type(const tensor_views& view);
    };

    struct metadata {
        std::string name;
        std::string type;
        std::string description;
        std::vector<tensor_info> inputs;
        std::vector<tensor_info> outputs;
    };

    /// @brief Returns metadata describing the inputs and outputs of the model
    inline struct metadata metadata() {
        return metadata({});
    }

    /// @brief Returns metadata describing the inputs and outputs of the model
    ///
    /// @param `extra`: Any additional arguments to the method.
    virtual struct metadata metadata(const AttributeMap& extra) = 0;

   protected:
    explicit MLModelService(std::string name);
};

template <>
struct API::api_map<MLModelService> {
    static API api() {
        return API(kRDK, kService, "mlmodel");
    }
};

}  // namespace sdk
}  // namespace viam
