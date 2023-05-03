#define BOOST_TEST_MODULE test module test_generic_component

#include <typeinfo>
#include <unordered_map>
#include <utility>

#include <boost/test/included/unit_test.hpp>

#include <viam/api/common/v1/common.pb.h>

#include <viam/sdk/components/generic/client.hpp>
#include <viam/sdk/components/generic/generic.hpp>
#include <viam/sdk/components/generic/server.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/tests/mocks/generic_mocks.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {

using namespace generic;

using namespace viam::sdk;

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
    server->resource_manager()->add(std::string("mock_generic"), MockGeneric::get_mock_generic());

    viam::common::v1::DoCommandRequest req;
    viam::common::v1::DoCommandResponse resp;
    grpc::ServerContext ctx;

    *req.mutable_name() = "mock_generic";
    grpc::Status status = server->DoCommand(&ctx, &req, &resp);

    AttributeMap result_map = struct_to_map(resp.result());
    AttributeMap expected_map = fake_map();

    ProtoType expected_pt = *(expected_map->at(std::string("test")));
    ProtoType result_pt = *(result_map->at(std::string("test")));

    BOOST_CHECK(expected_pt == result_pt);
}

BOOST_AUTO_TEST_CASE(test_do_client) {
    MockGenericClient client("mock_generic");

    AttributeMap command = fake_map();
    AttributeMap expected_map = fake_map();
    AttributeMap result_map = client.do_command(command);

    ProtoType expected_pt = *(expected_map->at(std::string("test")));
    ProtoType result_pt = *(result_map->at(std::string("test")));

    BOOST_CHECK(expected_pt == result_pt);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace sdktests
}  // namespace viam
