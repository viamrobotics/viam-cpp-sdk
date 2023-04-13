#define BOOST_TEST_MODULE test module test_generic_component

#include <typeinfo>
#include <unordered_map>
#include <utility>

#include <boost/test/included/unit_test.hpp>

#include <common/v1/common.pb.h>

#include <components/generic/client.hpp>
#include <components/generic/generic.hpp>
#include <components/generic/server.hpp>
#include <config/resource.hpp>
#include <tests/mocks/generic_mocks.hpp>
#include <tests/test_utils.hpp>

using namespace viam::cppsdk;

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
    auto server = std::make_shared<GenericServer>();
    server->get_sub_svc()->add(std::string("generic"), MockGeneric::get_mock_generic());

    viam::common::v1::DoCommandRequest req;
    viam::common::v1::DoCommandResponse resp;
    grpc::ServerContext ctx;

    *req.mutable_name() = "generic";
    grpc::Status status = server->DoCommand(&ctx, &req, &resp);

    AttributeMap result_map = struct_to_map(resp.result());
    AttributeMap expected_map = fake_map();

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

BOOST_AUTO_TEST_SUITE_END()
