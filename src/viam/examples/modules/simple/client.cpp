#include <iostream>
#include <memory>
#include <string>

#include <viam/sdk/common/instance.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/components/sensor.hpp>
#include <viam/sdk/log/logging.hpp>
#include <viam/sdk/robot/client.hpp>
#include <viam/sdk/rpc/dial.hpp>

using namespace viam::sdk;

int main() {
    // Every Viam C++ SDK program must have one and only one Instance object which is created before
    // any other C++ SDK objects and stays alive until all Viam C++ SDK objects are destroyed.
    Instance inst;

    const char* uri = "http://localhost:8080/";  // replace with your URI if connecting securely
    DialOptions dial_options;
    dial_options.set_allow_insecure_downgrade(true);  // set to false if connecting securely

    // Uncomment and fill out your credentials details if connecting securely
    // std::string type = "<your authentication type>";
    // std::string payload = "<your authentication payload>";
    // Credentials credentials(type, payload);
    // dial_options.set_credentials(credentials);

    boost::optional<DialOptions> opts(dial_options);
    std::string address(uri);
    Options options(1, opts);

    std::shared_ptr<RobotClient> robot = RobotClient::at_address(address, options);

    // Print resources
    VIAM_SDK_LOG(info) << "Resources";
    std::vector<Name> resource_names = robot->resource_names();
    for (const Name& resource : resource_names) {
        VIAM_SDK_LOG(info) << resource;
    }

    // Exercise sensor methods
    auto sensor = robot->resource_by_name<Sensor>("mysensor");
    if (!sensor) {
        VIAM_SDK_LOG(error) << "could not get 'mysensor' resource from robot";
        return EXIT_FAILURE;
    }

    ProtoStruct command{{"hello", "world"}};
    ProtoStruct resp = sensor->do_command(command);

    if (command != resp) {
        VIAM_SDK_LOG(error) << "Got unexpected result from 'mysensor'";
        return EXIT_FAILURE;
    }

    ProtoStruct readings = sensor->get_readings();

    auto itr = readings.find("signal");
    if (itr == readings.end()) {
        VIAM_SDK_LOG(error) << "Expected signal not found in sensor readings";
        return EXIT_FAILURE;
    }

    const double* signal = itr->second.get<double>();
    if (signal) {
        VIAM_SDK_LOG(info) << itr->first << ": " << *signal;
    } else {
        VIAM_SDK_LOG(error) << "Unexpected value type for sensor reading";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
