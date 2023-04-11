#define BOOST_TEST_MODULE test module test_base

#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/test/included/unit_test.hpp>

#include <common/v1/common.pb.h>
#include <component/base/v1/base.grpc.pb.h>
#include <component/base/v1/base.pb.h>

#include <common/proto_type.hpp>
#include <components/base/base.hpp>
#include <components/base/client.hpp>
#include <components/base/server.hpp>
#include <tests/mocks/base_mocks.hpp>
#include <tests/test_utils.hpp>

BOOST_AUTO_TEST_SUITE(test_base)

BOOST_AUTO_TEST_CASE(test_get_image) {
    BOOST_CHECK(true);
}

BOOST_AUTO_TEST_SUITE_END()
// TODO(zack)
