#include <robot/client.hpp>
int main() {
    std::shared_ptr<RobotClient> robot =
        RobotClient::at_address("localhost:8080", {0, boost::none});
}
