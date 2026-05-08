// Copyright 2024 Viam Inc.
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

#define BOOST_TEST_MODULE vision

#include <memory>

#include <boost/test/included/unit_test.hpp>

#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/rpc/dial.hpp>
#include <viam/sdk/rpc/server.hpp>
#include <viam/sdk/services/private/vision.hpp>
#include <viam/sdk/services/vision.hpp>
#include <viam/sdk/tests/mocks/mock_vision.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace vimpl = viam::sdk::impl::vision;
namespace vpb = viam::service::vision::v1;
using viam::sdk::Vision;

namespace viam {
namespace sdktests {
namespace vision {

// vision_fixture stands up an in-process gRPC server backed by MockVision and a
// VisionClient connected to it over an InProcessChannel.  Per-RPC test suites
// (tasks 8-14) declare this as their fixture so they can reach both the canned
// mock state and the client interface under test.
struct vision_fixture {
    std::shared_ptr<MockVision> mock;

    // Order: channel_ must outlive client (VisionClient holds a raw channel ptr).
    std::shared_ptr<sdk::Server> server_;
    TestServer test_server_;
    std::unique_ptr<sdk::ViamChannel> channel_;

    std::shared_ptr<sdk::Vision> client;

    vision_fixture()
        : mock(std::make_shared<MockVision>("mock-vision")),
          server_(std::make_shared<sdk::Server>()),
          test_server_(server_) {
        server_->add_resource(mock);
        server_->start();

        channel_ = std::make_unique<sdk::ViamChannel>(test_server_.grpc_in_process_channel());

        client = std::dynamic_pointer_cast<sdk::Vision>(
            sdk::Registry::get()
                .lookup_resource_client(sdk::API::get<sdk::Vision>())
                ->create_rpc_client(mock->name(), *channel_));

        BOOST_REQUIRE(client != nullptr);
    }

    ~vision_fixture() {
        server_->shutdown();
    }
};

}  // namespace vision
}  // namespace sdktests
}  // namespace viam

BOOST_TEST_DONT_PRINT_LOG_VALUE(Vision::detection)
BOOST_TEST_DONT_PRINT_LOG_VALUE(Vision::classification)
BOOST_TEST_DONT_PRINT_LOG_VALUE(Vision::properties)

BOOST_AUTO_TEST_SUITE(vision_proto_conv)

BOOST_AUTO_TEST_CASE(detection_round_trip_pixel_only) {
    Vision::detection d;
    d.x_min = 1;
    d.y_min = 2;
    d.x_max = 3;
    d.y_max = 4;
    d.class_name = "cat";
    d.confidence = 0.9;

    const vpb::Detection proto = vimpl::to_proto(d);
    const Vision::detection back = vimpl::from_proto(proto);

    BOOST_TEST(back == d);
    BOOST_TEST(!back.x_min_normalized.has_value());
}

BOOST_AUTO_TEST_CASE(detection_round_trip_normalized_only) {
    Vision::detection d;
    d.x_min_normalized = 0.1;
    d.y_min_normalized = 0.2;
    d.x_max_normalized = 0.3;
    d.y_max_normalized = 0.4;
    d.class_name = "dog";
    d.confidence = 0.7;

    const vpb::Detection proto = vimpl::to_proto(d);
    const Vision::detection back = vimpl::from_proto(proto);

    BOOST_TEST(back == d);
    BOOST_TEST(!back.x_min.has_value());
}

BOOST_AUTO_TEST_CASE(detection_round_trip_no_bbox) {
    Vision::detection d;
    d.class_name = "unknown";
    d.confidence = 0.1;

    const vpb::Detection proto = vimpl::to_proto(d);
    const Vision::detection back = vimpl::from_proto(proto);

    BOOST_TEST(back == d);
    BOOST_TEST(!back.x_min.has_value());
    BOOST_TEST(!back.x_min_normalized.has_value());
}

