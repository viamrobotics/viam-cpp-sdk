#include <cstddef>
#include <functional>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <unistd.h>
#include <vector>

#include <boost/optional.hpp>
#include <boost/program_options.hpp>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/support/status.h>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/robot/v1/robot.grpc.pb.h>
#include <viam/api/robot/v1/robot.pb.h>

#include <viam/sdk/robot/client.hpp>
#include <viam/sdk/robot/service.hpp>
#include <viam/sdk/rpc/dial.hpp>

using viam::robot::v1::Status;
using namespace viam::sdk;

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
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
    boost::optional<DialOptions> opts;
    if (vm.count("entity") && vm.count("api-key")) {
        DialOptions dial_options;
        dial_options.set_entity(vm["entity"].as<std::string>());
        Credentials credentials("api-key", vm["api-key"].as<std::string>());
        dial_options.set_credentials(credentials);
        opts = dial_options;
    }
    Options options(1, opts);

    // connect to robot, ensure we can refresh it
    std::shared_ptr<RobotClient> robot =
        RobotClient::at_address(vm["uri"].as<std::string>(), options);

    // ensure we can query resources
    std::vector<Name> resource_names = robot->resource_names();
    std::cout << "Resources" << std::endl;
    for (const Name& resource : resource_names) {
        std::cout << "\t" << resource << "\n";
    }

    // ensure we can query statuses
    std::vector<RobotClient::status> status_plural = robot->get_status();
    std::cout << "Status plural len " << status_plural.size() << std::endl;
    for (const RobotClient::status& s : status_plural) {
        std::cout << " Status! " << s.name->api().resource_subtype() << std::endl;
    }

    // ensure we can send requests for specific resources
    std::vector<Name> just_one = {resource_names[0]};
    std::vector<RobotClient::status> status_singular = robot->get_status(just_one);
    std::cout << "Status singular len " << status_singular.size() << std::endl;
    for (const RobotClient::status& s : status_singular) {
        std::cout << " Status! " << s.name->api().resource_subtype() << std::endl;
    }

    return EXIT_SUCCESS;
}
