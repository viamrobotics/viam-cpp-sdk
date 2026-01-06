#define BOOST_TEST_MODULE test module test_log
#include <boost/test/included/unit_test.hpp>

#include <viam/sdk/log/logging.hpp>

#include <iostream>
#include <sstream>

#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/tests/mocks/mock_sensor.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {

BOOST_AUTO_TEST_CASE(test_cout_logging) {
    cout_redirect redirect;

    VIAM_SDK_LOG(info) << "log1";

    using namespace std::string_literals;

    const std::string rec = redirect.os.str();
    redirect.release();

    for (const std::string& s :
         {"log1"s, to_string(sdk::log_level::info), std::string{sdk::global_resource_name()}}) {
        BOOST_CHECK(rec.find(s) != std::string::npos);
    }
}

BOOST_AUTO_TEST_CASE(test_global_name) {
    cout_redirect redirect;

    sdk::LogManager::get().set_global_resource_name("My Channel");

    VIAM_SDK_LOG(info) << "after";

    const std::string rec = redirect.os.str();
    redirect.release();

    for (const char* s : {"My Channel", "after"}) {
        BOOST_CHECK(rec.find(s) != std::string::npos);
    }

    BOOST_CHECK(rec.find(sdk::global_resource_name()) == std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_module_name) {
    cout_redirect redirect;

    sdk::LogManager::get().set_module_name("my module");

    VIAM_MODULE_LOG(info) << "after";
    const std::string rec = redirect.os.str();
    redirect.release();

    for (const char* s : {"my module", "after"}) {
        BOOST_CHECK(rec.find(s) != std::string::npos);
    }

    BOOST_CHECK(rec.find(sdk::default_module_name()) == std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_global_filter) {
    cout_redirect redirect;

    VIAM_SDK_LOG(info) << "info1";
    VIAM_SDK_LOG(error) << "error1";
    VIAM_SDK_LOG(trace) << "trace1";  // not logged

    auto& logger = sdk::LogManager::get();

    using ll = sdk::log_level;

    logger.set_global_log_level(ll::trace);

    VIAM_SDK_LOG(trace) << "trace2";
    VIAM_SDK_LOG(info) << "info2";

    logger.set_global_log_level(ll::error);

    VIAM_SDK_LOG(info) << "info3";  // not logged
    VIAM_SDK_LOG(error) << "error2";

    logger.set_global_log_level(ll::info);

    VIAM_SDK_LOG(info) << "info4";
    VIAM_SDK_LOG(trace) << "trace3";  // once again not logged

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

BOOST_AUTO_TEST_CASE(test_module_filter) {
    cout_redirect redirect;

    VIAM_MODULE_LOG(info) << "info1";
    VIAM_MODULE_LOG(error) << "error1";
    VIAM_MODULE_LOG(trace) << "trace1";  // not logged

    auto& logger = sdk::LogManager::get();

    using ll = sdk::log_level;

    logger.set_module_log_level(ll::trace);

    VIAM_MODULE_LOG(trace) << "trace2";
    VIAM_MODULE_LOG(info) << "info2";

    const std::string rec = redirect.os.str();
    redirect.release();

    for (const char* logged : {"info1", "error1", "trace2", "info2"}) {
        BOOST_TEST_INFO("Checking for " << logged << " in log rec\n" << rec);
        BOOST_CHECK(rec.find(logged) != std::string::npos);
    }

    BOOST_CHECK(rec.find("trace1") == std::string::npos);
}

struct LogSensor : sensor::MockSensor {
    using sensor::MockSensor::MockSensor;

    sdk::ProtoStruct get_readings(const sdk::ProtoStruct& extra) override {
        VIAM_RESOURCE_LOG(info) << "sensor info";
        VIAM_RESOURCE_LOG(error) << "sensor error";
        return sensor::MockSensor::get_readings(extra);
    }
};

BOOST_AUTO_TEST_CASE(test_resource_filter) {
    cout_redirect redirect;

    auto defaultSensor = std::make_shared<LogSensor>("DefaultSensor");
    auto errorSensor = std::make_shared<LogSensor>("ErrorSensor");
    errorSensor->set_log_level(sdk::log_level::error);

    for (auto sensor : {defaultSensor, errorSensor}) {
        client_to_mock_pipeline<Sensor>(sensor,
                                        [&](Sensor& client) { (void)client.get_readings({}); });
    }

    std::vector<std::string> defaultLogs;
    std::vector<std::string> errLogs;

    const std::string rec = redirect.os.str();
    BOOST_TEST_INFO("Log records\n" << rec);

    std::stringstream ss(rec);
    redirect.release();

    {
        std::string local;

        while (std::getline(ss, local, '\n')) {
            if (local.find("DefaultSensor") != std::string::npos) {
                defaultLogs.push_back(std::move(local));
            } else if (local.find("ErrorSensor") != std::string::npos) {
                errLogs.push_back(std::move(local));
            }
        }
    }

    BOOST_ASSERT(defaultLogs.size() == 2);
    {
        BOOST_TEST_MESSAGE("default logs\n" << defaultLogs.front() << "\n" << defaultLogs.back());
        BOOST_CHECK(defaultLogs.front().find("sensor info") != std::string::npos);
        BOOST_CHECK(defaultLogs.back().find("sensor error") != std::string::npos);
    }

    BOOST_ASSERT(errLogs.size() == 1);
    BOOST_CHECK(errLogs.back().find("sensor error") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(filename_trim) {
    using namespace log_detail;

    BOOST_CHECK(trim_filename("") == "");
    BOOST_CHECK(trim_filename("no_delim") == "no_delim");
    BOOST_CHECK(trim_filename("one/slash.cpp") == "one/slash.cpp");
    BOOST_CHECK(trim_filename("a/full/path.cpp") == "full/path.cpp");
    BOOST_CHECK(trim_filename("a/b/c/d.cpp") == "c/d.cpp");
}

}  // namespace sdktests
}  // namespace viam
