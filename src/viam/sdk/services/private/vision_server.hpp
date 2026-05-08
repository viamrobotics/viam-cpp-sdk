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

#include <viam/api/service/vision/v1/vision.grpc.pb.h>

#include <viam/sdk/resource/resource_server_base.hpp>
#include <viam/sdk/services/vision.hpp>

namespace viam {
namespace sdk {
namespace impl {

class VisionServer : public ResourceServer,
                     public ::viam::service::vision::v1::VisionService::Service {
   public:
    using interface_type = Vision;
    using service_type = service::vision::v1::VisionService;
    explicit VisionServer(std::shared_ptr<ResourceManager> manager);

    ::grpc::Status GetDetectionsFromCamera(
        ::grpc::ServerContext* context,
        const ::viam::service::vision::v1::GetDetectionsFromCameraRequest* request,
        ::viam::service::vision::v1::GetDetectionsFromCameraResponse* response) noexcept override;

    ::grpc::Status GetDetections(
        ::grpc::ServerContext* context,
        const ::viam::service::vision::v1::GetDetectionsRequest* request,
        ::viam::service::vision::v1::GetDetectionsResponse* response) noexcept override;

    ::grpc::Status GetClassificationsFromCamera(
        ::grpc::ServerContext* context,
        const ::viam::service::vision::v1::GetClassificationsFromCameraRequest* request,
        ::viam::service::vision::v1::GetClassificationsFromCameraResponse* response) noexcept
        override;

    ::grpc::Status GetClassifications(
        ::grpc::ServerContext* context,
        const ::viam::service::vision::v1::GetClassificationsRequest* request,
        ::viam::service::vision::v1::GetClassificationsResponse* response) noexcept override;

    ::grpc::Status GetObjectPointClouds(
        ::grpc::ServerContext* context,
        const ::viam::service::vision::v1::GetObjectPointCloudsRequest* request,
        ::viam::service::vision::v1::GetObjectPointCloudsResponse* response) noexcept override;

    ::grpc::Status GetProperties(
        ::grpc::ServerContext* context,
        const ::viam::service::vision::v1::GetPropertiesRequest* request,
        ::viam::service::vision::v1::GetPropertiesResponse* response) noexcept override;

    ::grpc::Status CaptureAllFromCamera(
        ::grpc::ServerContext* context,
        const ::viam::service::vision::v1::CaptureAllFromCameraRequest* request,
        ::viam::service::vision::v1::CaptureAllFromCameraResponse* response) noexcept override;

    ::grpc::Status DoCommand(::grpc::ServerContext* context,
                             const ::viam::common::v1::DoCommandRequest* request,
                             ::viam::common::v1::DoCommandResponse* response) noexcept override;

    ::grpc::Status GetStatus(::grpc::ServerContext* context,
                             const ::viam::common::v1::GetStatusRequest* request,
                             ::viam::common::v1::GetStatusResponse* response) noexcept override;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
