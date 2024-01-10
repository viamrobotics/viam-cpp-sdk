#define BOOST_TEST_MODULE test module test_resource
#include <boost/test/included/unit_test.hpp>

#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/resource/resource_api.hpp>

namespace viam {
namespace sdktests {

using namespace viam::sdk;

BOOST_AUTO_TEST_CASE(test_api) {
    API api;
    api.set_namespace("ns");
    BOOST_CHECK_EQUAL(api.type_namespace(), "ns");
    api.set_resource_type("component");
    BOOST_CHECK_EQUAL(api.resource_type(), "component");
    api.set_resource_subtype("st");
    BOOST_CHECK_EQUAL(api.resource_subtype(), "st");
    BOOST_CHECK_EQUAL(api.to_string(), "ns:component:st");
    BOOST_CHECK(!api.is_service_type());
    BOOST_CHECK(api.is_component_type());

    API api2 = API::from_string("ns:service:st");
    BOOST_CHECK_EQUAL(api2.to_string(), "ns:service:st");
    BOOST_CHECK(api2.is_service_type());
    BOOST_CHECK(!api2.is_component_type());

    BOOST_CHECK_THROW(API::from_string("nsservicest"), std::string);
}

}  // namespace sdktests
}  // namespace viam
