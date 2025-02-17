#define BOOST_TEST_MODULE test module test_discovery
#include <viam/sdk/components/discovery.hpp>

#include <boost/optional/optional_io.hpp>
#include <boost/qvm/all.hpp>
#include <boost/test/included/unit_test.hpp>

#include <viam/sdk/tests/mocks/mock_discovery.hpp>
#include <viam/sdk/tests/test_utils.hpp>

BOOST_TEST_DONT_PRINT_LOG_VALUE(std::vector<viam::sdk::ResourceConfig>)

namespace viam {
namespace sdktests {

using namespace viam::sdk;

BOOST_AUTO_TEST_SUITE(test_discovery)

BOOST_AUTO_TEST_CASE(mock_get_api) {
    const MockDiscovery discovery("mock_discovery");
    auto api = discovery.api();
    auto static_api = API::get<Discovery>();

    BOOST_CHECK_EQUAL(api, static_api);
    BOOST_CHECK_EQUAL(static_api.resource_subtype(), "discovery");
}

BOOST_AUTO_TEST_CASE(test_discover_resources) {
    std::shared_ptr<MockDiscovery> mock = MockDiscovery::get_mock_discovery();
    client_to_mock_pipeline<Discovery>(mock, [](Discovery& client) {
        const auto& resources = client.discover_resources();
        BOOST_CHECK_EQUAL(resource, fake_discovered_resources());
    });
}

BOOST_AUTO_TEST_CASE(test_do_command) {
    std::shared_ptr<MockDiscovery> mock = MockDiscovery::get_mock_discovery();
    client_to_mock_pipeline<Discovery>(mock, [](Discovery& client) {
        ProtoStruct expected = fake_map();

        ProtoStruct command = fake_map();
        ProtoStruc result_map = client.do_command(command);

        BOOST_CHECK(result.map.at("test") == expected.map.at("test"));
    });
}

}  // namespace sdktests
}  // namespace viam
