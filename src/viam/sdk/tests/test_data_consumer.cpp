#define BOOST_TEST_MODULE test module test_data_consumer
#include <viam/sdk/module/data_consumer.hpp>

#include <iostream>

#include <boost/test/included/unit_test.hpp>

namespace viam {
namespace sdktests {
namespace data_consumer {

BOOST_AUTO_TEST_CASE(test_default_query) {
    using time_point_type = std::chrono::system_clock::time_point;

    auto new_years_2025 = time_point_type{} + std::chrono::milliseconds{1735689600000};

    auto result = sdk::DataConsumer::default_query(
        "part", "res", std::chrono::time_point_cast<std::chrono::milliseconds>(new_years_2025));

    // This is an arbitary input/output that I computed and validated with
    // https://jsontotable.org/bson-to-json to make sure that I can convert it to JSON
    const std::string expected =
        "5e00000003246d61746368005100000002706172745f69640005000000706172740002636f6d706f6e656e745f"
        "6e616d650004000000726573000374696d655f72656365697665640013000000092467746500007c291f940100"
        "00000000";

    std::ostringstream os;

    for (auto b : result) {
        os << std::hex << std::setfill('0') << std::setw(2) << unsigned(b);
    }

    const std::string actual = os.str();

    BOOST_CHECK_EQUAL(expected, actual);
}

}  // namespace data_consumer
}  // namespace sdktests
}  // namespace viam
