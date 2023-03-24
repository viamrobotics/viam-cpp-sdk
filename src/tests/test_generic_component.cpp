#define BOOST_TEST_MODULE test module test_generic_component
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(generic_suite)

BOOST_AUTO_TEST_CASE(test1) {
    int i = 1;
    BOOST_CHECK(i * i == 1);
}

BOOST_AUTO_TEST_CASE(test2) {
    int i = 1;
    BOOST_CHECK(i + i == 2);
}

BOOST_AUTO_TEST_CASE(test3) {
    int i = 1;
    BOOST_CHECK(i + i == 3);
}

BOOST_AUTO_TEST_SUITE_END()
