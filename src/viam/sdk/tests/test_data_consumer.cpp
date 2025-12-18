#define BOOST_TEST_MODULE test module test_data_consumer
#include <viam/sdk/module/data_consumer.hpp>

#include <iostream>

#include <boost/test/included/unit_test.hpp>

namespace viam {
namespace sdktests {
namespace data_consumer {

BOOST_AUTO_TEST_CASE(test_default_query) {
    auto now =
        std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    auto result = sdk::DataConsumer::default_query("part", "res", now);

    std::cout << "result size " << result.size() << "\n";

    for (auto b : result) {
        std::cout << std::hex << std::setfill('0') << std::setw(2) << unsigned(b);
    }

    std::cout << "\n";
}

}  // namespace data_consumer
}  // namespace sdktests
}  // namespace viam
