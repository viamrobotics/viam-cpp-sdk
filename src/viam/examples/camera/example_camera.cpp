#include <fstream>
#include <string>
#include <unistd.h>
#include <vector>

#include <viam/sdk/components/camera/camera.hpp>
#include <viam/sdk/components/camera/client.hpp>
#include <viam/sdk/robot/client.hpp>
#include <viam/sdk/robot/service.hpp>
#include <viam/sdk/rpc/dial.hpp>

int main() {
    using std::cerr;
    using std::cout;
    using std::endl;
    namespace vs = ::viam::sdk;
    try {
        // If you want to connect to a remote robot, this should be the url of the robot
        // Ex: xxx.xxx.viam.cloud
        std::string robot_address("localhost:8080");
        // If you want to connect to a remote robot, you need the robot location secret (not
        // the part secret) You can find this on app.viam.com
        vs::Credentials credentials("");

        vs::DialOptions dial_options;

        // If you have credentials, use this to pass them to the robot
        // dial_options.credentials = credentials;

        // This is for an example. Care should be taken before exercising this option in production.
        dial_options.allow_insecure_downgrade = credentials.payload.empty();

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

        std::vector<vs::ResourceName>* resource_names = robot->resource_names();

        cout << "Resources of the robot:" << endl;
        for (vs::ResourceName resource : *resource_names) {
            cout << " - " << resource.name() << " (" << resource.subtype() << ")" << endl;
        }

        std::string camera_name("camera1");

        cout << "Getting camera: " << camera_name << endl;
        std::shared_ptr<vs::CameraClient> camera;
        try {
            camera = robot->resource_by_name<vs::CameraClient>(camera_name);
        } catch (const std::exception& e) {
            cerr << "Failed to find " << camera_name << ". Exiting." << endl;
            throw;
        }
        vs::Camera::properties props = camera->get_properties();
        vs::Camera::intrinsic_parameters intrinsics = props.intrinsic_parameters;
        cout << "Image dimensions: " << intrinsics.width_px << " x " << intrinsics.height_px
             << endl;

        std::string output_file("img.png");
        std::string image_mime_type("image/png");

        cout << "Getting image from camera " << endl;
        vs::Camera::raw_image img = camera->get_image(image_mime_type);
        cout << "Got image of mime type: " << img.mime_type << endl;

        cout << "Getting and saving image to " << output_file << endl;
        std::ofstream fout;
        fout.open(output_file, std::ios::binary | std::ios::out);
        if (fout.fail()) {
            throw std::runtime_error("Failed to open output file " + output_file);
        }
        fout.write(reinterpret_cast<char*>(img.bytes.data()), img.bytes.size());
        fout.close();
        if (fout.fail()) {
            throw std::runtime_error("Failed to write and close output file " + output_file);
        }
    } catch (const std::exception& ex) {
        cerr << "Program failed. Exception: " << std::string(ex.what()) << endl;
        return EXIT_FAILURE;
    } catch (...) {
        cerr << "Program failed without exception message." << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
