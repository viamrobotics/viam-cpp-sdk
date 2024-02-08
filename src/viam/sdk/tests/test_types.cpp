#include <memory>
#define BOOST_TEST_MODULE test module test_types

#include <cctype>
#include <unordered_map>

#include <boost/test/included/unit_test.hpp>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {

using namespace viam::sdk;

BOOST_AUTO_TEST_SUITE(test_prototype)

BOOST_AUTO_TEST_CASE(test_prototype_equality) {
    AttributeMap expected_map = fake_map();

    ProtoType type1 = ProtoType(expected_map);

    AttributeMap expected_map2 = fake_map();

    ProtoType type2 = ProtoType(expected_map2);

    BOOST_CHECK(type1 == type2);

    auto proto_ptr = std::make_shared<ProtoType>(std::move(std::string("not hello")));
    AttributeMap unequal_map =
        std::make_shared<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>();
    unequal_map->insert({{std::string("test"), proto_ptr}});

    ProtoType type3 = ProtoType(unequal_map);

    BOOST_CHECK(!(type1 == type3));
}

BOOST_AUTO_TEST_CASE(test_prototype_list_conversion) {
    std::string s("string");
    double d(3);
    bool b(false);
    std::vector<std::shared_ptr<ProtoType>> proto_vec{std::make_shared<ProtoType>(d),
                                                      std::make_shared<ProtoType>(s),
                                                      std::make_shared<ProtoType>(b)};

    ProtoType proto(proto_vec);

    google::protobuf::Value double_value;
    double_value.set_number_value(d);
    google::protobuf::Value string_value;
    string_value.set_string_value(s);
    google::protobuf::Value bool_value;
    bool_value.set_bool_value(b);
    google::protobuf::ListValue lv;

    *lv.add_values() = double_value;
    *lv.add_values() = string_value;
    *lv.add_values() = bool_value;

    google::protobuf::Value v;
    *v.mutable_list_value() = lv;

    ProtoType proto_from_value(v);

    BOOST_CHECK_EQUAL(proto.proto_value().list_value().values_size(), 3);
    BOOST_CHECK(proto == proto_from_value);

    auto round_trip1 = proto.proto_value();
    auto round_trip2 = ProtoType(round_trip1);

    BOOST_CHECK(round_trip2 == proto);
}

BOOST_AUTO_TEST_CASE(test_prototype_map_conversion) {
    std::string s("string");
    double d(3);
    bool b(false);

    auto m = std::make_shared<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>();

    m->insert({{std::string("double"), std::make_shared<ProtoType>(d)}});
    m->insert({{std::string("bool"), std::make_shared<ProtoType>(b)}});
    m->insert({{std::string("string"), std::make_shared<ProtoType>(s)}});

    google::protobuf::Value double_value;
    double_value.set_number_value(d);
    google::protobuf::Value string_value;
    string_value.set_string_value(s);
    google::protobuf::Value bool_value;
    bool_value.set_bool_value(b);
    google::protobuf::Map<std::string, google::protobuf::Value> proto_map;
    proto_map.insert({{std::string("string"), string_value}});
    proto_map.insert({{std::string("double"), double_value}});
    proto_map.insert({{std::string("bool"), bool_value}});

    google::protobuf::Struct proto_struct;
    *proto_struct.mutable_fields() = proto_map;
    AttributeMap from_proto = struct_to_map(proto_struct);
    BOOST_CHECK_EQUAL(from_proto->size(), m->size());

    ProtoType proto(from_proto);
    ProtoType map(m);
    BOOST_CHECK(map == proto);

    auto round_trip1 = map_to_struct(m);
    auto round_trip2 = struct_to_map(round_trip1);

    ProtoType round_trip_proto(round_trip2);
    BOOST_CHECK(round_trip_proto == map);
}

std::string remove_whitespace(std::string s) {
    s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());
    return s;
}

BOOST_AUTO_TEST_CASE(test_discovery_query) {
    RobotClient::discovery_query query;
    query.subtype = "subtype";
    query.model = "model";

    BOOST_CHECK_EQUAL(query.subtype, "subtype");
    BOOST_CHECK_EQUAL(query.model, "model");

    RobotClient::discovery_query from_proto =
        RobotClient::discovery_query::from_proto(query.to_proto());
    BOOST_CHECK_EQUAL(query.subtype, from_proto.subtype);
    BOOST_CHECK_EQUAL(query.model, query.model);
    BOOST_CHECK(query == from_proto);
}

BOOST_AUTO_TEST_CASE(test_discovery) {
    RobotClient::discovery discovery;

    RobotClient::discovery_query query;
    query.subtype = "subtype";
    query.model = "model";
    discovery.query = query;

    AttributeMap results =
        std::make_shared<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>();
    google::protobuf::Value v;
    *v.mutable_string_value() = "bar";
    google::protobuf::MapPair<std::string, std::shared_ptr<ProtoType>> pair(
        std::move("foo"), std::make_shared<ProtoType>(v));
    results->emplace(pair);
    discovery.results = results;

    BOOST_CHECK_EQUAL(discovery.query.subtype, "subtype");
    BOOST_CHECK_EQUAL(discovery.query.model, "model");
    std::string results_str =
        remove_whitespace(map_to_struct(discovery.results).SerializeAsString());
    BOOST_CHECK_EQUAL(results_str, "foobar");

    // RobotClient::
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace sdktests
}  // namespace viam
