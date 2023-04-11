#define BOOST_TEST_MODULE test module test_board

#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/test/included/unit_test.hpp>

#include <common/v1/common.pb.h>
#include <component/board/v1/board.grpc.pb.h>
#include <component/board/v1/board.pb.h>

#include <common/proto_type.hpp>
#include <components/board/board.hpp>
#include <components/board/client.hpp>
#include <components/board/server.hpp>
#include <tests/mocks/board_mocks.hpp>
#include <tests/test_utils.hpp>

BOOST_AUTO_TEST_SUITE(test_board)

BOOST_AUTO_TEST_CASE(test_get_image) {
    BOOST_CHECK(true);
}

BOOST_AUTO_TEST_SUITE_END()
// TODO(zack)
