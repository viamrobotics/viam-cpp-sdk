#define BOOST_TEST_MODULE test module test_resource
#include <boost/test/included/unit_test.hpp>

#include <viam/sdk/common/pose.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/referenceframe/frame.hpp>
#include <viam/sdk/resource/resource_api.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>
#include <viam/sdk/spatialmath/orientation.hpp>
#include <viam/sdk/spatialmath/orientation_types.hpp>

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
    // TODO: Currently, remotes not being returned from proto for whatever reason
    // BOOST_CHECK_EQUAL(Name::from_proto(name1.to_proto()), name1);

    Name name2 = Name::from_string("ns:component:st/name");
    BOOST_CHECK_EQUAL(name2.api().to_string(), "ns:component:st");
    BOOST_CHECK_EQUAL(name2.short_name(), "name");
    BOOST_CHECK_EQUAL(Name::from_proto(name2.to_proto()), name2);

    BOOST_CHECK_THROW(Name::from_string("ns:service:#st/remote:name"), std::string);
}

BOOST_AUTO_TEST_CASE(test_model) {
    ModelFamily mf("ns", "mf");
    BOOST_CHECK_EQUAL(mf.to_string(), "ns:mf");

    Model model1(mf, "model1");
    BOOST_CHECK_EQUAL(model1.to_string(), "ns:mf:model1");
    Model model2("ns", "mf", "model2");
    BOOST_CHECK_EQUAL(model2.to_string(), "ns:mf:model2");

    Model model3 = Model::from_str("ns:mf:model3");
    BOOST_CHECK_EQUAL(model3.to_string(), "ns:mf:model3");
    Model model4 = Model::from_str("model4");
    BOOST_CHECK_EQUAL(model4.to_string(), "rdk:builtin:model4");

    ModelFamily empty("", "");
    Model model5(empty, "model5");
    BOOST_CHECK_EQUAL(model5.to_string(), "model5");

    BOOST_CHECK_THROW(Model::from_str("@"), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(test_linkconfig) {
    translation t;
    t.x = 0;
    t.y = 1;
    t.z = 2;
    OrientationConfig ocfg = OrientationConfig();
    GeometryConfig gcfg;
    struct pose pose;
    pose.coordinates.x = 3;
    pose.coordinates.y = 4;
    pose.coordinates.z = 5;
    pose.orientation.o_x = 6;
    pose.orientation.o_y = 7;
    pose.orientation.o_z = 8;
    pose.theta = 3;
    gcfg.set_pose(pose);
    gcfg.set_label("label");
    gcfg.set_geometry_type(GeometryType::box);
    struct box box;
    box.x = 9;
    box.y = 10;
    box.z = 11;
    gcfg.set_geometry_specifics(box);

    LinkConfig lc;
    lc.set_orientation_config(ocfg);
    lc.set_parent("parent");
    BOOST_CHECK_EQUAL(lc.get_parent(), "parent");
    lc.set_translation(t);
    BOOST_CHECK_EQUAL(lc.get_translation().x, t.x);
    BOOST_CHECK_EQUAL(lc.get_translation().y, t.y);
    BOOST_CHECK_EQUAL(lc.get_translation().z, t.z);
    lc.set_geometry_config(gcfg);
    GeometryConfig result_gcfg = lc.get_geometry_config();
    BOOST_CHECK_EQUAL(result_gcfg.get_label(), "label");
    BOOST_CHECK_EQUAL(result_gcfg.get_pose(), pose);
    BOOST_CHECK_EQUAL(result_gcfg.get_geometry_type(), GeometryType::box);
    BOOST_CHECK_EQUAL(result_gcfg.get_geometry_specifics().which(),
                      gcfg.get_geometry_specifics().which());
    const auto result_gs = boost::get<struct box>(result_gcfg.get_geometry_specifics());
    BOOST_CHECK_EQUAL(result_gs.x, box.x);
    BOOST_CHECK_EQUAL(result_gs.y, box.y);
    BOOST_CHECK_EQUAL(result_gs.z, box.z);

}

}  // namespace sdktests
}  // namespace viam
