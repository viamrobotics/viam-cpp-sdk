#include <cstddef>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/support/status.h>

#include <viam/sdk/common/instance.hpp>
#include <viam/sdk/components/motor.hpp>
#include <viam/sdk/log/logging.hpp>
#include <viam/sdk/robot/client.hpp>
#include <viam/sdk/rpc/dial.hpp>

#include "gizmo/api.hpp"
#include "summation/api.hpp"

using namespace viam::sdk;

int main() {
    // Every Viam C++ SDK program must have one and only one Instance object which is created before
    // any other C++ SDK objects and stays alive until all Viam C++ SDK objects are destroyed.
    Instance inst;

    std::string address = "http://localhost:8080/";  // replace with your URI if connecting securely
    DialOptions dial_options;
    dial_options.allow_insecure_downgrade = true;  // set to false if connecting securely

    // Uncomment and fill out your credentials details if connecting securely
    // std::string type = "<your authentication type>";
    // std::string payload = "<your authentication payload>";
    // Credentials credentials(type, payload);
    // dial_options.set_credentials(credentials);

    // Register custom gizmo and summation clients so robot client can access resources
    // of that type from the server.
    Registry::get().register_resource_client<GizmoClient>();
    Registry::get().register_resource_client<SummationClient>();

    // Connect to robot.
    std::shared_ptr<RobotClient> robot =
        RobotClient::at_address(address, std::chrono::seconds{1}, dial_options);

    // Print resources.
    VIAM_SDK_LOG(info) << "Resources:";
    std::vector<Name> resource_names = robot->resource_names();
    for (const Name& resource : resource_names) {
        VIAM_SDK_LOG(info) << resource;
    }

    // Exercise Gizmo methods.
    auto gc = robot->resource_by_name<Gizmo>("gizmo1");
    if (!gc) {
        VIAM_SDK_LOG(error) << "could not get 'gizmo1' resource from robot" << std::endl;
        return EXIT_FAILURE;
    }

    bool do_one_ret = gc->do_one("arg1");
    VIAM_SDK_LOG(info) << "gizmo1 do_one returned: " << do_one_ret;

    bool do_one_client_stream_ret = gc->do_one_client_stream({"arg1", "arg1", "arg1"});
    VIAM_SDK_LOG(info) << "gizmo1 do_one_client_stream returned: " << do_one_client_stream_ret;

    std::string do_two_ret = gc->do_two(false);
    VIAM_SDK_LOG(info) << "gizmo1 do_two returned: " << do_two_ret;

    std::vector<bool> do_one_server_stream_ret = gc->do_one_server_stream("arg1");
    VIAM_SDK_LOG(info) << "gizmo1 do_one_server_stream returned: ";
    for (bool ret : do_one_server_stream_ret) {
        VIAM_SDK_LOG(info) << ret;
    }

    std::vector<bool> do_one_bidi_stream_ret = gc->do_one_bidi_stream({"arg1", "arg2", "arg3"});
    VIAM_SDK_LOG(info) << "gizmo1 do_one_bidi_stream returned: ";
    for (bool ret : do_one_bidi_stream_ret) {
        VIAM_SDK_LOG(info) << ret;
    }

    // Exercise Summation methods.
    auto sc = robot->resource_by_name<Summation>("mysum1");
    if (!sc) {
        VIAM_SDK_LOG(error) << "could not get 'mysum1' resource from robot";
        return EXIT_FAILURE;
    }

    double sum = sc->sum({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
    VIAM_SDK_LOG(info) << "mysum1 sum of numbers [0, 10) is: " << sum;

    // Exercise a Base method.
    auto mc = robot->resource_by_name<Motor>("motor1");
    if (!mc) {
        VIAM_SDK_LOG(error) << "could not get 'motor1' resource from robot";
        return EXIT_FAILURE;
    }

    if (mc->is_moving()) {
        VIAM_SDK_LOG(info) << "motor1 is moving";
    } else {
        VIAM_SDK_LOG(info) << "motor1 is not moving";
    }

    return EXIT_SUCCESS;
}
