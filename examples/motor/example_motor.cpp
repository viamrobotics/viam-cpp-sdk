#include <components/motor/client.hpp>
#include <components/motor/motor.hpp>
#include <components/motor/server.hpp>

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

using namespace viam::cppsdk;
using viam::cppsdk::Credentials;
using viam::cppsdk::DialOptions;
using viam::cppsdk::Options;
using viam::robot::v1::Status;

// TODO(RSDK-2751) Cleanup examples for components
int main() {
    DialOptions dial_options;
    dial_options.allow_insecure_downgrade = true;
    boost::optional<DialOptions> opts(dial_options);

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
    std::string motor_name = "motor1";

    std::cout << "Getting motor: " << motor_name << std::endl;
    std::shared_ptr<MotorClient> motor = robot->resource_by_name<MotorClient>(motor_name);
    {
        Motor::position pos = motor->get_position();

        std::cout << "Motor position " << pos << std::endl;
    }

    std::cout << "Moving forward by 1/2 rotation" << std::endl;
    motor->go_for(1000, 0.5);

    {
        Motor::position pos = motor->get_position();

        std::cout << "Motor position " << pos << std::endl;
    }

    robot->close();
    return 0;
}
