#include <cstddef>
#include <functional>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <unistd.h>
#include <vector>

#include <boost/optional.hpp>

#include <viam/sdk/components/generic.hpp>
#include <viam/sdk/robot/client.hpp>
#include <viam/sdk/rpc/dial.hpp>

using namespace viam::sdk;

int main() {
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
    std::cout << "Resources" << std::endl;
    for (const Name& resource : resource_names) {
        std::cout << "\t" << resource << "\n";
    }

    // ensure we can create clients to the robot
    auto gc = robot->resource_by_name<GenericComponent>("generic1");
    if (gc) {
        std::cout << "got generic component client named " << gc->name() << std::endl;
    }

    return EXIT_SUCCESS;
}