BOOST_AUTO_TEST_CASE(classification_round_trip) {
    Vision::classification c{"cat", 0.95};
    const vpb::Classification proto = vimpl::to_proto(c);
    const Vision::classification back = vimpl::from_proto(proto);
    BOOST_TEST(back == c);
}

BOOST_AUTO_TEST_CASE(properties_round_trip) {
    Vision::properties p{true, false, true};
    vpb::GetPropertiesResponse proto;
    vimpl::to_proto(p, &proto);
    BOOST_TEST(vimpl::from_proto(proto) == p);
}

BOOST_AUTO_TEST_CASE(point_cloud_object_round_trip_no_geometries) {
    Vision::point_cloud_object o;
    o.point_cloud.mime_type = "application/pcd";
    o.point_cloud.pc = {0xDE, 0xAD, 0xBE, 0xEF};
    // geometries empty
    ::viam::common::v1::PointCloudObject proto;
    vimpl::to_proto(o, &proto);
    auto back = vimpl::from_proto(proto);
    // mime_type round-trip is the caller's responsibility; assert only bytes + geometries.
    BOOST_CHECK_EQUAL_COLLECTIONS(back.point_cloud.pc.begin(),
                                  back.point_cloud.pc.end(),
                                  o.point_cloud.pc.begin(),
                                  o.point_cloud.pc.end());
    BOOST_TEST(back.geometries.empty());
}

BOOST_AUTO_TEST_SUITE_END()

