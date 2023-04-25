#include <fstream>
#include <string>
#include <unistd.h>
#include <vector>

#include <viam/sdk/components/camera/camera.hpp>
#include <viam/sdk/components/camera/client.hpp>
#include <viam/sdk/robot/client.hpp>
#include <viam/sdk/robot/service.hpp>
#include <viam/sdk/rpc/dial.hpp>

using namespace viam::sdk;
using namespace std;

int main() {
    // If you want to connect to a remote robot, this should be the url of the robot
    // Ex: xxx.xxx.viam.cloud
    string robot_address("localhost:8080");
    // If you want to connect to a remote robot, you need the robot location secret (not
    // the part secret) You can find this on app.viam.com
    Credentials credentials("");

    DialOptions dial_options;

    // If you have credentials, use this to pass them to the robot
    // dial_options.credentials = credentials;

    // This is for an example. You should **not** have this option enabled in real systems.
    dial_options.allow_insecure_downgrade = credentials.payload.empty();

    // Set the refresh interval of the robot (in seconds) (0 = auto refresh) and the dial
    // options
    Options options = Options(1, dial_options);

    shared_ptr<RobotClient> robot;
    try {
        robot = RobotClient::at_address(robot_address, options);
        cout << "Successfully connected to the robot" << endl;
    } catch (const std::exception& e) {
        cout << "Failed to connect to the robot. Exiting." << endl;
        cout << "Exception: " << string(e.what()) << endl;
        return EXIT_FAILURE;
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
        return EXIT_FAILURE;
    }
    Camera::properties props = camera->get_properties();
    Camera::intrinsic_parameters intrinsics = props.intrinsic_parameters;
    cout << "Image dimensions: " << intrinsics.width_px << " x " << intrinsics.height_px << endl;

    string output_file = "img.png";
    string image_mime_type = "image/png";

    cout << "Getting image from camera " << endl;
    Camera::raw_image img = camera->get_image(image_mime_type);
    cout << "Got image of mime type: " << img.mime_type << endl;

    cout << "Getting and saving image to " << output_file << endl;
    ofstream fout;
    fout.open(output_file, std::ios::binary | std::ios::out);
    fout.write((char*)&img.bytes[0], img.bytes.size());
    fout.close();

    return EXIT_SUCCESS;
}
