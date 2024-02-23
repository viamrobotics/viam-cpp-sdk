#define BOOST_TEST_MODULE test module test_base
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/qvm/all.hpp>
#include <boost/test/included/unit_test.hpp>

#include <viam/sdk/common/linear_algebra.hpp>
#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/components/base.hpp>
#include <viam/sdk/tests/mocks/mock_base.hpp>
#include <viam/sdk/tests/test_utils.hpp>

BOOST_TEST_DONT_PRINT_LOG_VALUE(std::vector<viam::sdk::GeometryConfig>)

namespace viam {
namespace sdktests {

using namespace base;

using namespace viam::sdk;

using boost::qvm::operator-;
using boost::qvm::mag;

BOOST_AUTO_TEST_SUITE(test_base)

BOOST_AUTO_TEST_CASE(mock_get_api) {
    const MockBase base("mock_base");
    auto api = base.api();
    auto static_api = API::get<Base>();

    BOOST_CHECK_EQUAL(api, static_api);
    BOOST_CHECK_EQUAL(static_api.resource_subtype(), "base");
}

BOOST_AUTO_TEST_CASE(test_move_straight) {
    std::shared_ptr<MockBase> mock = MockBase::get_mock_base();
    client_to_mock_pipeline<Base>(mock, [&](Base& client) {
        client.move_straight(32, 0.75);
        BOOST_CHECK_EQUAL(mock->peek_move_straight_distance_mm, 32);
        BOOST_CHECK_EQUAL(mock->peek_move_straight_mm_per_sec, 0.75);
    });
}

BOOST_AUTO_TEST_CASE(test_spin) {
    std::shared_ptr<MockBase> mock = MockBase::get_mock_base();
    client_to_mock_pipeline<Base>(mock, [&](Base& client) {
        client.spin(57.1, -21.1);
        BOOST_CHECK_EQUAL(mock->peek_spin_angle_deg, 57.1);
        BOOST_CHECK_EQUAL(mock->peek_spin_degs_per_sec, -21.1);
    });
}

BOOST_AUTO_TEST_CASE(test_set_power) {
    std::shared_ptr<MockBase> mock = MockBase::get_mock_base();
    client_to_mock_pipeline<Base>(mock, [&](Base& client) {
        Vector3 linear = {0.1, -0.1, 1.0};
        Vector3 angular = {0.5, -1.0, 1.0};
        client.set_power(linear, angular);

        BOOST_CHECK_SMALL(mag(mock->peek_set_power_linear - linear), 0.01);
        BOOST_CHECK_SMALL(mag(mock->peek_set_power_angular - angular), 0.01);
    });
}

BOOST_AUTO_TEST_CASE(test_set_velocity) {
    std::shared_ptr<MockBase> mock = MockBase::get_mock_base();
    client_to_mock_pipeline<Base>(mock, [&](Base& client) {
        Vector3 linear = {0.1, -0.1, 1.0};
        Vector3 angular = {0.5, -1.0, 1.0};
        client.set_velocity(linear, angular);

        BOOST_CHECK_SMALL(mag(mock->peek_set_velocity_linear - linear), 0.01);
        BOOST_CHECK_SMALL(mag(mock->peek_set_velocity_angular - angular), 0.01);
    });
}

BOOST_AUTO_TEST_CASE(test_stop) {
    std::shared_ptr<MockBase> mock = MockBase::get_mock_base();
    client_to_mock_pipeline<Base>(mock, [&](Base& client) {
        mock->peek_stop_called = false;
        client.stop();
        BOOST_CHECK(mock->peek_stop_called);
    });
}

BOOST_AUTO_TEST_CASE(test_get_properties) {
    std::shared_ptr<MockBase> mock = MockBase::get_mock_base();
    client_to_mock_pipeline<Base>(mock, [](Base& client) {
        const auto& properties = client.get_properties();
        BOOST_CHECK_EQUAL(properties, fake_properties());
    });
}

BOOST_AUTO_TEST_CASE(test_get_geometries) {
    std::shared_ptr<MockBase> mock = MockBase::get_mock_base();
    client_to_mock_pipeline<Base>(mock, [](Base& client) {
        const auto& geometries = client.get_geometries();
        BOOST_CHECK_EQUAL(geometries, fake_geometries());
    });
}

BOOST_AUTO_TEST_CASE(test_is_moving) {
    std::shared_ptr<MockBase> mock = MockBase::get_mock_base();
    client_to_mock_pipeline<Base>(mock,
                                        [](Base& client) { BOOST_CHECK(!client.is_moving()); });
}

BOOST_AUTO_TEST_CASE(test_do_command) {
    std::shared_ptr<MockBase> mock = MockBase::get_mock_base();
    client_to_mock_pipeline<Base>(mock, [](Base& client) {
        AttributeMap expected = fake_map();

        AttributeMap command = fake_map();
        AttributeMap result_map = client.do_command(command);

        ProtoType expected_pt = *(expected->at(std::string("test")));
        ProtoType result_pt = *(result_map->at(std::string("test")));
        BOOST_CHECK(result_pt == expected_pt);
    });
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace sdktests
}  // namespace viam