namespace viam {
namespace sdktests {
namespace vision {

BOOST_AUTO_TEST_SUITE(vision_rpc)

BOOST_AUTO_TEST_CASE(get_properties_round_trip) {
    vision_fixture f;
    f.mock->canned_properties = {true, false, true};
    auto got = f.client->get_properties();
    BOOST_TEST(got == f.mock->canned_properties);
    BOOST_TEST(f.mock->last_extra.empty());
}

BOOST_AUTO_TEST_CASE(get_properties_passes_extra) {
    vision_fixture f;
    f.mock->canned_properties = {false, true, false};
    sdk::ProtoStruct extra;
    extra["k"] = sdk::ProtoValue("v");
    auto got = f.client->get_properties(extra);
    BOOST_TEST(got == f.mock->canned_properties);
}

BOOST_AUTO_TEST_CASE(get_status_round_trip) {
    vision_fixture f;
    sdk::ProtoStruct status;
    status["ready"] = sdk::ProtoValue(true);
    status["model"] = sdk::ProtoValue(std::string("yolov8"));
    f.mock->canned_status = status;

    auto got = f.client->get_status();
    BOOST_TEST(got == f.mock->canned_status);
}

BOOST_AUTO_TEST_CASE(do_command_round_trip_simple) {
    vision_fixture f;
    sdk::ProtoStruct cmd;
    cmd["op"] = sdk::ProtoValue(std::string("inspect"));
    cmd["limit"] = sdk::ProtoValue(static_cast<double>(42));

    auto echoed = f.client->do_command(cmd);

    // MockVision::do_command echoes its input back; verify echo round-trips.
    BOOST_TEST(echoed == cmd);
    // Also verify the mock recorded what came in.
    BOOST_TEST(f.mock->last_command == cmd);
}

BOOST_AUTO_TEST_CASE(do_command_round_trip_nested) {
    vision_fixture f;
    sdk::ProtoStruct nested;
    nested["enabled"] = sdk::ProtoValue(true);
    nested["weight"] = sdk::ProtoValue(0.75);

    sdk::ProtoStruct cmd;
    cmd["mode"] = sdk::ProtoValue(std::string("calibrate"));
    cmd["params"] = sdk::ProtoValue(nested);

    auto echoed = f.client->do_command(cmd);
    BOOST_TEST(echoed == cmd);
    BOOST_TEST(f.mock->last_command == cmd);
}

BOOST_AUTO_TEST_CASE(get_classifications_from_camera_round_trip) {
    vision_fixture f;
    f.mock->canned_classifications = {{"cat", 0.9}, {"dog", 0.1}};

    auto got = f.client->get_classifications_from_camera("cam0", 5);

    BOOST_TEST_REQUIRE(got.size() == 2u);
    BOOST_TEST(got[0] == f.mock->canned_classifications[0]);
    BOOST_TEST(got[1] == f.mock->canned_classifications[1]);
    BOOST_TEST(f.mock->last_camera_name == "cam0");
    BOOST_TEST(f.mock->last_count == 5);
}

BOOST_AUTO_TEST_CASE(get_classifications_round_trip) {
    vision_fixture f;
    f.mock->canned_classifications = {{"box", 0.7}};
    sdk::Vision::raw_image img;
    img.mime_type = "image/jpeg";
    img.bytes = {1, 2, 3, 4};

    auto got = f.client->get_classifications(img, 3);

    BOOST_TEST_REQUIRE(got.size() == 1u);
    BOOST_TEST(got[0] == f.mock->canned_classifications[0]);
    BOOST_TEST(f.mock->last_image.bytes == img.bytes);
    BOOST_TEST(f.mock->last_image.mime_type == img.mime_type);
    BOOST_TEST(f.mock->last_count == 3);
}

BOOST_AUTO_TEST_CASE(get_detections_from_camera_round_trip) {
    vision_fixture f;
    sdk::Vision::detection d1;
    d1.x_min = 10;
    d1.y_min = 20;
    d1.x_max = 30;
    d1.y_max = 40;
    d1.class_name = "car";
    d1.confidence = 0.95;
    sdk::Vision::detection d2;
    d2.x_min_normalized = 0.1;
    d2.y_min_normalized = 0.2;
    d2.x_max_normalized = 0.3;
    d2.y_max_normalized = 0.4;
    d2.class_name = "tree";
    d2.confidence = 0.4;
    f.mock->canned_detections = {d1, d2};

    auto got = f.client->get_detections_from_camera("front");

    BOOST_TEST_REQUIRE(got.size() == 2u);
    BOOST_TEST(got[0] == d1);
    BOOST_TEST(got[1] == d2);
    BOOST_TEST(f.mock->last_camera_name == "front");
}

BOOST_AUTO_TEST_CASE(get_detections_round_trip) {
    vision_fixture f;
    sdk::Vision::detection d;
    d.x_min = 5;
    d.y_min = 6;
    d.x_max = 7;
    d.y_max = 8;
    d.class_name = "ball";
    d.confidence = 0.5;
    f.mock->canned_detections = {d};

    sdk::Vision::raw_image img;
    img.mime_type = "image/png";
    img.bytes = {9, 8, 7, 6, 5};

    auto got = f.client->get_detections(img);

    BOOST_TEST_REQUIRE(got.size() == 1u);
    BOOST_TEST(got[0] == d);
    BOOST_TEST(f.mock->last_image.bytes == img.bytes);
    BOOST_TEST(f.mock->last_image.mime_type == img.mime_type);
}

BOOST_AUTO_TEST_CASE(get_detections_no_bbox_round_trip_over_wire) {
    // Edge case: a Detection with neither pixel nor normalized bbox engaged
    // must round-trip with both groups remaining unset on the response side.
    vision_fixture f;
    sdk::Vision::detection d;
    d.class_name = "unknown";
    d.confidence = 0.1;
    // Intentionally no bbox fields engaged.
    f.mock->canned_detections = {d};

    auto got = f.client->get_detections_from_camera("any");

    BOOST_TEST_REQUIRE(got.size() == 1u);
    BOOST_TEST(got[0] == d);
    BOOST_TEST(!got[0].x_min.has_value());
    BOOST_TEST(!got[0].x_min_normalized.has_value());
    BOOST_TEST(got[0].class_name == "unknown");
    BOOST_TEST(got[0].confidence == 0.1);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace vision
}  // namespace sdktests
}  // namespace viam
