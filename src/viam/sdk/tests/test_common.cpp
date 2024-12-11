#define BOOST_TEST_MODULE test module test_common

#include <boost/test/included/unit_test.hpp>

#include <viam/api/common/v1/common.pb.h>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/common/version_metadata.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {

using namespace viam::sdk;

// For common/util.* protobuf duration from/to_proto
// conversion to ensure we are within the proto
// spec
BOOST_AUTO_TEST_SUITE(test_time_conversion)

using Duration = google::protobuf::Duration;

BOOST_AUTO_TEST_CASE(test_zero) {
    Duration input;
    input.set_nanos(0);
    input.set_seconds(0);
    auto converted = v2::from_proto(input);
    BOOST_CHECK_EQUAL(converted.count(), 0);
    auto reconverted = v2::to_proto(converted);
    BOOST_CHECK_EQUAL(reconverted.nanos(), 0);
    BOOST_CHECK_EQUAL(reconverted.seconds(), 0);
}

BOOST_AUTO_TEST_CASE(test_rounding_negative) {
    Duration input;
    input.set_nanos(-100);
    input.set_seconds(0);
    auto converted = v2::from_proto(input);
    BOOST_CHECK_EQUAL(converted.count(), -1);
    auto reconverted = v2::to_proto(converted);
    BOOST_CHECK_EQUAL(reconverted.nanos(), -1000);
    BOOST_CHECK_EQUAL(reconverted.seconds(), 0);
}

BOOST_AUTO_TEST_CASE(test_rounding_positive) {
    Duration input;
    input.set_nanos(999);
    input.set_seconds(0);
    auto converted = v2::from_proto(input);
    BOOST_CHECK_EQUAL(converted.count(), 1);
    auto reconverted = v2::to_proto(converted);
    BOOST_CHECK_EQUAL(reconverted.nanos(), 1000);
    BOOST_CHECK_EQUAL(reconverted.seconds(), 0);
}

BOOST_AUTO_TEST_CASE(test_mixed_sign_rounding) {
    Duration input;
    // Should round to -1 μs
    input.set_nanos(-500);
    input.set_seconds(1);
    auto converted = v2::from_proto(input);
    BOOST_CHECK_EQUAL(converted.count(), 1e6 - 1);
    auto reconverted = v2::to_proto(converted);
    BOOST_CHECK_EQUAL(reconverted.nanos(), 1e9 - 1000);
    BOOST_CHECK_EQUAL(reconverted.seconds(), 0);
}

BOOST_AUTO_TEST_CASE(test_medium_positive) {
    Duration input;
    // Should round to 2 μs
    input.set_nanos(1500);
    input.set_seconds(1000);
    auto converted = v2::from_proto(input);
    BOOST_CHECK_EQUAL(converted.count(), 1000 * 1e6 + 2);
    auto reconverted = v2::to_proto(converted);
    BOOST_CHECK_EQUAL(reconverted.nanos(), 2000);
    BOOST_CHECK_EQUAL(reconverted.seconds(), 1000);
}

BOOST_AUTO_TEST_CASE(test_large_positive) {
    Duration input;
    // Should round to 2 μs
    input.set_nanos(1500);
    // We need to support at least +/-10k years to be
    // compliant with the proto spec
    int64_t max_seconds = 10e3 * 365 * 24 * 60 * 60;
    input.set_seconds(max_seconds);
    auto converted = v2::from_proto(input);
    BOOST_CHECK_EQUAL(converted.count(), 1e6 * max_seconds + 2);
    auto reconverted = v2::to_proto(converted);
    BOOST_CHECK_EQUAL(reconverted.nanos(), 2000);
    BOOST_CHECK_EQUAL(reconverted.seconds(), max_seconds);
}

BOOST_AUTO_TEST_CASE(test_from_dm_from_extra) {
    ProtoStruct map = fake_map();
    BOOST_CHECK_EQUAL(from_dm_from_extra(map), false);

    map = ProtoStruct{{"fromDataManagement", true}};
    BOOST_CHECK_EQUAL(from_dm_from_extra(map), true);

    map = ProtoStruct{{"fromDataManagement", false}};
    BOOST_CHECK_EQUAL(from_dm_from_extra(map), false);

    map = ProtoStruct{{"fromDataManagement", "hello"}};
    BOOST_CHECK_EQUAL(from_dm_from_extra(map), false);

    map = ProtoStruct{{"fromDataManagement", 3.5}};
    BOOST_CHECK_EQUAL(from_dm_from_extra(map), false);
}

BOOST_AUTO_TEST_CASE(test_version_metadata) {
    // we don't want to check the specific values because they're going to be changing,
    // but we want to confirm that the parsing works and extracts an int value successfully.
    int major = sdk_major_version();
    int minor = sdk_minor_version();
    int patch = sdk_patch_version();

    std::string version_constructed =
        "v" + std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(patch);
    std::string version = sdk_version();

    BOOST_CHECK_EQUAL(version_constructed, version);
}

BOOST_AUTO_TEST_CASE(test_name_conversion) {
    std::string long_name1 = "foo:bar";
    auto res1 = long_name_to_remote_and_short(long_name1);
    BOOST_CHECK_EQUAL(res1.first, "foo");
    BOOST_CHECK_EQUAL(res1.second, "bar");

    std::string long_name2 = "foo:bar:baz";
    auto res2 = long_name_to_remote_and_short(long_name2);
    BOOST_CHECK_EQUAL(res2.first, "foo:bar");
    BOOST_CHECK_EQUAL(res2.second, "baz");

    std::string long_name3 = "foo";
    auto res3 = long_name_to_remote_and_short(long_name3);
    BOOST_CHECK_EQUAL(res3.first, "");
    BOOST_CHECK_EQUAL(res3.second, "foo");
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace sdktests
}  // namespace viam
