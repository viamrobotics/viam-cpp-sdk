#include "mock_vision.hpp"

#include <viam/sdk/common/exception.hpp>

namespace viam {
namespace sdktests {
namespace vision {

using namespace viam::sdk;

MockVision::MockVision(std::string name) : sdk::Vision(std::move(name)) {}

std::vector<Vision::detection> MockVision::get_detections_from_camera(
    const std::string& camera_name, const ProtoStruct& extra) {
    if (throw_on_next_call) {
        throw_on_next_call = false;
        throw sdk::Exception("mock failure");
    }
    last_camera_name = camera_name;
    last_extra = extra;
    return canned_detections;
}

std::vector<Vision::detection> MockVision::get_detections(const Vision::image& img,
                                                          const ProtoStruct& extra) {
    if (throw_on_next_call) {
        throw_on_next_call = false;
        throw sdk::Exception("mock failure");
    }
    last_image = img;
    last_extra = extra;
    return canned_detections;
}

std::vector<Vision::classification> MockVision::get_classifications_from_camera(
    const std::string& camera_name, int count, const ProtoStruct& extra) {
    if (throw_on_next_call) {
        throw_on_next_call = false;
        throw sdk::Exception("mock failure");
    }
    last_camera_name = camera_name;
    last_count = count;
    last_extra = extra;
    return canned_classifications;
}

std::vector<Vision::classification> MockVision::get_classifications(const Vision::image& img,
                                                                    int count,
                                                                    const ProtoStruct& extra) {
    if (throw_on_next_call) {
        throw_on_next_call = false;
        throw sdk::Exception("mock failure");
    }
    last_image = img;
    last_count = count;
    last_extra = extra;
    return canned_classifications;
}

std::vector<Vision::point_cloud_object> MockVision::get_object_point_clouds(
    const std::string& camera_name, const std::string& mime_type, const ProtoStruct& extra) {
    if (throw_on_next_call) {
        throw_on_next_call = false;
        throw sdk::Exception("mock failure");
    }
    last_camera_name = camera_name;
    last_mime_type = mime_type;
    last_extra = extra;
    return canned_objects;
}

Vision::properties MockVision::get_properties(const ProtoStruct& extra) {
    if (throw_on_next_call) {
        throw_on_next_call = false;
        throw sdk::Exception("mock failure");
    }
    last_extra = extra;
    return canned_properties;
}

Vision::capture_all_result MockVision::capture_all_from_camera(const std::string& camera_name,
                                                               const capture_options& opts,
                                                               const ProtoStruct& extra) {
    if (throw_on_next_call) {
        throw_on_next_call = false;
        throw sdk::Exception("mock failure");
    }
    last_camera_name = camera_name;
    last_capture_options = opts;
    last_extra = extra;
    return canned_capture_all;
}

ProtoStruct MockVision::do_command(const ProtoStruct& command) {
    if (throw_on_next_call) {
        throw_on_next_call = false;
        throw sdk::Exception("mock failure");
    }
    last_command = command;
    return command;
}

ProtoStruct MockVision::get_status() {
    if (throw_on_next_call) {
        throw_on_next_call = false;
        throw sdk::Exception("mock failure");
    }
    return canned_status;
}

}  // namespace vision
}  // namespace sdktests
}  // namespace viam
