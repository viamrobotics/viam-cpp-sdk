#include <cstddef>
#include <functional>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include <boost/optional.hpp>
#include <boost/program_options.hpp>

#include <viam/sdk/common/instance.hpp>
#include <viam/sdk/robot/client.hpp>
#include <viam/sdk/rpc/dial.hpp>

using namespace viam::sdk;

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
    // Every Viam C++ SDK program must have one and only one Instance object which is created before
    // any other C++ SDK objects and stays alive until all Viam C++ SDK objects are destroyed.
    Instance inst;

    po::options_description desc("Allowed options");
    desc.add_options()("help", "List options and exit")(
        "uri", po::value<std::string>(), "URI of robot")(
        "entity", po::value<std::string>(), "api key ID")(
        "api-key", po::value<std::string>(), "api key secret");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 0;
    }

    DialOptions opts;

    if (vm.count("entity") && vm.count("api-key")) {
        opts.auth_entity = vm["entity"].as<std::string>();
        Credentials credentials("api-key", vm["api-key"].as<std::string>());
        opts.credentials = credentials;
    }

    // connect to robot, ensure we can refresh it
    std::shared_ptr<RobotClient> robot =
        RobotClient::at_address(vm["uri"].as<std::string>(), std::chrono::seconds{1}, opts);

    // ensure we can query resources
    std::vector<Name> resource_names = robot->resource_names();

    VIAM_SDK_LOG(info) << "Resources:";
    for (const Name& resource : resource_names) {
        VIAM_SDK_LOG(info) << resource;
    }

    return EXIT_SUCCESS;
}
