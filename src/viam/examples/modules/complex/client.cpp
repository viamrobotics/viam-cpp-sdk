#include <cstddef>
#include <functional>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <unistd.h>
#include <vector>

#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/support/status.h>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/robot/v1/robot.grpc.pb.h>
#include <viam/api/robot/v1/robot.pb.h>

#include <viam/sdk/components/motor.hpp>
#include <viam/sdk/robot/client.hpp>
#include <viam/sdk/robot/service.hpp>
#include <viam/sdk/rpc/dial.hpp>

#include "gizmo/api.hpp"
#include "summation/api.hpp"

using namespace viam::sdk;

int main() {
    const char* uri = "http://localhost:8080/";  // replace with your URI if connecting securely
    DialOptions dial_options;
    dial_options.set_allow_insecure_downgrade(true);  // set to false if connecting securely

    // Uncomment and fill out your credentials details if connecting securely
    // std::string type = "<your authentication type>";
    // std::string payload = "<your authentication payload>";
    // Credentials credentials(type, payload);
    // dial_options.set_credentials(credentials);

    boost::optional<DialOptions> opts(dial_options);
    std::string address(uri);
    Options options(1, opts);

    // Register custom gizmo and summation clients so robot client can access resources
    // of that type from the server.
    Registry::register_resource_client<GizmoClient>();
    Registry::register_resource_client<SummationClient>();

    // Connect to robot.
    std::shared_ptr<RobotClient> robot = RobotClient::at_address(address, options);
    // Print resources.
    std::cout << "Resources" << std::endl;
    std::vector<Name> resource_names = robot->resource_names();
    for (const Name& resource : resource_names) {
        std::cout << "\t" << resource << "\n";
    }

    // Exercise Gizmo methods.
    auto gc = robot->resource_by_name<Gizmo>("gizmo1");
    if (!gc) {
        std::cerr << "could not get 'gizmo1' resource from robot" << std::endl;
        return EXIT_FAILURE;
    }
    bool do_one_ret = gc->do_one("arg1");
    std::cout << "gizmo1 do_one returned: " << do_one_ret << std::endl;
    bool do_one_client_stream_ret = gc->do_one_client_stream({"arg1", "arg1", "arg1"});
    std::cout << "gizmo1 do_one_client_stream returned: " << do_one_client_stream_ret << std::endl;
    std::string do_two_ret = gc->do_two(false);
    std::cout << "gizmo1 do_two returned: " << do_two_ret << std::endl;
    std::vector<bool> do_one_server_stream_ret = gc->do_one_server_stream("arg1");
    std::cout << "gizmo1 do_one_server_stream returned: " << std::endl;
    for (bool ret : do_one_server_stream_ret) {
        std::cout << '\t' << ret << std::endl;
    }
    std::vector<bool> do_one_bidi_stream_ret = gc->do_one_bidi_stream({"arg1", "arg2", "arg3"});
    std::cout << "gizmo1 do_one_bidi_stream returned: " << std::endl;
    for (bool ret : do_one_bidi_stream_ret) {
        std::cout << '\t' << ret << std::endl;
    }

    // Exercise Summation methods.
    auto sc = robot->resource_by_name<Summation>("mysum1");
    if (!sc) {
        std::cerr << "could not get 'mysum1' resource from robot" << std::endl;
        return EXIT_FAILURE;
    }
    double sum = sc->sum({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
    std::cout << "mysum1 sum of numbers [0, 10) is: " << sum << std::endl;

    // Exercise a Base method.
    auto mc = robot->resource_by_name<Motor>("motor1");
    if (!mc) {
        std::cerr << "could not get 'motor1' resource from robot" << std::endl;
        return EXIT_FAILURE;
    }
    if (mc->is_moving()) {
        std::cout << "motor1 is moving" << std::endl;
    } else {
        std::cout << "motor1 is not moving" << std::endl;
    }

    return EXIT_SUCCESS;
}
