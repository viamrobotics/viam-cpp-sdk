#include <chrono>
#include <fstream>
#include <string>
#include <unistd.h>
#include <vector>

#include <viam/sdk/components/motor.hpp>
#include <viam/sdk/robot/client.hpp>
#include <viam/sdk/robot/service.hpp>
#include <viam/sdk/rpc/dial.hpp>

void print_motor_position(std::shared_ptr<viam::sdk::Motor> motor) {
    // Whether the motor supports returning its position
    if (motor->get_properties().position_reporting) {
        // Position is measured in rotations (position is a typedef double)
        std::cout << "Motor pos: " << motor->get_position() << std::endl;
    } else {
        std::cout << "Motor position unavailable" << std::endl;
    }
}

int main() {
    using std::cerr;
    using std::cout;
    using std::endl;
    namespace vs = ::viam::sdk;

    try {
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
        dial_options.set_allow_insecure_downgrade(
            (credentials.type().empty() && credentials.payload().empty()));

        // Set the refresh interval of the robot (in seconds) (0 = auto refresh) and the dial
        // options
        vs::Options options = vs::Options(1, dial_options);

        std::shared_ptr<vs::RobotClient> robot;
        try {
            robot = vs::RobotClient::at_address(robot_address, options);
            cout << "Successfully connected to the robot" << endl;
        } catch (const std::exception& e) {
            cerr << "Failed to connect to the robot. Exiting." << endl;
            throw;
        }

        std::vector<vs::Name> resource_names = robot->resource_names();

        cout << "Resources of the robot:" << endl;
        for (const vs::Name& resource : resource_names) {
            cout << resource << endl;
        }

        std::string motor_name("motor1");

        cout << "Getting motor: " << motor_name << endl;
        std::shared_ptr<vs::Motor> motor;
        try {
            motor = robot->resource_by_name<vs::Motor>(motor_name);
        } catch (const std::exception& e) {
            cerr << "Failed to find " << motor_name << ". Exiting." << endl;
            throw;
        }

        // Motors are always turned off if there is no attached client so this is expected to be 0
        cout << "Motor power: " << motor->get_power_status().power_pct << endl;
        cout << "Setting motor power to 50%" << endl;
        motor->set_power(0.5);
        cout << "Motor power: " << motor->get_power_status().power_pct << endl;

        print_motor_position(motor);

        cout << "Moving motor by 0.5 rotations at 10rpm" << endl;
        motor->go_for(10, 0.5);
        print_motor_position(motor);
        cout << "Moving motor back to starting location" << endl;
        if (motor->get_properties().position_reporting) {
            // More accurate
            motor->go_to(10, 0);
        } else {
            // If position reporting is unavailable, we cannot call go_to
            // but we can defer to reverting past actions
            motor->go_for(10, -0.5);
        }
        print_motor_position(motor);
        cout << "Moving motor forward for 2 seconds at 30rpm" << endl;
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
        cout << "Stopping motor" << endl;
        motor->stop({});

    } catch (const std::exception& ex) {
        cerr << "Program failed. Exception: " << std::string(ex.what()) << endl;
        return EXIT_FAILURE;
    } catch (...) {
        cerr << "Program failed without exception message." << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
