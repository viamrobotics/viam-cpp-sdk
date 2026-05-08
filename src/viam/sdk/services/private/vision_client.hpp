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

#include <viam/sdk/rpc/dial.hpp>
#include <viam/sdk/services/vision.hpp>

namespace viam {
namespace sdk {
namespace impl {

class VisionClient : public Vision {
   public:
    using interface_type = Vision;
    using service_type = ::viam::service::vision::v1::VisionService;

    VisionClient(std::string name, const ViamChannel& channel);

    const ViamChannel& channel() const {
        return *channel_;
    }

    std::vector<Vision::detection> get_detections_from_camera(const std::string& camera_name,
                                                              const ProtoStruct& extra) override;
    std::vector<Vision::detection> get_detections(const Vision::raw_image& image,
                                                  const ProtoStruct& extra) override;
    std::vector<Vision::classification> get_classifications_from_camera(
        const std::string& camera_name, int count, const ProtoStruct& extra) override;
    std::vector<Vision::classification> get_classifications(const Vision::raw_image& image,
                                                            int count,
                                                            const ProtoStruct& extra) override;
    std::vector<Vision::point_cloud_object> get_object_point_clouds(
        const std::string& camera_name,
        const std::string& mime_type,
        const ProtoStruct& extra) override;
    Vision::properties get_properties(const ProtoStruct& extra) override;
    Vision::capture_all_result capture_all_from_camera(const std::string& camera_name,
                                                       const Vision::capture_options& opts,
                                                       const ProtoStruct& extra) override;
    ProtoStruct do_command(const ProtoStruct& command) override;
    ProtoStruct get_status() override;

    using Vision::capture_all_from_camera;
    using Vision::get_classifications;
    using Vision::get_classifications_from_camera;
    using Vision::get_detections;
    using Vision::get_detections_from_camera;
    using Vision::get_object_point_clouds;
    using Vision::get_properties;

   private:
    std::unique_ptr<service_type::StubInterface> stub_;
    const ViamChannel* channel_;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
