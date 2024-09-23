#define BOOST_TEST_MODULE test module test_proto_value
#include <viam/sdk/common/proto_value.hpp>

#include <memory>
#include <unordered_map>

#include <google/protobuf/struct.pb.h>

#include <boost/mp11/algorithm.hpp>
#include <boost/mp11/integer_sequence.hpp>
#include <boost/mp11/tuple.hpp>
#include <boost/test/included/unit_test.hpp>

#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {

using namespace viam::sdk;
using namespace boost::mp11;

using google::protobuf::Value;

BOOST_AUTO_TEST_SUITE(test_proto_value)

BOOST_AUTO_TEST_CASE(test_object_equality) {
    // null is always equal
    BOOST_CHECK(ProtoValue() == ProtoValue());
    BOOST_CHECK(ProtoValue().is_a<std::nullptr_t>());
    BOOST_CHECK(ProtoValue().is_null());

    BOOST_CHECK(!(ProtoValue() == ProtoValue(5)));
    BOOST_CHECK(!(ProtoValue(false) == ProtoValue(nullptr)));

    BOOST_CHECK(ProtoValue(5) == ProtoValue(5));
    BOOST_CHECK(!(ProtoValue(6) == ProtoValue(5)));

    // heterogeneous arithmetic types do not "inuitively" compare equal
    BOOST_CHECK(!(ProtoValue(false) == ProtoValue(0)));
    BOOST_CHECK(!(ProtoValue(0) == ProtoValue(0.0)));

    auto test_cases = std::make_tuple(
        std::make_pair(true, false),
        std::make_pair(6.0, 7.5),
        std::make_pair(std::string("string"), std::string("different")),
        std::make_pair(ProtoList({ProtoValue{"asdf"}}),
                       ProtoList({ProtoValue{"asdf"}, ProtoValue{true}, ProtoValue{12.3}})),
        std::make_pair(ProtoStruct({{"ab", "cd"}}),
                       ProtoStruct({{"elem1", 5.0},
                                    {"elem2", "str"},
                                    {"vec", ProtoList({ProtoValue{3.0}, ProtoValue{"str"}})}})));

    boost::mp11::tuple_for_each(test_cases, [](auto test_pair) {
        using test_type = typename decltype(test_pair)::first_type;

        constexpr auto kind = proto_value_details::kind<test_type>::type::value;
        BOOST_TEST_MESSAGE("Testing with kind " << kind);

        ProtoValue v1(test_pair.first);
        BOOST_CHECK(v1.kind() == kind);
        BOOST_CHECK(v1.is_a<test_type>());

        {
            const test_type* ptr = v1.get<test_type>();
            BOOST_REQUIRE(ptr);
            BOOST_CHECK(*ptr == test_pair.first);
        }

        ProtoValue v2(test_pair.first);
        BOOST_CHECK(v1 == v2);

        ProtoValue v3(test_pair.second);
        ProtoValue v4(test_pair.second);

        BOOST_CHECK(!(v1 == v3));
        BOOST_CHECK(v3 == v4);

        // test copy construction
        auto v1_copy = v1;
        BOOST_CHECK(v1_copy == v1);

        {
            // mutate through copy
            auto v1_copy_to_change = v1;
            BOOST_CHECK(v1_copy_to_change == v1);

            test_type* ptr = v1_copy_to_change.get<test_type>();
            BOOST_REQUIRE(ptr);
            *ptr = test_pair.second;
            BOOST_CHECK(!(v1_copy_to_change == v1));
            BOOST_CHECK(v1_copy_to_change == v3);
        }

        Value converted = to_proto(v3);
        auto roundtrip = ProtoValue::from_proto(converted);
        Value value_roundtrip = to_proto(roundtrip);

        BOOST_CHECK(v3.kind() == roundtrip.kind());

        auto string1 = converted.ShortDebugString();
        auto string2 = value_roundtrip.ShortDebugString();
        BOOST_TEST_INFO("Converted strings: " << string1 << ", " << string2);

        BOOST_CHECK(v3 == roundtrip);
        BOOST_CHECK(string1 == string2);

        auto v1_move = std::move(v1);
        BOOST_CHECK(v1_copy == v1_move);
        BOOST_CHECK(v1.is_a<test_type>());
    });
}

