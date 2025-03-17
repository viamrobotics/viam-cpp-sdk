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

    VIAM_LOG(info) << "log1";

    using namespace std::string_literals;

    const std::string rec = redirect.os.str();
    redirect.release();

    for (const std::string& s :
         {"log1"s, to_string(sdk::log_level::info), std::string{sdk::global_resource_name()}}) {
        BOOST_CHECK(rec.find(s) != std::string::npos);
    }
}

BOOST_AUTO_TEST_CASE(test_global_filter) {
    cout_redirect redirect;

    VIAM_LOG(info) << "info1";
    VIAM_LOG(error) << "error1";
    VIAM_LOG(trace) << "trace1";  // not logged

    auto& logger = sdk::LogManager::get();

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

}  // namespace sdktests
}  // namespace viam
