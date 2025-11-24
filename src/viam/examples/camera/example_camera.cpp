#include <fstream>
#include <string>
#include <vector>

#include <viam/sdk/common/instance.hpp>
#include <viam/sdk/components/camera.hpp>
#include <viam/sdk/log/logging.hpp>
#include <viam/sdk/robot/client.hpp>
#include <viam/sdk/rpc/dial.hpp>

int main() try {
    namespace vs = ::viam::sdk;
    // Every Viam C++ SDK program must have one and only one Instance object which is created
    // before
    // any other C++ SDK objects and stays alive until all Viam C++ SDK objects are destroyed.
    vs::Instance inst;

    // If you want to connect to a remote robot, this should be the url of the robot
    // Ex: xxx.xxx.viam.cloud
    std::string robot_address("localhost:8080");
    // If you want to connect to a remote robot, you need some authentication secret
    // You can find this on app.viam.com
    vs::Credentials credentials("", "");

    vs::ViamChannel::Options dial_options;

    // If you have credentials, use this to pass them to the robot
    // dial_options.credentials = credentials;

    // This is for an example. Care should be taken before exercising this option in production.
    dial_options.set_allow_insecure_downgrade(
        (credentials.type().empty() && credentials.payload().empty()));

    // Set the refresh interval of the robot (in seconds) (0 = auto refresh) and the dial
    // options
    vs::Options options = vs::Options(1, dial_options);

    std::shared_ptr<vs::RobotClient> robot = vs::RobotClient::at_address(robot_address, options);
    VIAM_SDK_LOG(info) << "Successfully connected to the robot";

    std::vector<vs::Name> resource_names = robot->resource_names();

    VIAM_SDK_LOG(info) << "Resources of the robot:";
    for (const auto& resource : resource_names) {
        VIAM_SDK_LOG(info) << " - " << resource.name() << " (" << resource.api().resource_subtype()
                           << ")";
    }

    std::string camera_name("camera1");

    VIAM_SDK_LOG(info) << "Getting camera: " << camera_name;
    std::shared_ptr<vs::Camera> camera = robot->resource_by_name<vs::Camera>(camera_name);

    vs::Camera::properties props = camera->get_properties();
    vs::Camera::intrinsic_parameters intrinsics = props.intrinsic_parameters;
    VIAM_SDK_LOG(info) << "Image dimensions: " << intrinsics.width_px << " x "
                       << intrinsics.height_px;

    std::string output_file("img.png");
    std::string image_mime_type("image/png");

    VIAM_SDK_LOG(info) << "Getting image from camera ";

    vs::Camera::raw_image img = camera->get_image(image_mime_type);

    VIAM_SDK_LOG(info) << "Got image of mime type: " << img.mime_type;

    VIAM_SDK_LOG(info) << "Getting and saving image to " << output_file;

    std::ofstream fout;

    // Operations on the ofstream will throw on failure.
    fout.exceptions(std::ofstream::failbit);
    fout.open(output_file, std::ios::binary | std::ios::out);

    fout.write(reinterpret_cast<char*>(img.bytes.data()), img.bytes.size());
    fout.close();

    return EXIT_SUCCESS;
} catch (const std::exception& ex) {
    std::cerr << "Program failed. Exception: " << std::string(ex.what()) << "\n";
    return EXIT_FAILURE;
}
