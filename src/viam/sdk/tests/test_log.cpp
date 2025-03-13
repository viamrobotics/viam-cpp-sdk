#define BOOST_TEST_MODULE test module test_log
#include <boost/test/included/unit_test.hpp>

#include <viam/sdk/log/logger.hpp>

#include <iostream>
#include <sstream>

#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {

BOOST_AUTO_TEST_CASE(test_cout_logging) {
    cout_redirect redirect;

    VIAM_LOG(info) << "log1";

    using namespace std::string_literals;

    const std::string rec = redirect.os.str();
    redirect.release();

    for (const std::string& s :
         {"log1"s, to_string(sdk::log_level::info), sdk::global_resource_name()}) {
        BOOST_CHECK(rec.find(s) != std::string::npos);
    }
}

BOOST_AUTO_TEST_CASE(test_global_filter) {
    cout_redirect redirect;

    VIAM_LOG(info) << "info1";
    VIAM_LOG(error) << "error1";
    VIAM_LOG(trace) << "trace1";  // not logged

    auto& logger = sdk::Logger::get();

    using ll = sdk::log_level;

    logger.set_global_log_level(ll::trace);

    VIAM_LOG(trace) << "trace2";
    VIAM_LOG(info) << "info2";

    logger.set_global_log_level(ll::error);

    VIAM_LOG(info) << "info3";  // not logged
    VIAM_LOG(error) << "error2";

    logger.set_global_log_level(ll::info);

    VIAM_LOG(info) << "info4";
    VIAM_LOG(trace) << "trace3";  // once again not logged

    const std::string rec = redirect.os.str();
    redirect.release();

    for (const char* logged : {"info1", "error1", "trace2", "info2", "error2", "info4"}) {
        BOOST_TEST_INFO("Checking for " << logged << " in log rec\n" << rec);
        BOOST_CHECK(rec.find(logged) != std::string::npos);
    }

    for (const char* not_logged : {"trace1", "info3", "trace3"}) {
        BOOST_TEST_INFO("Checking for " << not_logged << " not in log rec\n" << rec);
        BOOST_CHECK(rec.find(not_logged) == std::string::npos);
    }
}

}  // namespace sdktests
}  // namespace viam
