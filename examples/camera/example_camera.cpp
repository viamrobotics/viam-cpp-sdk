#include <components/camera/camera.hpp>
#include <components/camera/client.hpp>
#include <components/camera/server.hpp>

#include <common/v1/common.pb.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/support/status.h>
#include <robot/v1/robot.grpc.pb.h>
#include <robot/v1/robot.pb.h>
#include <unistd.h>

#include <boost/optional.hpp>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include "robot/client.hpp"
#include "robot/service.hpp"
#include "rpc/dial.hpp"

using viam::robot::v1::Status;
using Viam::SDK::Credentials;
using Viam::SDK::Options;

// TODO:
// - The image is currently invalid
// - Running this currently corrupts the malloc chunk metadata. <- Real problem

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
    std::string camera_name = "camera1";
    std::string output_file = "img.png";
    std::string image_mime_type = "image/png";

    std::cout << "Getting camera " << camera_name << std::endl;
    std::shared_ptr<CameraClient> cam = robot->resource_by_name<CameraClient>(camera_name);

    std::cout << "Getting camera properties" << std::endl;
    Camera::properties props = cam->get_properties();
    Camera::intrinsic_parameters intrinsics = props.intrinsic_parameters;
    std::cout << "Image dimensions: " << intrinsics.width_px << " x " << intrinsics.height_px
              << std::endl;

    std::cout << "Getting and saving image to " << output_file << std::endl;
    Camera::raw_image img = cam->get_image(image_mime_type);
    std::ofstream fout;
    fout.open(output_file, std::ios::binary | std::ios::out);
    fout.write((char*)&img.bytes, sizeof(img.bytes.size()));
    fout.close();

    robot->close();
    return 0;
}
