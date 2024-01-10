#define BOOST_TEST_MODULE test module test_resource
#include <boost/test/included/unit_test.hpp>

#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/resource/resource_api.hpp>

namespace viam {
namespace sdktests {

using namespace viam::sdk;

BOOST_AUTO_TEST_CASE(test_api) {
    API api1;
    api1.set_namespace("ns");
    BOOST_CHECK_EQUAL(api1.type_namespace(), "ns");
    api1.set_resource_type("component");
    BOOST_CHECK_EQUAL(api1.resource_type(), "component");
    api1.set_resource_subtype("st");
    BOOST_CHECK_EQUAL(api1.resource_subtype(), "st");
    BOOST_CHECK_EQUAL(api1.to_string(), "ns:component:st");
    BOOST_CHECK(!api1.is_service_type());
    BOOST_CHECK(api1.is_component_type());

    API api2 = API::from_string("ns:service:st");
    BOOST_CHECK_EQUAL(api2.to_string(), "ns:service:st");
    BOOST_CHECK(api2.is_service_type());
    BOOST_CHECK(!api2.is_component_type());

    BOOST_CHECK_THROW(API::from_string("nsservicest"), std::string);
    BOOST_CHECK_THROW(API::from_string("ns:service:#st"), std::string);
    BOOST_CHECK_THROW(API::from_string("ns:service"), std::string);
}

BOOST_AUTO_TEST_CASE(test_name) {
    Name name1(API::from_string("ns:service:st"), "remote", "name");
    BOOST_CHECK_EQUAL(name1.api().to_string(), "ns:service:st");
    BOOST_CHECK_EQUAL(name1.remote_name(), "remote");
    BOOST_CHECK_EQUAL(name1.name(), "name");
    BOOST_CHECK_EQUAL(name1.to_string(), "ns:service:st/remote:name");
    BOOST_CHECK_EQUAL(name1.short_name(), "remote:name");

    Name name2 = Name::from_string("ns:component:st/name");
    BOOST_CHECK_EQUAL(name2.api().to_string(), "ns:component:st");
    BOOST_CHECK_EQUAL(name2.short_name(), "name");

    BOOST_CHECK_THROW(Name::from_string("ns:service:#st/remote:name"), std::string);
}

}  // namespace sdktests
}  // namespace viam
