#include <memory>
#include <string>

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/robot/client.hpp>
#include <viam/sdk/rpc/dial.hpp>
#include <viam/sdk/services/generic.hpp>

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

    std::shared_ptr<RobotClient> robot = RobotClient::at_address(address, options);

    // Print resources
    std::cout << "Resources\n";
    std::vector<Name> resource_names = robot->resource_names();
    for (const Name& resource : resource_names) {
        std::cout << "\t" << resource << "\n";
    }

    // Exercise printer methods
    auto printer = robot->resource_by_name<GenericService>("printer1");
    if (!printer) {
        std::cerr << "could not get 'printer1' resource from robot\n";
        return EXIT_FAILURE;
    }

    ProtoStruct command{{"hello", "world"}};
    ProtoStruct resp = printer->do_command(command);

    if (command != resp) {
        std::cerr << "Got unexpected result from 'printer1'\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
