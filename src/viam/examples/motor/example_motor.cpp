#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <viam/sdk/common/instance.hpp>
#include <viam/sdk/components/motor.hpp>
#include <viam/sdk/log/logging.hpp>
#include <viam/sdk/robot/client.hpp>
#include <viam/sdk/rpc/dial.hpp>

void print_motor_position(std::shared_ptr<viam::sdk::Motor> motor) {
    // Whether the motor supports returning its position
    if (motor->get_properties().position_reporting) {
        // Position is measured in rotations (position is a typedef double)
        VIAM_SDK_LOG(info) << "Motor pos: " << motor->get_position();
    } else {
        VIAM_SDK_LOG(info) << "Motor position unavailable";
    }
}

int main() try {
    namespace vs = ::viam::sdk;

    // Every Viam C++ SDK program must have one and only one Instance object which is created
    // before any other C++ SDK objects and stays alive until all Viam C++ SDK objects are
    // destroyed.
    vs::Instance inst;

    // If you want to connect to a remote robot, this should be the url of the robot
    // Ex: xxx.xxx.viam.cloud
    std::string robot_address("localhost:8080");
    // If you want to connect to a remote robot, you need some authentication secret
    // You can find this on app.viam.com
    vs::Credentials credentials("", "");

    vs::DialOptions dial_options;

    // If you have credentials, use this to pass them to the robot
    // dial_options.credentials = credentials;

    // This is for an example. Care should be taken before exercising this option in production.
    dial_options.allow_insecure_downgrade =
        (credentials.type().empty() && credentials.payload().empty());

    // Pass the scheduled refresh interval of the robot (0 seconds = only on config update) and the
    // dial options
    std::shared_ptr<vs::RobotClient> robot =
        vs::RobotClient::at_address(robot_address, std::chrono::seconds{1}, dial_options);

    VIAM_SDK_LOG(info) << "Successfully connected to the robot";

    std::vector<vs::Name> resource_names = robot->resource_names();

    VIAM_SDK_LOG(info) << "Resources of the robot:";
    for (const vs::Name& resource : resource_names) {
        VIAM_SDK_LOG(info) << resource;
    }

    std::string motor_name("motor1");

    VIAM_SDK_LOG(info) << "Getting motor: " << motor_name;
    std::shared_ptr<vs::Motor> motor = robot->resource_by_name<vs::Motor>(motor_name);

    // Motors are always turned off if there is no attached client so this is expected to be 0
    VIAM_SDK_LOG(info) << "Motor power: " << motor->get_power_status().power_pct;
    VIAM_SDK_LOG(info) << "Setting motor power to 50%";
    motor->set_power(0.5);
    VIAM_SDK_LOG(info) << "Motor power: " << motor->get_power_status().power_pct;

    print_motor_position(motor);

    VIAM_SDK_LOG(info) << "Moving motor by 0.5 rotations at 10rpm";
    motor->go_for(10, 0.5);

    print_motor_position(motor);

    VIAM_SDK_LOG(info) << "Moving motor back to starting location";
    if (motor->get_properties().position_reporting) {
        // More accurate
        motor->go_to(10, 0);
    } else {
        // If position reporting is unavailable, we cannot call go_to
        // but we can defer to reverting past actions
        motor->go_for(10, -0.5);
    }

    print_motor_position(motor);

    VIAM_SDK_LOG(info) << "Moving motor forward for 2 seconds at 30rpm";
    // With `revolutions`==0, the call will be non-blocking
    // and the motor will rotate indefintely.
    motor->go_for(30, 0);

    print_motor_position(motor);

    // Wait for 2 seconds, print position every 0.5 seconds
    for (int i = 0; i < 4; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        print_motor_position(motor);
    }

    // Explicitly stop the motor
    VIAM_SDK_LOG(info) << "Stopping motor";
    motor->stop({});

    return EXIT_SUCCESS;
} catch (const std::exception& ex) {
    std::cerr << "Program failed. Exception: " << std::string(ex.what()) << "\n";
    return EXIT_FAILURE;
}
