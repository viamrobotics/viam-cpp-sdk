#define BOOST_TEST_MODULE test module test_data_consumer
#include <viam/sdk/module/private/data_consumer_query.hpp>

#include <iostream>

#include <boost/test/included/unit_test.hpp>

namespace viam {
namespace sdktests {
namespace data_consumer {

BOOST_AUTO_TEST_CASE(test_default_query) {
    using time_point_type = std::chrono::system_clock::time_point;

    auto new_years_2025 = time_point_type{} + std::chrono::milliseconds{1735689600000};

    auto result = sdk::impl::default_historical_data_query(
        "part", "res", std::chrono::time_point_cast<std::chrono::milliseconds>(new_years_2025));

    // These are arbitary input/output that I computed and validated with
    // https://jsontotable.org/bson-to-json to make sure that I can convert it to JSON
    // It is hex encoded bytes of the default query constructed above.
    // We rely on unordered_map as an implementation detail so the generated BSON may be equivalent
    // but with field orders reversed, so we include a couple equivalent output strings.

    const std::vector<std::string> expected{
        "5e00000003246d61746368005100000002706172745f69640005000000706172740002636f6d706f6e656e745f"
        "6e616d650004000000726573000374696d655f72656365697665640013000000092467746500007c291f940100"
        "00000000",
        "5e00000003246d6174636800510000000374696d655f72656365697665640013000000092467746500007c"
        "291f940100000002636f6d706f6e656e745f6e616d6500040000007265730002706172745f696400050000"
        "0070617274000000"};

    std::ostringstream os;

    for (auto b : result) {
        os << std::hex << std::setfill('0') << std::setw(2) << unsigned(b);
    }

    const std::string actual = os.str();
    const auto count = std::count(expected.begin(), expected.end(), actual);

    BOOST_CHECK_EQUAL(count, 1);

    if (count == 0) {
        BOOST_CHECK_EQUAL(actual, expected.front());
        BOOST_CHECK_EQUAL(actual, expected.back());
    }
}

}  // namespace data_consumer
}  // namespace sdktests
}  // namespace viam
