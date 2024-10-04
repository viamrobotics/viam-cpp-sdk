#define BOOST_TEST_MODULE test module test_complex_module

#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/test/included/unit_test.hpp>

#include <viam/api/common/v1/common.pb.h>

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/tests/test_utils.hpp>

#include "gizmo.grpc.pb.h"
#include "gizmo.pb.h"
#include "gizmo/api.hpp"
#include "gizmo/impl.hpp"
#include "summation.grpc.pb.h"
#include "summation.pb.h"
#include "summation/api.hpp"
#include "summation/impl.hpp"

using namespace viam::sdk;
using namespace viam::sdktests;

struct RegisterGizmoAndSummationFixture {
    RegisterGizmoAndSummationFixture() {
        Registry::register_resource<GizmoClient, GizmoServer>();
        Registry::register_resource<SummationClient, SummationServer>();
    }

    // Test teardown is a noop;
    ~RegisterGizmoAndSummationFixture() = default;
};

// Gizmo and Summation APIs must be registered before these tests are run
// (otherwise client_to_mock_pipeline will not be able to use their resource
// registrations to create the appropriate servers).
BOOST_TEST_GLOBAL_FIXTURE(RegisterGizmoAndSummationFixture);

BOOST_AUTO_TEST_SUITE(test_gizmo_impl)

// get_gizmo creates a MyGizmo for testing purposes named "testgizmo" with
// `arg1_` set to "foo".
std::shared_ptr<MyGizmo> get_gizmo() {
    return std::make_shared<MyGizmo>("testgizmo", "foo");
}

BOOST_AUTO_TEST_CASE(impl_do_one) {
    auto gizmo = get_gizmo();
    BOOST_CHECK(gizmo->do_one("foo"));
    BOOST_CHECK(!gizmo->do_one("bar"));
}

BOOST_AUTO_TEST_CASE(impl_do_one_client_stream) {
    auto gizmo = get_gizmo();
    BOOST_CHECK(gizmo->do_one_client_stream({"foo", "foo"}));
    BOOST_CHECK(!gizmo->do_one_client_stream({"foo", "bar"}));
}

BOOST_AUTO_TEST_CASE(impl_do_one_server_stream) {
    auto gizmo = get_gizmo();
    std::vector<bool> ret1 = {true, false, true, false};
    std::vector<bool> ret2 = {false, false, true, false};
    BOOST_CHECK(gizmo->do_one_server_stream("foo") == ret1);
    BOOST_CHECK(gizmo->do_one_server_stream("bar") == ret2);
}

BOOST_AUTO_TEST_CASE(impl_do_one_bidi_stream) {
    auto gizmo = get_gizmo();
    std::vector<bool> ret1 = {true, false};
    std::vector<bool> ret2 = {false, true};
    BOOST_CHECK(gizmo->do_one_bidi_stream({"foo", "bar"}) == ret1);
    BOOST_CHECK(gizmo->do_one_bidi_stream({"bar", "foo"}) == ret2);
}

BOOST_AUTO_TEST_CASE(impl_do_two) {
    auto gizmo = get_gizmo();
    BOOST_CHECK(gizmo->do_two(true) == "arg1=true");
    BOOST_CHECK(gizmo->do_two(false) == "arg1=false");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_gizmo_client_server)

BOOST_AUTO_TEST_CASE(test_do_one) {
    const auto mock = test_gizmo_impl::get_gizmo();
    client_to_mock_pipeline<Gizmo>(mock, [](Gizmo& client) {
        BOOST_CHECK(client.do_one("foo"));
        BOOST_CHECK(!client.do_one("bar"));
    });
}

BOOST_AUTO_TEST_CASE(test_do_one_client_stream) {
    const auto mock = test_gizmo_impl::get_gizmo();
    client_to_mock_pipeline<Gizmo>(mock, [](Gizmo& client) {
        BOOST_CHECK(client.do_one_client_stream({"foo", "foo"}));
        BOOST_CHECK(!client.do_one_client_stream({"foo", "bar"}));
    });
}

BOOST_AUTO_TEST_CASE(test_do_one_server_stream) {
    const auto mock = test_gizmo_impl::get_gizmo();
    client_to_mock_pipeline<Gizmo>(mock, [](Gizmo& client) {
        std::vector<bool> ret1 = {true, false, true, false};
        std::vector<bool> ret2 = {false, false, true, false};
        BOOST_CHECK(client.do_one_server_stream("foo") == ret1);
        BOOST_CHECK(client.do_one_server_stream("bar") == ret2);
    });
}

BOOST_AUTO_TEST_CASE(test_do_one_bidi_stream) {
    const auto mock = test_gizmo_impl::get_gizmo();
    client_to_mock_pipeline<Gizmo>(mock, [](Gizmo& client) {
        std::vector<bool> ret1 = {true, false};
        std::vector<bool> ret2 = {false, true};
        BOOST_CHECK(client.do_one_bidi_stream({"foo", "bar"}) == ret1);
        BOOST_CHECK(client.do_one_bidi_stream({"bar", "foo"}) == ret2);
    });
}

BOOST_AUTO_TEST_CASE(test_do_two) {
    const auto mock = test_gizmo_impl::get_gizmo();
    client_to_mock_pipeline<Gizmo>(mock, [](Gizmo& client) {
        BOOST_CHECK(client.do_two(true) == "arg1=true");
        BOOST_CHECK(client.do_two(false) == "arg1=false");
    });
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_summation_impl)

// get_summation creates a MySummation for testing purposes named "testsum"
// with `subtract_` set to false.
std::shared_ptr<MySummation> get_summation() {
    return std::make_shared<MySummation>("testsum", false);
}

BOOST_AUTO_TEST_CASE(impl_sum) {
    auto summation = get_summation();
    double sum = summation->sum({0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0});
    BOOST_CHECK(sum == 45);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_summation_client_server)

BOOST_AUTO_TEST_CASE(test_sum) {
    const auto mock = test_summation_impl::get_summation();
    client_to_mock_pipeline<Summation>(mock, [](Summation& client) {
        double sum = client.sum({0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0});
        BOOST_CHECK(sum == 45);
    });
}

BOOST_AUTO_TEST_SUITE_END()
