#define BOOST_TEST_MODULE test module test_types
#include <boost/test/included/unit_test.hpp>
#include <common/proto_type.hpp>
#include <config/resource.hpp>
#include <tests/test_utils.hpp>
#include <unordered_map>

BOOST_AUTO_TEST_SUITE(test_prototype)

BOOST_AUTO_TEST_CASE(test_prototype_equality) {

  AttributeMap expected_map = fake_map();

  ProtoType type1 = ProtoType(expected_map);

  AttributeMap expected_map2 = fake_map();

  ProtoType type2 = ProtoType(expected_map2);

  BOOST_CHECK(type1 == type2);

  std::shared_ptr<ProtoType> proto_ptr =
      std::make_shared<ProtoType>(std::move(std::string("not hello")));
  AttributeMap unequal_map = std::make_shared<
      std::unordered_map<std::string, std::shared_ptr<ProtoType>>>();
  unequal_map->insert({{std::string("test"), proto_ptr}});

  ProtoType type3 = ProtoType(unequal_map);

  BOOST_CHECK(!(type1 == type3));
}
BOOST_AUTO_TEST_SUITE_END()
