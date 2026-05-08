#pragma once

#include <string>
#include <vector>

#include <viam/sdk/services/vision.hpp>

namespace viam {
namespace sdk {

class MockVision : public Vision {
   public:
    explicit MockVision(std::string name = "mock-vision");

    // Canned responses (set per-test).
    std::vector<detection> canned_detections;
    std::vector<classification> canned_classifications;
    std::vector<point_cloud_object> canned_objects;
    properties canned_properties{false, false, false};
    capture_all_result canned_capture_all;
    ProtoStruct canned_status;

    // Last-call records (used to assert request fields round-trip).
    std::string last_camera_name;
    int last_count = 0;
    std::string last_mime_type;
    raw_image last_image;
    capture_options last_capture_options;
    ProtoStruct last_extra;
    ProtoStruct last_command;

    // Pluggable hook for the exception-mapping test.
    bool throw_on_next_call = false;

    std::vector<detection> get_detections_from_camera(const std::string& camera_name,
                                                      const ProtoStruct& extra) override;
    std::vector<detection> get_detections(const raw_image& image,
                                          const ProtoStruct& extra) override;
    std::vector<classification> get_classifications_from_camera(const std::string& camera_name,
                                                                int count,
                                                                const ProtoStruct& extra) override;
    std::vector<classification> get_classifications(const raw_image& image,
                                                    int count,
                                                    const ProtoStruct& extra) override;
    std::vector<point_cloud_object> get_object_point_clouds(const std::string& camera_name,
                                                            const std::string& mime_type,
                                                            const ProtoStruct& extra) override;
    properties get_properties(const ProtoStruct& extra) override;
    capture_all_result capture_all_from_camera(const std::string& camera_name,
                                               const capture_options& opts,
                                               const ProtoStruct& extra) override;
    ProtoStruct do_command(const ProtoStruct& command) override;
    ProtoStruct get_status() override;
};

}  // namespace sdk
}  // namespace viam
