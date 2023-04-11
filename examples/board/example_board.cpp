#include <components/board/board.hpp>
#include <components/board/client.hpp>
#include <components/board/server.hpp>

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

void pretty_print_board_status(const Board::board_status& status) {
    std::cout << "Board Status:\n";
    std::cout << "--------------\n";
    std::cout << "Analog Inputs:\n";
    for (const auto& pair : status.analogs) {
        const std::string& name = pair.first;
        const Board::analog_status& analog = pair.second;
        std::cout << "- " << name << ": " << analog.value << "\n";
    }
    std::cout << "Digital Interrupts:\n";
    for (const auto& pair : status.digital_interrupts) {
        const std::string& name = pair.first;
        const Board::digital_interrupt_status& digital_interrupt = pair.second;
        std::cout << "- " << name << ": " << digital_interrupt.value << "\n";
    }
    std::cout << std::endl;
}
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
    std::string board_name = "board1";

    std::cout << "Getting board " << board_name << std::endl;
    std::shared_ptr<BoardClient> board = robot->resource_by_name<BoardClient>(board_name);
    Board::board_status status = board->get_status();
    pretty_print_board_status(status);
    std::string gpio_pin("di1");
    std::cout << "Setting gpio pin " << gpio_pin << " to LOW " << std::endl;
    board->set_gpio(gpio_pin, false);
    std::cout << "Pin value: " << board->get_gpio(gpio_pin) << std::endl;
    std::cout << "Setting gpio pin " << gpio_pin << " to HIGH " << std::endl;
    board->set_gpio(gpio_pin, true);
    std::cout << "Pin value: " << board->get_gpio(gpio_pin) << std::endl;
    pretty_print_board_status(status);

    robot->close();
    return 0;
}
