#include <common/v1/common.pb.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/support/status.h>
#include <robot/v1/robot.grpc.pb.h>
#include <robot/v1/robot.pb.h>
#include <unistd.h>

#include <boost/optional.hpp>
#include <cstddef>
#include <functional>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include <components/generic/client.hpp>
#include <robot/client.hpp>
#include <robot/service.hpp>
#include <rpc/dial.hpp>

using viam::robot::v1::Status;
using Viam::SDK::Credentials;
using Viam::SDK::Options;

int main() {
    const char* uri = "<your robot URI here>";
    Viam::SDK::DialOptions dial_options = Viam::SDK::DialOptions();
    std::string payload = "<your payload here>";
    Credentials credentials(payload);
    dial_options.credentials = credentials;
    boost::optional<Viam::SDK::DialOptions> opts(dial_options);
    std::string address(uri);
    Options options = Options(1, opts);

    // connect to robot, ensure we can refresh it
    std::shared_ptr<RobotClient> robot = RobotClient::at_address(address, options);
    robot->refresh();

    // ensure we can query resources
    std::vector<ResourceName>* resource_names = robot->resource_names();
    for (ResourceName resource : *resource_names) {
        std::cout << "Resource name: " << resource.name() << resource.type() << resource.subtype()
                  << std::endl;
    }

    // ensure we can query statuses
    std::vector<Status> status_plural = robot->get_status();
    std::cout << "Status plural len " << status_plural.size() << std::endl;
    for (Status s : status_plural) {
        std::cout << " Status! " << s.name().subtype() << std::endl;
    }

    // ensure we can send requests for specific resources
    std::vector<ResourceName> just_one = {resource_names->at(0)};
    std::vector<Status> status_singular = robot->get_status(just_one);
    std::cout << "Status singular len " << status_singular.size() << std::endl;
    for (Status s : status_singular) {
        std::cout << " Status! " << s.name().subtype() << std::endl;
    }

    // ensure we can create clients to the robot
    std::shared_ptr<GenericClient> gc = robot->resource_by_name<GenericClient>("generic1");
    std::cout << "got generic client named " << gc->name() << std::endl;

    robot->close();
    return 0;
}

