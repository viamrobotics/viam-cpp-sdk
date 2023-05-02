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
#include <xtensor/xchunked_array.hpp>

#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/services/service_base.hpp>

namespace viam {
namespace sdk {

class MLModelServiceSubtype : public ResourceSubtype {
   public:
    explicit MLModelServiceSubtype(const google::protobuf::ServiceDescriptor* service_descriptor);

    std::shared_ptr<ResourceServerBase> create_resource_server(
        std::shared_ptr<ResourceManager> manager) override;

    std::shared_ptr<ResourceBase> create_rpc_client(
        std::string name, std::shared_ptr<grpc::Channel> channel) override;
};

///
/// The `MLModelService` presents the API for an ML Model Service.
///
class MLModelService : public ServiceBase {
   public:
    // Ultimately, we want an xchunked_array backed by a std::vector
    // of adapted linear buffers over T. Explicitly naming that type
    // would be profoundly difficult given how `xtensor` works. Use
    // decltype/declval to metaprogram up the type we want instead.
    template <typename T>
    class tensor_view {
       private:
        using shape_t = std::vector<std::size_t>;

        using xt_no_ownership_t = decltype(xt::no_ownership());

        // The type for the first layer of adapt over the raw buffer
        using adapt_l1_t = decltype(xt::adapt(std::declval<T*>(),
                                              std::declval<std::size_t>(),
                                              std::declval<xt_no_ownership_t>(),
                                              std::declval<shape_t>()));

        // The l1 adapts will be stored in a vector.
        using adapt_l1_storage_t = std::vector<adapt_l1_t>;

        // The type for the second layer of adapt over a vector of l1 adapts.
        using adapt_l2_t = decltype(xt::adapt(std::declval<adapt_l1_storage_t>().data(),
                                              std::declval<adapt_l1_storage_t>().size(),
                                              std::declval<xt_no_ownership_t>(),
                                              std::declval<shape_t>()));

       public:
        // The type for an `xchunked_array` viewing over the l2 adapt.
        //
        // Hooray! we made it!
        using type = decltype(xt::xchunked_array<adapt_l2_t>(std::move(std::declval<adapt_l2_t>()),
                                                             std::declval<shape_t>(),
                                                             std::declval<shape_t>()));
    };

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
        boost::mpl::transform_view<base_types, tensor_view<boost::mpl::placeholders::_1>>;

    // Union the tensor views for the various base types.
    using tensor_views = boost::make_variant_over<tensor_view_types>::type;

    // Our parameters to and from the model come as named tensor_views.
    using tensor_map = std::unordered_map<std::string, tensor_views>;

    // Inputs are just the name / tensor_view mapping.
    using infer_request = tensor_map;

    // Outputs are also the name / tensor_view mapping, but tupled together
    // with an opaque state handle that owns the backing memory.
    struct infer_result_state;
    using infer_result = std::tuple<std::shared_ptr<infer_result_state>, tensor_map>;

    // XXX ACM TODO: doc comment
    virtual infer_result infer(const infer_request& inputs) = 0;

    struct tensor_info {
        struct file {
            std::string name;
            std::string description;

            enum {
                k_type_tensor_value,
                k_type_tensor_axis,
            } label_type;
        };

        std::string name;
        std::string description;

        // XXX ACM TODO: make `data_type` an enum.
        std::string data_type;

        std::vector<int> shape;
        std::vector<file> associated_files;

        // XXX ACM TODO: support `extra` field
        // extra;
    };

    struct metadata {
        std::string name;
        std::string type;
        std::string description;
        std::vector<tensor_info> inputs;
        std::vector<tensor_info> outputs;
    };

    // XXX ACM TODO: doc comment
    virtual struct metadata metadata() = 0;

    static std::shared_ptr<ResourceSubtype> resource_subtype();

   protected:
    explicit MLModelService(std::string name);
};

}  // namespace sdk
}  // namespace viam
