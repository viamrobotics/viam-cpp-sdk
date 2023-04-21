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

#include <boost/optional.hpp>
#include <boost/program_options.hpp>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/support/status.h>
#include <unistd.h>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/robot/v1/robot.grpc.pb.h>
#include <viam/api/robot/v1/robot.pb.h>

#include <viam/sdk/components/camera/camera.hpp>
#include <viam/sdk/components/camera/client.hpp>
#include <viam/sdk/components/camera/server.hpp>
#include <viam/sdk/robot/client.hpp>
#include <viam/sdk/robot/service.hpp>
#include <viam/sdk/rpc/dial.hpp>

using namespace viam::sdk;
using namespace std;

using viam::robot::v1::Status;

int main() {
    // If you want to connect to a remote robot, this should be the url of the robot
    // Ex: xxx.xxx.viam.cloud
    string robot_address("localhost:8080");
    // If you want to connect to a remote robot, you will need the robot secret
    // You can find this on app.viam.com
    Credentials credentials("");

    DialOptions dial_options;

    // This is for an example. You should NOT have this option enabled in real systems.
    dial_options.allow_insecure_downgrade = credentials.payload.length() == 0;

    // Set the refresh interval of the robot (in seconds) (0 = auto refresh)
    Options options = Options(1, dial_options);

    shared_ptr<RobotClient> robot;
    try {
        robot = RobotClient::at_address(robot_address, options);
        robot->refresh();
        cout << "Successfully connected to the robot" << endl;
    } catch (const std::exception& e) {
        cout << "Failed to connect to the robot. Exiting." << endl;
        cout << "Exception: " << string(e.what()) << endl;
        return 1;
    }

    vector<ResourceName>* resource_names = robot->resource_names();

    cout << "Resources of the robot:" << endl;
    for (ResourceName resource : *resource_names) {
        cout << " - " << resource.name() << " (" << resource.subtype() << ")" << endl;
    }

    string camera_name("camera1");

    cout << "Getting camera: " << camera_name << endl;
    shared_ptr<CameraClient> camera;
    try {
        camera = robot->resource_by_name<CameraClient>(camera_name);
    } catch (const std::exception& e) {
        cout << "Failed to find " << camera_name << ". Exiting." << endl;
        cout << "Exception: " << e.what() << endl;
        robot->close();
        return 1;
    }
    Camera::properties props = camera->get_properties();
    Camera::intrinsic_parameters intrinsics = props.intrinsic_parameters;
    cout << "Image dimensions: " << intrinsics.width_px << " x " << intrinsics.height_px << endl;

    string output_file = "img.png";
    string image_mime_type = "image/png";

    std::cout << "Getting and saving image to " << output_file << std::endl;
    Camera::raw_image img = camera->get_image(image_mime_type);
    std::ofstream fout;
    fout.open(output_file, std::ios::binary | std::ios::out);
    fout.write((char*)&img.bytes, sizeof(img.bytes.size()));
    fout.close();

    robot->close();
    return 0;
}
