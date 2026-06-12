#pragma once

#include <string>
#include <vector>

#include <viam/sdk/services/vision.hpp>

namespace viam {
namespace sdktests {
namespace vision {

class MockVision : public sdk::Vision {
   public:
    explicit MockVision(std::string name = "mock-vision");

    // Canned responses (set per-test).
    std::vector<sdk::Vision::detection> canned_detections;
    std::vector<sdk::Vision::classification> canned_classifications;
    std::vector<sdk::Vision::point_cloud_object> canned_objects;
    sdk::Vision::properties canned_properties{false, false, false};
    sdk::Vision::capture_all_result canned_capture_all;
    sdk::ProtoStruct canned_status;

    // Last-call records (used to assert request fields round-trip).
    std::string last_camera_name;
    int last_count = 0;
    std::string last_mime_type;
    sdk::Vision::image last_image;
    sdk::Vision::capture_options last_capture_options;
    sdk::ProtoStruct last_extra;
    sdk::ProtoStruct last_command;

    // Pluggable hook for the exception-mapping test.
    bool throw_on_next_call = false;

    std::vector<sdk::Vision::detection> get_detections_from_camera(
        const std::string& camera_name, const sdk::ProtoStruct& extra) override;
    std::vector<sdk::Vision::detection> get_detections(const sdk::Vision::image& img,
                                                       const sdk::ProtoStruct& extra) override;
    std::vector<sdk::Vision::classification> get_classifications_from_camera(
        const std::string& camera_name, int count, const sdk::ProtoStruct& extra) override;
    std::vector<sdk::Vision::classification> get_classifications(
        const sdk::Vision::image& img, int count, const sdk::ProtoStruct& extra) override;
    std::vector<sdk::Vision::point_cloud_object> get_object_point_clouds(
        const std::string& camera_name,
        const std::string& mime_type,
        const sdk::ProtoStruct& extra) override;
    sdk::Vision::properties get_properties(const sdk::ProtoStruct& extra) override;
    sdk::Vision::capture_all_result capture_all_from_camera(
        const std::string& camera_name,
        const sdk::Vision::capture_options& opts,
        const sdk::ProtoStruct& extra) override;
    sdk::ProtoStruct do_command(const sdk::ProtoStruct& command) override;
    sdk::ProtoStruct get_status() override;
};

}  // namespace vision
}  // namespace sdktests
}  // namespace viam
