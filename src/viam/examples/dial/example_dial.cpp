#include <cstddef>
#include <functional>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include <boost/optional.hpp>

#include <viam/sdk/common/instance.hpp>
#include <viam/sdk/components/generic.hpp>
#include <viam/sdk/log/logging.hpp>
#include <viam/sdk/robot/client.hpp>
#include <viam/sdk/rpc/dial.hpp>

using namespace viam::sdk;

int main() {
    // Every Viam C++ SDK program must have one and only one Instance object which is created before
    // any other C++ SDK objects and stays alive until all Viam C++ SDK objects are destroyed.
    Instance inst;

    const char* uri = "<your robot URI here>";
    DialOptions dial_options;
    std::string type = "<your authentication type>";
    std::string payload = "<your authentication payload>";
    Credentials credentials(type, payload);
    dial_options.set_credentials(credentials);
    boost::optional<DialOptions> opts(dial_options);
    std::string address(uri);
    Options options(1, opts);

    // connect to robot, ensure we can refresh it
    std::shared_ptr<RobotClient> robot = RobotClient::at_address(address, options);

    // ensure we can query resources
    std::vector<Name> resource_names = robot->resource_names();
    VIAM_SDK_LOG(info) << "Resources:";
    for (const Name& resource : resource_names) {
        VIAM_SDK_LOG(info) << resource;
    }

    // ensure we can create clients to the robot
    auto gc = robot->resource_by_name<GenericComponent>("generic1");
    if (gc) {
        VIAM_SDK_LOG(info) << "got generic component client named " << gc->name();
    }

    return EXIT_SUCCESS;
}
