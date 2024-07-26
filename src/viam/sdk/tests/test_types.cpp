#include <memory>
#define BOOST_TEST_MODULE test module test_types

#include <unordered_map>

#include <boost/mp11/tuple.hpp>
#include <boost/test/included/unit_test.hpp>

#include <viam/sdk/common/proto_t.hpp>
#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {

using namespace viam::sdk;

using google::protobuf::Value;

BOOST_AUTO_TEST_SUITE(test_prototype)

BOOST_AUTO_TEST_CASE(test_object_equality) {
    // null is always equal
    BOOST_CHECK(ProtoT() == ProtoT());

    BOOST_CHECK(ProtoT(5) == ProtoT(5));
    BOOST_CHECK(!(ProtoT(6) == ProtoT(5)));

    // roundtrip integer conversion is not idempotent because the integer gets coerced to a double
    {
        ProtoT i5(5);
        ProtoT int_roundtrip(to_proto_value(i5));
        BOOST_CHECK(i5.kind() == kind_t<int>{});
        BOOST_CHECK(int_roundtrip.kind() == kind_t<double>{});
        BOOST_CHECK(!(i5 == int_roundtrip));
        BOOST_CHECK(int_roundtrip == ProtoT(5.0));
    }

    auto test_cases = std::make_tuple(
        std::make_pair(true, false),
        /* integer not included, see above */
        std::make_pair(6.0, 7.5),
        std::make_pair(std::string("string"), std::string("different")),
        std::make_pair(std::vector<ProtoT>({ProtoT{"asdf"}}),
                       std::vector<ProtoT>({ProtoT{"asdf"}, ProtoT{true}, ProtoT{12.3}})),
        std::make_pair(AttrMap({{"ab", "cd"}}),
                       AttrMap({{"elem1", 5.0},
                                {"elem2", "str"},
                                {"vec", std::vector<ProtoT>({ProtoT{3.0}, ProtoT{"str"}})}})));

    boost::mp11::tuple_for_each(test_cases, [](auto test_pair) {
        constexpr auto kind = kind_t<typename decltype(test_pair)::first_type>::value;
        BOOST_TEST_MESSAGE("Testing with kind " << kind);

        ProtoT v1(test_pair.first);
        BOOST_CHECK(v1.kind() == kind);

        ProtoT v2(test_pair.first);
        BOOST_CHECK(v1 == v2);

        ProtoT v3(test_pair.second);
        ProtoT v4(test_pair.second);

        BOOST_CHECK(!(v1 == v3));
        BOOST_CHECK(v3 == v4);

        Value converted = to_proto_value(v3);
        ProtoT roundtrip(converted);
        Value value_roundtrip = to_proto_value(roundtrip);

        BOOST_CHECK(v3.kind() == roundtrip.kind());

        auto string1 = converted.ShortDebugString();
        auto string2 = value_roundtrip.ShortDebugString();
        BOOST_TEST_INFO("Converted strings: " << string1 << ", " << string2);

        BOOST_CHECK(v3 == roundtrip);
        BOOST_CHECK(string1 == string2);
    });
}

BOOST_AUTO_TEST_CASE(test_nested_objects) {
    std::unordered_map<std::string, ProtoT> map;
    map.insert({"double", 1.0});

    std::vector<ProtoT> vec;
    vec.push_back(12.0);
    vec.push_back("asdf");

    std::unordered_map<std::string, ProtoT> nest;
    nest.insert({"asdf", "ghjk"});
    nest.insert({"copyvec", vec});

    vec.push_back(nest);

    map.insert({"vec", vec});

    ProtoT map_proto(map);

    Value val = to_proto_value(map_proto);
    ProtoT roundtrip{val};
    Value val2 = to_proto_value(roundtrip);

    BOOST_CHECK(map_proto == roundtrip);

    BOOST_CHECK(val.ShortDebugString() == val2.ShortDebugString());
}

BOOST_AUTO_TEST_CASE(test_manual_list_conversion) {
    auto test_cases = std::make_tuple(
        std::make_pair(std::string("string"), &Value::set_string_value<std::string>),
        std::make_pair(3.0, &Value::set_number_value),
        std::make_pair(false, &Value::set_bool_value));

    boost::mp11::tuple_for_each(test_cases, [](auto test_pair) {
        ProtoT val(test_pair.first);

        Value protoval;
        (protoval.*(test_pair.second))(std::move(test_pair.first));

        ProtoT from_value(protoval);
        BOOST_CHECK(val == from_value);

        Value converted_to_value = to_proto_value(val);
        BOOST_CHECK(protoval.ShortDebugString() == converted_to_value.ShortDebugString());

        ProtoT roundtrip(converted_to_value);

        BOOST_CHECK(val == roundtrip);
    });

    std::vector<ProtoT> proto_vec;
    google::protobuf::ListValue lv;

    boost::mp11::tuple_for_each(test_cases, [&](auto test_pair) {
        proto_vec.push_back(test_pair.first);

        Value val;
        (val.*(test_pair.second))(std::move(test_pair.first));
        *lv.add_values() = val;
    });

    ProtoT proto(proto_vec);
    Value v;
    *v.mutable_list_value() = lv;

    Value value_from_proto = to_proto_value(proto);
    BOOST_CHECK(v.ShortDebugString() == value_from_proto.ShortDebugString());

    ProtoT roundtrip(value_from_proto);
    BOOST_CHECK(proto == roundtrip);
}

BOOST_AUTO_TEST_CASE(test_manual_map_conversion) {
    auto test_case =
        std::make_tuple(std::make_pair(std::make_pair("string", std::string("s")),
                                       &Value::set_string_value<std::string>),
                        std::make_pair(std::make_pair("double", 3.0), &Value::set_number_value),
                        std::make_pair(std::make_pair("bool", true), &Value::set_bool_value));

    AttrMap m;
    google::protobuf::Map<std::string, Value> proto_map;

    boost::mp11::tuple_for_each(test_case, [&](auto test_pair) {
        m.insert(test_pair.first);
        Value v;

        (v.*(test_pair.second))(std::move(test_pair.first.second));
        proto_map.insert({{test_pair.first.first, v}});
    });

    google::protobuf::Struct proto_struct;
    *proto_struct.mutable_fields() = proto_map;
    Value v;
    *v.mutable_struct_value() = proto_struct;

    ProtoT from_proto(v);
    ProtoT from_map(m);
    BOOST_CHECK(v == m);
}

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

BOOST_AUTO_TEST_SUITE_END()

}  // namespace sdktests
}  // namespace viam