BOOST_AUTO_TEST_CASE(test_move_validity) {
    auto test_cases =
        std::make_tuple(std::string("str"),
                        ProtoList{{ProtoValue("asdf"), ProtoValue(true)}},
                        ProtoStruct{{"asdf", true}, {"vec", ProtoList{{ProtoValue(5)}}}});

    tuple_for_each(test_cases, [](auto test_elem) {
        using TestType = decltype(test_elem);

        ProtoValue move_construct_src(test_elem);
        ProtoValue move_assign_src(test_elem);

        ProtoValue move_construct(std::move(move_construct_src));
        ProtoValue move_assign;
        move_assign = std::move(move_assign_src);

        // in practice pretty much any implementation would set the containers to empty, but we just
        // want to check that they are in an unspecified but valid state bc that is all that is
        // guaranteed

        for (TestType* ptr : {move_construct.get<TestType>(), move_assign.get<TestType>()}) {
            BOOST_REQUIRE(ptr);
            BOOST_CHECK(*ptr == test_elem);
        }

        for (TestType* moved_from_ptr :
             {move_construct.get<TestType>(), move_assign.get<TestType>()}) {
            BOOST_REQUIRE(moved_from_ptr);
            auto sample_value = *test_elem.begin();
            moved_from_ptr->clear();
            moved_from_ptr->insert(moved_from_ptr->end(), sample_value);
            BOOST_CHECK(moved_from_ptr->size() == 1);
        }
    });
}

BOOST_AUTO_TEST_CASE(test_unchecked_access) {
    auto scalar_tests = std::make_tuple(
        std::make_pair(false, true), std::make_pair(5, 6), std::make_pair(8.0, 9.5));

    auto nonscalar_tests = std::make_tuple(
        std::make_pair(std::string("s1"), std::string("s2")),
        std::make_pair(ProtoList{{ProtoValue(1), ProtoValue("asdf")}},
                       ProtoList{{ProtoValue(false), ProtoValue(5.0)}}),
        std::make_pair(ProtoStruct{{"asdf", true}, {"vec", ProtoList{{ProtoValue(5)}}}},
                       ProtoStruct{{"int", 5}, {"double", 6.0}}));

    tuple_for_each(std::tuple_cat(scalar_tests, nonscalar_tests), [](auto test_pair) {
        using first_type = typename decltype(test_pair)::first_type;
        using test_type = std::conditional_t<std::is_same<first_type, int>{}, double, first_type>;

        const test_type first = test_pair.first;
        const test_type second = test_pair.second;

        const ProtoValue const_val(first);
        BOOST_CHECK(const_val.get_unchecked<test_type>() == first);
        BOOST_CHECK(ProtoValue(first).get_unchecked<test_type>() == first);
        ProtoValue mut_val(second);
        BOOST_CHECK(mut_val.get_unchecked<test_type>() == second);
        mut_val.get_unchecked<test_type>() = first;
        BOOST_CHECK(mut_val.get_unchecked<test_type>() == first);
    });

    tuple_for_each(nonscalar_tests, [](auto test_pair) {
        using test_type = typename decltype(test_pair)::first_type;
        const test_type first = test_pair.first;

        ProtoValue to_move(first);

        const test_type from_move(std::move(to_move).get_unchecked<test_type>());
        BOOST_CHECK(from_move == first);
    });

    {
        // std::vector is the only of these containers with a guaranteed post-condition on move, so
        // we can use it to check that the rvalue overload is in fact being called
        ProtoList vec({{ProtoValue("asdf")}});

        ProtoValue vec_proto(vec);
        BOOST_CHECK(!vec_proto.get_unchecked<ProtoList>().empty());

        auto vec2 = std::move(vec_proto).get_unchecked<ProtoList>();
        BOOST_CHECK(vec2 == vec);
        BOOST_CHECK(vec_proto.get_unchecked<ProtoList>().empty());
    }
}

