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

#include <viam/sdk/services/private/vision_server.hpp>

#include <viam/sdk/common/private/raw_image.hpp>
#include <viam/sdk/common/private/service_helper.hpp>
#include <viam/sdk/services/private/vision.hpp>

namespace viam {
namespace sdk {
namespace impl {

VisionServer::VisionServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(std::move(manager)) {}

::grpc::Status VisionServer::GetDetectionsFromCamera(
    ::grpc::ServerContext* context,
    const ::viam::service::vision::v1::GetDetectionsFromCameraRequest* request,
    ::viam::service::vision::v1::GetDetectionsFromCameraResponse* response) noexcept {
    return make_service_helper<Vision>(
        "VisionServer::GetDetectionsFromCamera", this, context, request)(
        [&](auto& helper, auto& vs) {
            const auto results =
                vs->get_detections_from_camera(request->camera_name(), helper.getExtra());
            for (const auto& d : results) {
                *response->add_detections() = impl::vision::to_proto(d);
            }
        });
}

::grpc::Status VisionServer::GetDetections(
    ::grpc::ServerContext* context,
    const ::viam::service::vision::v1::GetDetectionsRequest* request,
    ::viam::service::vision::v1::GetDetectionsResponse* response) noexcept {
    return make_service_helper<Vision>(
        "VisionServer::GetDetections", this, context, request)([&](auto& helper, auto& vs) {
        Vision::raw_image image;
        image.bytes.assign(request->image().begin(), request->image().end());
        image.mime_type = request->mime_type();
        const auto results = vs->get_detections(image, helper.getExtra());
        for (const auto& d : results) {
            *response->add_detections() = impl::vision::to_proto(d);
        }
    });
}

::grpc::Status VisionServer::GetClassificationsFromCamera(
    ::grpc::ServerContext* context,
    const ::viam::service::vision::v1::GetClassificationsFromCameraRequest* request,
    ::viam::service::vision::v1::GetClassificationsFromCameraResponse* response) noexcept {
    return make_service_helper<Vision>(
        "VisionServer::GetClassificationsFromCamera", this, context, request)(
        [&](auto& helper, auto& vs) {
            const auto results = vs->get_classifications_from_camera(
                request->camera_name(), request->n(), helper.getExtra());
            for (const auto& c : results) {
                *response->add_classifications() = impl::vision::to_proto(c);
            }
        });
}

::grpc::Status VisionServer::GetClassifications(
    ::grpc::ServerContext* context,
    const ::viam::service::vision::v1::GetClassificationsRequest* request,
    ::viam::service::vision::v1::GetClassificationsResponse* response) noexcept {
    return make_service_helper<Vision>(
        "VisionServer::GetClassifications", this, context, request)([&](auto& helper, auto& vs) {
        Vision::raw_image image;
        image.bytes.assign(request->image().begin(), request->image().end());
        image.mime_type = request->mime_type();
        const auto results = vs->get_classifications(image, request->n(), helper.getExtra());
        for (const auto& c : results) {
            *response->add_classifications() = impl::vision::to_proto(c);
        }
    });
}

::grpc::Status VisionServer::GetObjectPointClouds(
    ::grpc::ServerContext* context,
    const ::viam::service::vision::v1::GetObjectPointCloudsRequest* request,
    ::viam::service::vision::v1::GetObjectPointCloudsResponse* response) noexcept {
    return make_service_helper<Vision>(
        "VisionServer::GetObjectPointClouds", this, context, request)([&](auto& helper, auto& vs) {
        const auto results = vs->get_object_point_clouds(
            request->camera_name(), request->mime_type(), helper.getExtra());
        for (const auto& obj : results) {
            impl::vision::to_proto(obj, response->add_objects());
        }
        response->set_mime_type(request->mime_type());
    });
}

::grpc::Status VisionServer::GetProperties(
    ::grpc::ServerContext* context,
    const ::viam::service::vision::v1::GetPropertiesRequest* request,
    ::viam::service::vision::v1::GetPropertiesResponse* response) noexcept {
    return make_service_helper<Vision>(
        "VisionServer::GetProperties", this, context, request)([&](auto& helper, auto& vs) {
        const auto props = vs->get_properties(helper.getExtra());
        impl::vision::to_proto(props, response);
    });
}

::grpc::Status VisionServer::CaptureAllFromCamera(
    ::grpc::ServerContext* context,
    const ::viam::service::vision::v1::CaptureAllFromCameraRequest* request,
    ::viam::service::vision::v1::CaptureAllFromCameraResponse* response) noexcept {
    return make_service_helper<Vision>(
        "VisionServer::CaptureAllFromCamera", this, context, request)([&](auto& helper, auto& vs) {
        Vision::capture_options opts;
        opts.return_image = request->return_image();
        opts.return_detections = request->return_detections();
        opts.return_classifications = request->return_classifications();
        opts.return_object_point_clouds = request->return_object_point_clouds();

        const auto result =
            vs->capture_all_from_camera(request->camera_name(), opts, helper.getExtra());

        if (result.image) {
            impl::to_proto(*result.image, response->mutable_image());
        }
        for (const auto& d : result.detections) {
            *response->add_detections() = impl::vision::to_proto(d);
        }
        for (const auto& c : result.classifications) {
            *response->add_classifications() = impl::vision::to_proto(c);
        }
        for (const auto& o : result.objects) {
            impl::vision::to_proto(o, response->add_objects());
        }
        if (!result.extra.empty()) {
            *response->mutable_extra() = to_proto(result.extra);
        }
    });
}

::grpc::Status VisionServer::DoCommand(::grpc::ServerContext* context,
                                       const ::viam::common::v1::DoCommandRequest* request,
                                       ::viam::common::v1::DoCommandResponse* response) noexcept {
    return make_service_helper<Vision>(
        "VisionServer::DoCommand", this, context, request)([&](auto&, auto& vs) {
        const ProtoStruct result = vs->do_command(sdk::from_proto(request->command()));
        *response->mutable_result() = to_proto(result);
    });
}

::grpc::Status VisionServer::GetStatus(::grpc::ServerContext* context,
                                       const ::viam::common::v1::GetStatusRequest* request,
                                       ::viam::common::v1::GetStatusResponse* response) noexcept {
    return make_service_helper<Vision>(
        "VisionServer::GetStatus", this, context, request)([&](auto&, auto& vs) {
        const ProtoStruct result = vs->get_status();
        *response->mutable_result() = to_proto(result);
    });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
