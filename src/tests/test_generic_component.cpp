#define BOOST_TEST_MODULE test module test_generic_component
#include <common/v1/common.pb.h>

#include <boost/test/included/unit_test.hpp>
#include <components/generic/client.hpp>
#include <components/generic/generic.hpp>
#include <components/generic/server.hpp>
#include <config/resource.hpp>
#include <tests/mocks/generic_mocks.hpp>
#include <tests/test_utils.hpp>
#include <typeinfo>
#include <unordered_map>
#include <utility>

BOOST_AUTO_TEST_SUITE(generic_suite)

std::shared_ptr<MockGeneric> generic = MockGeneric::get_mock_generic();

BOOST_AUTO_TEST_CASE(test_do) {
  AttributeMap expected = fake_map();

  AttributeMap command;
  AttributeMap result_map = generic->do_command(command);

  std::shared_ptr<ProtoType> expected_pt = expected->at(std::string("test"));
  std::shared_ptr<ProtoType> result_pt = result_map->at(std::string("test"));

  BOOST_CHECK(*expected_pt == *result_pt);
}

BOOST_AUTO_TEST_CASE(test_do_service) {

  MockGenericStub mock = MockGenericStub();

  viam::common::v1::DoCommandRequest req;
  viam::common::v1::DoCommandResponse resp;
  grpc::ClientContext ctx;

  AttributeMap command = fake_map();

  *req.mutable_command() = map_to_struct(command);
  *req.mutable_name() = "generic";

  AttributeMap expected_map = fake_map();

  grpc::Status status = mock.DoCommand(&ctx, req, &resp);

  AttributeMap result_map = struct_to_map(resp.result());

  ProtoType expected_pt = *(expected_map->at(std::string("test")));
  ProtoType result_pt = *(result_map->at(std::string("test")));

  BOOST_CHECK(expected_pt == result_pt);
}

BOOST_AUTO_TEST_CASE(test_do_client) {
  MockGenericClient client = MockGenericClient("generic");

  AttributeMap command = fake_map();

  AttributeMap expected_map = fake_map();

  AttributeMap result_map = client.do_command(command);

  ProtoType expected_pt = *(expected_map->at(std::string("test")));
  ProtoType result_pt = *(result_map->at(std::string("test")));

  BOOST_CHECK(expected_pt == result_pt);
}

BOOST_AUTO_TEST_CASE(test3) {
    int i = 1;
    BOOST_CHECK(i + i == 3);
}

BOOST_AUTO_TEST_SUITE_END()