BOOST_AUTO_TEST_CASE(test_nested_objects) {
    std::unordered_map<std::string, ProtoValue> map;
    map.insert({"double", 1.0});

    ProtoList vec;
    vec.push_back(12.0);
    vec.push_back("asdf");

    std::unordered_map<std::string, ProtoValue> nest;
    nest.insert({"asdf", "ghjk"});
    nest.insert({"copyvec", vec});

    vec.push_back(nest);

    map.insert({"vec", vec});

    ProtoValue map_proto(map);

    Value val = to_proto(map_proto);
    auto roundtrip = ProtoValue::from_proto(val);
    Value val2 = to_proto(roundtrip);

    BOOST_CHECK(map_proto == roundtrip);

    BOOST_CHECK(val.ShortDebugString() == val2.ShortDebugString());
}

void set_proto_value(Value& val, bool b) {
    val.set_bool_value(b);
}
void set_proto_value(Value& val, double d) {
    val.set_number_value(d);
}
void set_proto_value(Value& val, std::string s) {
    val.set_string_value(std::move(s));
}

BOOST_AUTO_TEST_CASE(test_manual_list_conversion) {
    auto test_cases = std::make_tuple(std::string("string"), 3.0, false);

    boost::mp11::tuple_for_each(test_cases, [](auto test_val) {
        ProtoValue val(test_val);

        Value protoval;
        set_proto_value(protoval, test_val);

        auto from_value = ProtoValue::from_proto(protoval);
        BOOST_CHECK(val == from_value);

        Value converted_to_value = to_proto(val);
        BOOST_CHECK(protoval.ShortDebugString() == converted_to_value.ShortDebugString());

        auto roundtrip = ProtoValue::from_proto(converted_to_value);

        BOOST_CHECK(val == roundtrip);
    });

    ProtoList proto_vec;
    google::protobuf::ListValue lv;

    boost::mp11::tuple_for_each(test_cases, [&](auto test_val) {
        proto_vec.push_back(test_val);

        Value val;
        set_proto_value(val, test_val);
        *lv.add_values() = val;
    });

    ProtoValue proto(proto_vec);

    {
        // check that we can cast and type check vector elements
        const auto* ptr = proto.get<ProtoList>();
        BOOST_REQUIRE(ptr);

        using TupleType = decltype(test_cases);

        mp_for_each<mp_iota_c<mp_size<TupleType>{}>>([&](auto I) {
            auto tuple_elem = std::get<I()>(test_cases);
            const ProtoValue& elem = (*ptr)[I()];
            const auto* elem_p = elem.get<decltype(tuple_elem)>();
            BOOST_REQUIRE(elem_p);
            BOOST_CHECK(*elem_p == tuple_elem);
        });
    }

    Value v;
    *v.mutable_list_value() = lv;

    Value value_from_proto = to_proto(proto);
    BOOST_CHECK(v.ShortDebugString() == value_from_proto.ShortDebugString());

    auto roundtrip = ProtoValue::from_proto(value_from_proto);
    BOOST_CHECK(proto == roundtrip);
}

BOOST_AUTO_TEST_CASE(test_manual_map_conversion) {
    auto test_case = std::make_tuple(std::make_pair("string", std::string("s")),
                                     std::make_pair("double", 3.0),
                                     std::make_pair("bool", true));

    ProtoStruct m;
    google::protobuf::Map<std::string, Value> proto_map;

    boost::mp11::tuple_for_each(test_case, [&](auto map_pair) {
        m.insert(map_pair);
        Value v;

        set_proto_value(v, map_pair.second);
        proto_map.insert({{map_pair.first, v}});
    });

    google::protobuf::Struct proto_struct;
    *proto_struct.mutable_fields() = proto_map;
    Value v;
    *v.mutable_struct_value() = proto_struct;

    auto from_proto = ProtoValue::from_proto(v);
    ProtoValue from_map(m);
    BOOST_CHECK(from_proto == from_map);

    const ProtoStruct* ptr = from_map.get<ProtoStruct>();
    BOOST_REQUIRE(ptr);

    using TupleType = decltype(test_case);
    mp_for_each<mp_iota_c<mp_size<TupleType>{}>>([&](auto I) {
        auto map_pair = std::get<I>(test_case);

        const ProtoValue& map_elem = ptr->at(map_pair.first);
        const auto* elem_ptr = map_elem.get<decltype(map_pair.second)>();
        BOOST_REQUIRE(elem_ptr);
        BOOST_CHECK(*elem_ptr == map_pair.second);
    });
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace sdktests
}  // namespace viam
