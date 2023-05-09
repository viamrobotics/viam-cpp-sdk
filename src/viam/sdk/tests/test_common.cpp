#define BOOST_TEST_MODULE test module test_common

#include <boost/test/included/unit_test.hpp>

#include <viam/api/common/v1/common.pb.h>

#include <viam/sdk/common/utils.hpp>

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
    auto converted = from_proto(input);
    BOOST_CHECK_EQUAL(converted.count(), 0);
    auto reconverted = to_proto(converted);
    BOOST_CHECK_EQUAL(reconverted.nanos(), 0);
    BOOST_CHECK_EQUAL(reconverted.seconds(), 0);
}

BOOST_AUTO_TEST_CASE(test_rounding_negative) {
    Duration input;
    input.set_nanos(-100);
    input.set_seconds(0);
    auto converted = from_proto(input);
    BOOST_CHECK_EQUAL(converted.count(), -1);
    auto reconverted = to_proto(converted);
    BOOST_CHECK_EQUAL(reconverted.nanos(), -1000);
    BOOST_CHECK_EQUAL(reconverted.seconds(), 0);
}

BOOST_AUTO_TEST_CASE(test_rounding_positive) {
    Duration input;
    input.set_nanos(999);
    input.set_seconds(0);
    auto converted = from_proto(input);
    BOOST_CHECK_EQUAL(converted.count(), 1);
    auto reconverted = to_proto(converted);
    BOOST_CHECK_EQUAL(reconverted.nanos(), 1000);
    BOOST_CHECK_EQUAL(reconverted.seconds(), 0);
}

BOOST_AUTO_TEST_CASE(test_mixed_sign_rounding) {
    Duration input;
    // Should round to -1 μs
    input.set_nanos(-500);
    input.set_seconds(1);
    auto converted = from_proto(input);
    BOOST_CHECK_EQUAL(converted.count(), 1e6 - 1);
    auto reconverted = to_proto(converted);
    BOOST_CHECK_EQUAL(reconverted.nanos(), 1e9 - 1000);
    BOOST_CHECK_EQUAL(reconverted.seconds(), 0);
}

BOOST_AUTO_TEST_CASE(test_medium_positive) {
    Duration input;
    // Should round to 2 μs
    input.set_nanos(1500);
    input.set_seconds(1000);
    auto converted = from_proto(input);
    BOOST_CHECK_EQUAL(converted.count(), 1000 * 1e6 + 2);
    auto reconverted = to_proto(converted);
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
    auto converted = from_proto(input);
    BOOST_CHECK_EQUAL(converted.count(), 1e6 * max_seconds + 2);
    auto reconverted = to_proto(converted);
    BOOST_CHECK_EQUAL(reconverted.nanos(), 2000);
    BOOST_CHECK_EQUAL(reconverted.seconds(), max_seconds);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace sdktests
}  // namespace viam
