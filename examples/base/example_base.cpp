#include <components/base/base.hpp>
#include <components/base/client.hpp>
#include <components/base/server.hpp>

#include <common/v1/common.pb.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/support/status.h>
#include <robot/v1/robot.grpc.pb.h>
#include <robot/v1/robot.pb.h>
#include <unistd.h>

#include <boost/optional.hpp>
#include <chrono>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <thread>
#include <vector>

#include "robot/client.hpp"
#include "robot/service.hpp"
#include "rpc/dial.hpp"

using viam::robot::v1::Status;
using Viam::SDK::Credentials;
using Viam::SDK::Options;

int main() {
    std::cout << "Started" << std::endl;

    Viam::SDK::DialOptions dial_options = Viam::SDK::DialOptions();

    dial_options.allow_insecure_downgrade = true;
    boost::optional<Viam::SDK::DialOptions> opts(dial_options);

    std::string address = "localhost:8080";
    Options options = Options(0, opts);
    std::shared_ptr<RobotClient> robot = RobotClient::at_address(address, options);
    std::cout << "Created robot" << std::endl;
    robot->refresh();
    std::vector<ResourceName>* resource_names = robot->resource_names();

    std::cout << "List resources of the robot" << std::endl;
    for (ResourceName resource : *resource_names) {
        std::cout << "Resource name: " << resource.name() << resource.type() << resource.subtype()
                  << std::endl;
    }
    /* std::string motor_name = "motor1"; */
    /* /1* std::string output_file = "img.png"; *1/ */
    /* /1* std::string image_mime_type = "image/png"; *1/ */

    /* std::cout << "Getting motor " << motor_name << std::endl; */
    /* std::shared_ptr<MotorClient> motor = robot->resource_by_name<MotorClient>(motor_name); */
    /* { */
    /*     Motor::position pos = motor->get_position(); */

    /*     std::cout << "motor position " << pos.position << std::endl; */
    /* } */

    /* motor->go_for(1000, 0.5); */

    /* { */
    /*     Motor::position pos = motor->get_position(); */

    /*     std::cout << "motor position " << pos.position << std::endl; */
    /* } */

    robot->close();
    return 0;
}
