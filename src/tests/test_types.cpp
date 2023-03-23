#define BOOST_TEST_MODULE test module test_types
#include <boost/test/included/unit_test.hpp>
#include <common/proto_type.hpp>
#include <config/resource.hpp>
#include <unordered_map>

BOOST_AUTO_TEST_SUITE(test_prototype)

BOOST_AUTO_TEST_CASE(test_prototype_equality) {
    std::shared_ptr<ProtoType> proto_ptr =
        std::make_shared<ProtoType>(std::move(std::string("hello")));
    std::unordered_map<std::string, std::shared_ptr<ProtoType>> map = {
        {std::string("test"), proto_ptr}};

    AttributeMap expected_map =
        std::make_shared<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>(map);

    ProtoType type1 = ProtoType(expected_map);

    std::shared_ptr<ProtoType> proto_ptr2 =
        std::make_shared<ProtoType>(std::move(std::string("hello")));
    std::unordered_map<std::string, std::shared_ptr<ProtoType>> map2 = {
        {std::string("test"), proto_ptr2}};

    AttributeMap expected_map2 =
        std::make_shared<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>(map2);

    ProtoType type2 = ProtoType(expected_map2);

    BOOST_CHECK(type1 == type2);

    std::unordered_map<std::string, std::shared_ptr<ProtoType>> map3 = {
        {std::string("test"), std::make_shared<ProtoType>(std::move(std::string("not hello")))}};

    AttributeMap unequal_map =
        std::make_shared<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>(map3);

    ProtoType type3 = ProtoType(unequal_map);

    BOOST_CHECK(!(type1 == type3));
}
BOOST_AUTO_TEST_SUITE_END()
