#include <memory>
#define BOOST_TEST_MODULE generic_test
#include <boost/bind/bind.hpp>
#include <boost/test/unit_test.hpp>

class generic_test {
   public:
    void test_method1() {
        BOOST_TEST(true);
    }
    void test_method2() {
        BOOST_TEST(false);
    }
};

BOOST_AUTO_TEST_SUITE(generic_suite)

boost::unit_test::test_suite* unit_test_suite(int argc, char* argv[]) {
    std::shared_ptr<generic_test> tester = std::make_shared<generic_test>();
    boost::unit_test::framework::master_test_suite().add(
        BOOST_TEST_CASE(boost::bind(&generic_test::test_method1, tester)));
    boost::unit_test::framework::master_test_suite().add(
        BOOST_TEST_CASE(boost::bind(&generic_test::test_method2, tester)));
    return 0;
};

BOOST_AUTO_TEST_SUITE_END()
