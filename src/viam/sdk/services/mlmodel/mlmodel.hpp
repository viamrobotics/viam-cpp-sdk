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
        // TODO: Should this be an enum?
        std::string data_type;
        std::vector<int> shape;
        std::vector<file> associated_files;
        // XXX ACM TODO: 'extra' field
    };

    struct metadata {
        std::string name;
        std::string type;
        std::string description;
        std::vector<tensor_info> inputs;
        std::vector<tensor_info> outputs;
    };

    virtual void infer() = 0;
    virtual struct metadata metadata() = 0;

    static std::shared_ptr<ResourceSubtype> resource_subtype();

   protected:
    explicit MLModelService(std::string name);
};

}  // namespace sdk
}  // namespace viam
