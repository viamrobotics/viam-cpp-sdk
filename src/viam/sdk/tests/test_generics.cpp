#define BOOST_TEST_MODULE test module test_generic_component

#include <typeinfo>
#include <unordered_map>
#include <utility>

#include <boost/test/included/unit_test.hpp>

#include <viam/sdk/components/generic.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>
#include <viam/sdk/tests/mocks/generic_mocks.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {

using namespace generic;

using namespace viam::sdk;

BOOST_AUTO_TEST_SUITE(generic_suite)

BOOST_AUTO_TEST_CASE(mock_get_component_api) {
    const auto generic = MockGenericComponent::get_mock_generic();
    auto api = generic->api();
    auto static_api = API::get<GenericComponent>();

    BOOST_CHECK_EQUAL(api, static_api);
    BOOST_CHECK_EQUAL(static_api.resource_subtype(), "generic");
}

BOOST_AUTO_TEST_CASE(test_component_do_command) {
    std::shared_ptr<MockGenericComponent> mock = MockGenericComponent::get_mock_generic();
    client_to_mock_pipeline<GenericComponent>(mock, [](GenericComponent& client) {
        AttributeMap expected = fake_map();
        AttributeMap command;
        AttributeMap result_map = client.do_command(command);

        std::shared_ptr<ProtoType> expected_pt = expected->at(std::string("test"));
        std::shared_ptr<ProtoType> result_pt = result_map->at(std::string("test"));

        BOOST_CHECK(*expected_pt == *result_pt);
    });
}

BOOST_AUTO_TEST_CASE(test_component_get_geometries) {
    std::shared_ptr<MockGenericComponent> mock = MockGenericComponent::get_mock_generic();
    client_to_mock_pipeline<GenericComponent>(mock, [](GenericComponent& client) {
        std::vector<GeometryConfig> expected_geometries = fake_geometries();
        std::vector<GeometryConfig> result_geometries = client.get_geometries(fake_map());

        BOOST_CHECK(expected_geometries == result_geometries);
    });
}

BOOST_AUTO_TEST_CASE(mock_get_service_api) {
    const auto generic = MockGenericService::get_mock_generic();
    auto api = generic->api();
    auto static_api = API::get<GenericService>();

    BOOST_CHECK_EQUAL(api, static_api);
    BOOST_CHECK_EQUAL(static_api.resource_subtype(), "generic");
}

BOOST_AUTO_TEST_CASE(test_service_do_command) {
    std::shared_ptr<MockGenericService> mock = MockGenericService::get_mock_generic();
    client_to_mock_pipeline<GenericService>(mock, [](GenericService& client) {
        AttributeMap expected = fake_map();
        AttributeMap command;
        AttributeMap result_map = client.do_command(command);

        std::shared_ptr<ProtoType> expected_pt = expected->at(std::string("test"));
        std::shared_ptr<ProtoType> result_pt = result_map->at(std::string("test"));

        BOOST_CHECK(*expected_pt == *result_pt);
    });
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace sdktests
}  // namespace viam
