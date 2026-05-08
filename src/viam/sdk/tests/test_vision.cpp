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

#include <boost/test/included/unit_test.hpp>

#include <viam/sdk/services/private/vision.hpp>
#include <viam/sdk/services/vision.hpp>

namespace vimpl = viam::sdk::impl::vision;
namespace vpb = viam::service::vision::v1;
using viam::sdk::Vision;

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
