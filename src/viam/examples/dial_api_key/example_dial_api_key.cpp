#include <cstddef>
#include <functional>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <unistd.h>
#include <vector>

#include <boost/optional.hpp>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/support/status.h>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/robot/v1/robot.grpc.pb.h>
#include <viam/api/robot/v1/robot.pb.h>

#include <viam/sdk/components/generic/client.hpp>
#include <viam/sdk/robot/client.hpp>
#include <viam/sdk/robot/service.hpp>
#include <viam/sdk/rpc/dial.hpp>

using viam::robot::v1::Status;
using namespace viam::sdk;

int main() {
    const char* uri = "<your robot URI here>";
    DialOptions dial_options;
    std::string type = "api-key";
    std::string entity = "<your api key id>";
    std::string payload = "<your api key value>";
    dial_options.set_entity(entity);
    Credentials credentials(type, payload);
    dial_options.set_credentials(credentials);
    boost::optional<DialOptions> opts(dial_options);
    std::string address(uri);
    Options options(1, opts);

    // connect to robot, ensure we can refresh it
    std::shared_ptr<RobotClient> robot = RobotClient::at_address(address, options);

    // ensure we can query resources
    std::vector<Name>* resource_names = robot->resource_names();
    std::cout << "Resources" << std::endl;
    for (const Name& resource : *resource_names) {
        std::cout << "\tname: " << resource.name() << " (type:" << resource.api().resource_type()
                  << " subtype:" << resource.api().resource_subtype() << ")" << std::endl;
    }

    // ensure we can query statuses
    std::vector<RobotClient::status> status_plural = robot->get_status();
    std::cout << "Status plural len " << status_plural.size() << std::endl;
    for (const RobotClient::status& s : status_plural) {
        std::cout << " Status! " << s.name->api().resource_subtype() << std::endl;
    }

    // ensure we can send requests for specific resources
    std::vector<Name> just_one = {resource_names->at(0)};
    std::vector<RobotClient::status> status_singular = robot->get_status(just_one);
    std::cout << "Status singular len " << status_singular.size() << std::endl;
    for (const RobotClient::status& s : status_singular) {
        std::cout << " Status! " << s.name->api().resource_subtype() << std::endl;
    }

    return EXIT_SUCCESS;
}
