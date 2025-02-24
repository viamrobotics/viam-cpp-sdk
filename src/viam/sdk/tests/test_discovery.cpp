#define BOOST_TEST_MODULE test module test_discovery
#include <viam/sdk/services/discovery.hpp>

#include <boost/optional/optional_io.hpp>
#include <boost/qvm/all.hpp>
#include <boost/test/included/unit_test.hpp>

#include <viam/sdk/tests/mocks/mock_discovery.hpp>
#include <viam/sdk/tests/test_utils.hpp>

BOOST_TEST_DONT_PRINT_LOG_VALUE(std::vector<viam::sdk::ResourceConfig>)

namespace viam {
namespace sdktests {
namespace discovery {

using namespace viam::sdk;

BOOST_AUTO_TEST_CASE(test_discover_resources) {
    auto mock = std::make_shared<MockDiscovery>("mock_discovery");
    client_to_mock_pipeline<Discovery>(mock, [](Discovery& client) {
        const auto& resources = client.discover_resources();
        BOOST_CHECK_EQUAL(resources.size(), 3);
    });
}

BOOST_AUTO_TEST_CASE(test_do_command) {
    auto mock = std::make_shared<MockDiscovery>("mock_discovery");
    client_to_mock_pipeline<Discovery>(mock, [&](Discovery& client) {
        ProtoStruct expected = fake_map();

        ProtoStruct command = fake_map();
        ProtoStruct result_map = client.do_command(command);

        BOOST_CHECK(result_map.at("test") == expected.at("test"));
    });
}

}  // namespace discovery
}  // namespace sdktests
}  // namespace viam
