#define BOOST_TEST_MODULE test module test_resource
#include <boost/test/included/unit_test.hpp>

#include <google/protobuf/struct.pb.h>
#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/pose.hpp>
#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/referenceframe/frame.hpp>
#include <viam/sdk/resource/resource_api.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>
#include <viam/sdk/spatialmath/orientation.hpp>
#include <viam/sdk/spatialmath/orientation_types.hpp>

namespace viam {
namespace sdktests {

using namespace viam::sdk;
using google::protobuf::Struct;
using google::protobuf::Value;

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
    BOOST_CHECK_EQUAL(api2.type_namespace(), "ns");
    BOOST_CHECK_EQUAL(api2.resource_subtype(), "st");
    BOOST_CHECK_EQUAL(api2.to_string(), "ns:service:st");
    BOOST_CHECK(api2.is_service_type());
    BOOST_CHECK(!api2.is_component_type());

    BOOST_CHECK_THROW(API::from_string("nsservicest"), ViamException);
    BOOST_CHECK_THROW(API::from_string("ns:service:#st"), ViamException);
    BOOST_CHECK_THROW(API::from_string("ns:service"), ViamException);
}

BOOST_AUTO_TEST_CASE(test_name) {
    Name name1(API::from_string("ns:service:st"), "remote", "name");
    BOOST_CHECK_EQUAL(name1.api().to_string(), "ns:service:st");
    BOOST_CHECK_EQUAL(name1.remote_name(), "remote");
    BOOST_CHECK_EQUAL(name1.name(), "name");
    BOOST_CHECK_EQUAL(name1.short_name(), "remote:name");
    BOOST_CHECK_EQUAL(name1.to_string(), "ns:service:st/remote:name");
    BOOST_CHECK_EQUAL(Name::from_proto(name1.to_proto()), name1);

    Name name2(API::from_string("ns:service:st"), "remote1:remote2", "name");
    BOOST_CHECK_EQUAL(name2.api().to_string(), "ns:service:st");
    BOOST_CHECK_EQUAL(name2.remote_name(), "remote1:remote2");
    BOOST_CHECK_EQUAL(name2.name(), "name");
    BOOST_CHECK_EQUAL(name2.short_name(), "remote1:remote2:name");
    BOOST_CHECK_EQUAL(name2.to_string(), "ns:service:st/remote1:remote2:name");
    BOOST_CHECK_EQUAL(Name::from_proto(name2.to_proto()), name2);

    Name name3 = Name::from_string("ns:component:st/name");
    BOOST_CHECK_EQUAL(name3.api().to_string(), "ns:component:st");
    BOOST_CHECK_EQUAL(name3.remote_name(), "");
    BOOST_CHECK_EQUAL(name3.name(), "name");
    BOOST_CHECK_EQUAL(name3.short_name(), "name");
    BOOST_CHECK_EQUAL(name3.to_string(), "ns:component:st/name");
    BOOST_CHECK_EQUAL(Name::from_proto(name3.to_proto()), name3);

    BOOST_CHECK_THROW(Name::from_string("ns:service:#st/remote:name"), ViamException);
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

    BOOST_CHECK_THROW(Model::from_str("@"), ViamException);
}

BOOST_AUTO_TEST_CASE(test_linkconfig) {
    viam::app::v1::Frame frame;

    viam::common::v1::Geometry g;
    viam::common::v1::Pose pose;
    pose.set_x(0);
    pose.set_y(1);
    pose.set_z(2);
    pose.set_o_x(3);
    pose.set_o_y(4);
    pose.set_o_z(5);
    pose.set_theta(6);
    viam::common::v1::RectangularPrism box;
    viam::common::v1::Vector3 vec3;
    vec3.set_x(7);
    vec3.set_y(8);
    vec3.set_z(9);
    *box.mutable_dims_mm() = vec3;
    *g.mutable_label() = "label";
    *g.mutable_center() = pose;
    *g.mutable_box() = box;

    viam::app::v1::Orientation o;
    viam::app::v1::Orientation_AxisAngles aa;
    aa.set_x(10);
    aa.set_y(11);
    aa.set_z(12);
    aa.set_theta(13);
    *o.mutable_axis_angles() = aa;

    viam::app::v1::Translation t;
    t.set_x(14);
    t.set_y(15);
    t.set_z(16);

    *frame.mutable_parent() = "parent";
    *frame.mutable_geometry() = g;
    *frame.mutable_orientation() = o;
    *frame.mutable_translation() = t;

    LinkConfig lc = LinkConfig::from_proto(frame);
    BOOST_CHECK_EQUAL(lc.get_parent(), "parent");
    BOOST_CHECK_EQUAL(lc.get_translation().x, t.x());
    BOOST_CHECK_EQUAL(lc.get_translation().y, t.y());
    BOOST_CHECK_EQUAL(lc.get_translation().z, t.z());
    GeometryConfig gcfg = lc.get_geometry_config();
    BOOST_CHECK_EQUAL(gcfg.get_label(), "label");
    BOOST_CHECK_EQUAL(gcfg.get_pose(), pose::from_proto(pose));
    BOOST_CHECK_EQUAL(gcfg.get_geometry_type(), GeometryType::box);
    const auto gs = gcfg.box_proto();
    BOOST_CHECK_EQUAL(gs.dims_mm().x(), box.dims_mm().x());
    BOOST_CHECK_EQUAL(gs.dims_mm().y(), box.dims_mm().y());
    BOOST_CHECK_EQUAL(gs.dims_mm().z(), box.dims_mm().z());

    viam::app::v1::Frame proto_lc = lc.to_proto();
    BOOST_CHECK_EQUAL(proto_lc.parent(), "parent");
    BOOST_CHECK_EQUAL(proto_lc.translation().x(), t.x());
    BOOST_CHECK_EQUAL(proto_lc.translation().y(), t.y());
    BOOST_CHECK_EQUAL(proto_lc.translation().z(), t.z());
    BOOST_CHECK(proto_lc.has_orientation());
    viam::app::v1::Orientation proto_ocfg = proto_lc.orientation();
    BOOST_CHECK_EQUAL(proto_ocfg.type_case(), viam::app::v1::Orientation::TypeCase::kAxisAngles);
    BOOST_CHECK_EQUAL(proto_ocfg.axis_angles().x(), o.axis_angles().x());
    BOOST_CHECK_EQUAL(proto_ocfg.axis_angles().y(), o.axis_angles().y());
    BOOST_CHECK_EQUAL(proto_ocfg.axis_angles().z(), o.axis_angles().z());
    BOOST_CHECK_EQUAL(proto_ocfg.axis_angles().theta(), o.axis_angles().theta());
    BOOST_CHECK(proto_lc.has_geometry());
    viam::common::v1::Geometry proto_gcfg = proto_lc.geometry();
    BOOST_CHECK_EQUAL(proto_gcfg.label(), "label");
    BOOST_CHECK_EQUAL(proto_gcfg.center().x(), pose.x());
    BOOST_CHECK_EQUAL(proto_gcfg.center().y(), pose.y());
    BOOST_CHECK_EQUAL(proto_gcfg.center().z(), pose.z());
    BOOST_CHECK_EQUAL(proto_gcfg.geometry_type_case(),
                      viam::common::v1::Geometry::GeometryTypeCase::kBox);
    BOOST_CHECK_EQUAL(proto_gcfg.box().dims_mm().x(), box.dims_mm().x());
    BOOST_CHECK_EQUAL(proto_gcfg.box().dims_mm().y(), box.dims_mm().y());
    BOOST_CHECK_EQUAL(proto_gcfg.box().dims_mm().z(), box.dims_mm().z());
}

BOOST_AUTO_TEST_CASE(test_resource) {
    ResourceConfig resource1("type");
    BOOST_CHECK_EQUAL(resource1.api().to_string(), "rdk:type:");
    BOOST_CHECK_EQUAL(resource1.frame().get_parent(), "");
    BOOST_CHECK_EQUAL(resource1.model().to_string(), "rdk:builtin:builtin");
    BOOST_CHECK_EQUAL(resource1.name(), "");
    BOOST_CHECK_EQUAL(resource1.namespace_(), "");
    BOOST_CHECK_EQUAL(resource1.type(), "type");
    BOOST_CHECK_EQUAL(resource1.resource_name().to_string(), "rdk:type:type/");

    viam::app::v1::ComponentConfig proto_cfg;
    *proto_cfg.mutable_name() = "name";
    *proto_cfg.mutable_namespace_() = "ns";
    *proto_cfg.mutable_type() = "type";
    *proto_cfg.mutable_api() = "ns:component:type";
    *proto_cfg.mutable_model() = "ns:mf:model1";
    Struct attributes;
    Value v;
    v.set_number_value(1);
    const google::protobuf::MapPair<std::string, Value> val("a", v);
    attributes.mutable_fields()->insert(val);
    *proto_cfg.mutable_attributes() = attributes;

    viam::app::v1::Frame frame;
    viam::common::v1::Geometry g;
    viam::common::v1::Pose pose;
    pose.set_x(0);
    pose.set_y(1);
    pose.set_z(2);
    pose.set_o_x(3);
    pose.set_o_y(4);
    pose.set_o_z(5);
    pose.set_theta(6);
    viam::common::v1::RectangularPrism box;
    viam::common::v1::Vector3 vec3;
    vec3.set_x(7);
    vec3.set_y(8);
    vec3.set_z(9);
    *box.mutable_dims_mm() = vec3;
    *g.mutable_label() = "label";
    *g.mutable_center() = pose;
    *g.mutable_box() = box;
    viam::app::v1::Orientation o;
    viam::app::v1::Orientation_AxisAngles aa;
    aa.set_x(10);
    aa.set_y(11);
    aa.set_z(12);
    aa.set_theta(13);
    *o.mutable_axis_angles() = aa;
    viam::app::v1::Translation t;
    t.set_x(14);
    t.set_y(15);
    t.set_z(16);
    *frame.mutable_parent() = "parent";
    *frame.mutable_geometry() = g;
    *frame.mutable_orientation() = o;
    *frame.mutable_translation() = t;
    *proto_cfg.mutable_frame() = frame;

    ResourceConfig resource2 = ResourceConfig::from_proto(proto_cfg);
    BOOST_CHECK_EQUAL(resource2.name(), "name");
    BOOST_CHECK_EQUAL(resource2.namespace_(), "ns");
    BOOST_CHECK_EQUAL(resource2.type(), "type");
    BOOST_CHECK_EQUAL(resource2.model().to_string(), "ns:mf:model1");
    BOOST_CHECK_EQUAL(resource2.api().to_string(), "ns:component:type");
    BOOST_CHECK_EQUAL(resource2.frame().get_parent(), "parent");
    std::string key;
    Value value;
    for (const auto& key_and_value : *resource2.attributes()) {
        key = key_and_value.first;
        value = key_and_value.second->proto_value();
    }
    BOOST_CHECK_EQUAL(key, "a");
    BOOST_CHECK_EQUAL(value.number_value(), 1);

    *proto_cfg.mutable_api() = "ns:component:test";
    BOOST_CHECK_THROW(ResourceConfig::from_proto(proto_cfg), ViamException);
}

}  // namespace sdktests
}  // namespace viam
