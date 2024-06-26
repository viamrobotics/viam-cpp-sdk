#include <memory>
#include <string>

#include <viam/sdk/common/proto_type.hpp>
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

    auto proto_ptr = std::make_shared<ProtoType>(std::string("world"));
    AttributeMap command =
        std::make_shared<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>();
    command->insert({{std::string("hello"), proto_ptr}});

    auto resp = printer->do_command(command);

    if (!resp) {
        std::cerr << "Failed to get a response from 'printer1'\n";
        return EXIT_FAILURE;
    }

    std::shared_ptr<ProtoType> expected = command->at(std::string("hello"));
    std::shared_ptr<ProtoType> result = resp->at(std::string("hello"));

    if (!(*expected == *result)) {
        std::cerr << "Got unexpected result from 'printer1'\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
