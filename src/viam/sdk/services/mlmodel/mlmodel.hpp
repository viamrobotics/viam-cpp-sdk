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

#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/services/service_base.hpp>
#include <viam/sdk/subtype/subtype.hpp>

namespace viam {
namespace sdk {

class MLModelServiceSubtype : public ResourceSubtype {
   public:
    explicit MLModelServiceSubtype(const google::protobuf::ServiceDescriptor* service_descriptor);

    std::shared_ptr<ResourceServerBase> create_resource_server(
        std::shared_ptr<SubtypeService> service) override;

    std::shared_ptr<ResourceBase> create_rpc_client(
        std::string name, std::shared_ptr<grpc::Channel> channel) override;
};

///
/// The `MLModelService` presents the API for an ML Model Service.
///
class MLModelService : public ServiceBase {
   public:
    template <typename T>
    class tensor_view {};

    using signed_integral_types =
        boost::mpl::list<std::int8_t, std::int16_t, std::int32_t, std::int64_t>;
    using unsigned_integral_types = boost::mpl::transform_view<signed_integral_types, std::make_unsigned<boost::mpl::placeholders::_1>>;
    using integral_types = boost::mpl::joint_view<signed_integral_types, unsigned_integral_types>;

    using fp_types = boost::mpl::list<float, double>;

    using tensor_types = boost::mpl::joint_view<integral_types, fp_types>;

    using tensor_views = boost::make_variant_over<tensor_types>::type;
    using tensor_map = std::unordered_map<std::string, tensor_views>;

    struct tensor_state;
    using infer_result = std::tuple<std::shared_ptr<tensor_state>, tensor_map>;

    virtual infer_result infer(const tensor_map& inputs) = 0;

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

    virtual struct metadata metadata() = 0;

    static std::shared_ptr<ResourceSubtype> resource_subtype();

   protected:
    explicit MLModelService(std::string name);
};

}  // namespace sdk
}  // namespace viam
