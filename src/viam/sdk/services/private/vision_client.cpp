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

#include <viam/sdk/services/private/vision_client.hpp>

#include <stdexcept>

#include <grpcpp/channel.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/private/raw_image.hpp>
#include <viam/sdk/services/private/vision.hpp>

namespace viam {
namespace sdk {
namespace impl {

VisionClient::VisionClient(std::string name, const ViamChannel& channel)
    : Vision(std::move(name)),
      stub_(service_type::NewStub(channel.channel())),
      channel_(&channel) {}

std::vector<Vision::detection> VisionClient::get_detections_from_camera(
    const std::string& camera_name, const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &service_type::StubInterface::GetDetectionsFromCamera)
        .with(extra, [&](auto& req) { req.set_camera_name(camera_name); })
        .invoke([](auto& response) {
            std::vector<Vision::detection> out;
            out.reserve(response.detections_size());
            for (const auto& d : response.detections()) {
                out.push_back(impl::vision::from_proto(d));
            }
            return out;
        });
}

std::vector<Vision::detection> VisionClient::get_detections(const Vision::image& img,
                                                            const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &service_type::StubInterface::GetDetections)
        .with(extra,
              [&](auto& req) {
                  req.set_image(std::string(img.bytes.begin(), img.bytes.end()));
                  req.set_mime_type(img.mime_type);
              })
        .invoke([](auto& response) {
            std::vector<Vision::detection> out;
            out.reserve(response.detections_size());
            for (const auto& d : response.detections()) {
                out.push_back(impl::vision::from_proto(d));
            }
            return out;
        });
}

std::vector<Vision::classification> VisionClient::get_classifications_from_camera(
    const std::string& camera_name, int count, const ProtoStruct& extra) {
    return make_client_helper(
               this, *stub_, &service_type::StubInterface::GetClassificationsFromCamera)
        .with(extra,
              [&](auto& req) {
                  req.set_camera_name(camera_name);
                  req.set_n(count);
              })
        .invoke([](auto& response) {
            std::vector<Vision::classification> out;
            out.reserve(response.classifications_size());
            for (const auto& c : response.classifications()) {
                out.push_back(impl::vision::from_proto(c));
            }
            return out;
        });
}

std::vector<Vision::classification> VisionClient::get_classifications(const Vision::image& img,
                                                                      int count,
                                                                      const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &service_type::StubInterface::GetClassifications)
        .with(extra,
              [&](auto& req) {
                  req.set_image(std::string(img.bytes.begin(), img.bytes.end()));
                  req.set_mime_type(img.mime_type);
                  req.set_n(count);
              })
        .invoke([](auto& response) {
            std::vector<Vision::classification> out;
            out.reserve(response.classifications_size());
            for (const auto& c : response.classifications()) {
                out.push_back(impl::vision::from_proto(c));
            }
            return out;
        });
}

std::vector<Vision::point_cloud_object> VisionClient::get_object_point_clouds(
    const std::string& camera_name, const std::string& mime_type, const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &service_type::StubInterface::GetObjectPointClouds)
        .with(extra,
              [&](auto& req) {
                  req.set_camera_name(camera_name);
                  req.set_mime_type(mime_type);
              })
        .invoke([](auto& response) {
            std::vector<Vision::point_cloud_object> out;
            out.reserve(response.objects_size());
            // The wire format carries one mime_type for all objects; copy it
            // onto each returned object.  May differ from the requested mime —
            // see GetObjectPointCloudsResponse.mime_type in vision.proto.
            for (const auto& proto_obj : response.objects()) {
                auto pco = impl::vision::from_proto(proto_obj);
                pco.cloud.mime_type = response.mime_type();
                out.push_back(std::move(pco));
            }
            return out;
        });
}

Vision::properties VisionClient::get_properties(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &service_type::StubInterface::GetProperties)
        .with(extra)
        .invoke([](auto& response) { return impl::vision::from_proto(response); });
}

Vision::capture_all_result VisionClient::capture_all_from_camera(
    const std::string& camera_name, const Vision::capture_options& opts, const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &service_type::StubInterface::CaptureAllFromCamera)
        .with(extra,
              [&](auto& req) {
                  req.set_camera_name(camera_name);
                  req.set_return_image(opts.return_image);
                  req.set_return_detections(opts.return_detections);
                  req.set_return_classifications(opts.return_classifications);
                  req.set_return_object_point_clouds(opts.return_object_point_clouds);
              })
        .invoke([](auto& response) {
            Vision::capture_all_result out;
            if (response.has_image()) {
                out.image = impl::from_proto(response.image());
            }
            for (const auto& d : response.detections()) {
                out.detections.push_back(impl::vision::from_proto(d));
            }
            for (const auto& c : response.classifications()) {
                out.classifications.push_back(impl::vision::from_proto(c));
            }
            for (const auto& o : response.objects()) {
                out.objects.push_back(impl::vision::from_proto(o));
            }
            if (response.has_extra()) {
                out.extra = sdk::from_proto(response.extra());
            }
            return out;
        });
}

ProtoStruct VisionClient::do_command(const ProtoStruct& command) {
    return make_client_helper(this, *stub_, &service_type::StubInterface::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = to_proto(command); })
        .invoke([](auto& response) { return sdk::from_proto(response.result()); });
}

ProtoStruct VisionClient::get_status() {
    return make_client_helper(this, *stub_, &service_type::StubInterface::GetStatus)
        .invoke([](auto& response) { return sdk::from_proto(response.result()); });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
