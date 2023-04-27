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

#include <viam/sdk/services/mlmodel/client.hpp>
#include <viam/sdk/services/mlmodel/mlmodel.hpp>
#include <viam/sdk/services/mlmodel/server.hpp>

namespace viam {
namespace sdktests {

class MockMLModelService : public sdk::MLModelService {
   public:
    explicit MockMLModelService(std::string name = "mock_mlmodel") : MLModelService(std::move(name)) {}

    infer_result infer(const tensor_map& inputs) override;

    MockMLModelService& metadata(struct metadata metadata);
    struct metadata metadata() override;

private:
    struct metadata metadata_;
};

#if 0
class MockMLModelStub : public viam::service::mlmodel::v1::MLModelService::StubInterface {
   public:
    MockMLModelStub();

    ::grpc::Status Infer(::grpc::ClientContext* context,
                         const ::viam::service::mlmodel::v1::InferRequest& request,
                         ::viam::service::mlmodel::v1::InferResponse* response) override;

    ::grpc::Status Metadata(::grpc::ClientContext* context,
                            const ::viam::service::mlmodel::v1::MetadataRequest& request,
                            ::viam::service::mlmodel::v1::MetadataResponse* response) override;

   private:
    ::grpc::ClientAsyncResponseReaderInterface<::viam::service::mlmodel::v1::InferResponse>*
    AsyncInferRaw(::grpc::ClientContext* context,
                  const ::viam::service::mlmodel::v1::InferRequest& request,
                  ::grpc::CompletionQueue* cq) override;

    ::grpc::ClientAsyncResponseReaderInterface<::viam::service::mlmodel::v1::InferResponse>*
    PrepareAsyncInferRaw(::grpc::ClientContext* context,
                         const ::viam::service::mlmodel::v1::InferRequest& request,
                         ::grpc::CompletionQueue* cq) override;

    ::grpc::ClientAsyncResponseReaderInterface<::viam::service::mlmodel::v1::MetadataResponse>*
    AsyncMetadataRaw(::grpc::ClientContext* context,
                     const ::viam::service::mlmodel::v1::MetadataRequest& request,
                     ::grpc::CompletionQueue* cq) override;

    ::grpc::ClientAsyncResponseReaderInterface<::viam::service::mlmodel::v1::MetadataResponse>*
    PrepareAsyncMetadataRaw(::grpc::ClientContext* context,
                            const ::viam::service::mlmodel::v1::MetadataRequest& request,
                            ::grpc::CompletionQueue* cq) override;
};

class MockMLModelServiceClient : public sdk::MLModelServiceClient {
   public:
    explicit MockMLModelServiceClient(std::string name)
        : MLModelServiceClient(std::move(name), std::make_unique<MockMLModelStub>()) {}
};

#endif
}  // namespace sdktests
}  // namespace viam
