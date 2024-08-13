#include <iostream>
#include <memory>
#include <sstream>

#include <boost/log/trivial.hpp>
#include <grpcpp/grpcpp.h>
#include <grpcpp/server_context.h>

#include <viam/api/common/v1/common.grpc.pb.h>
#include <viam/api/robot/v1/robot.pb.h>
#include <viam/api/service/generic/v1/generic.grpc.pb.h>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/module/module.hpp>
#include <viam/sdk/module/service.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/reconfigurable.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/rpc/dial.hpp>
#include <viam/sdk/rpc/server.hpp>
#include <viam/sdk/services/generic.hpp>
#include <viam/sdk/services/service.hpp>

using namespace viam::sdk;

// Printer is a modular resource that can print a to_print value to STDOUT when
// a DoCommand request is received or when reconfiguring. The to_print value
// must be provided as an attribute in the config.
class Printer : public GenericService, public Reconfigurable {
   public:
    void reconfigure(const Dependencies& deps, const ResourceConfig& cfg) {
        std::cout << "Printer " << Resource::name() << " is reconfiguring" << std::endl;
        for (auto& dep : deps) {
            std::cout << "dependency: " << dep.first.to_string() << std::endl;
        }
        to_print_ = find_to_print(cfg);
        std::cout << "Printer " << Resource::name() << " will now print " << to_print_ << std::endl;
    }

    Printer(Dependencies deps, ResourceConfig cfg) : GenericService(cfg.name()) {
        std::cout << "Creating Printer " + Resource::name() << std::endl;
        to_print_ = find_to_print(cfg);
        std::cout << "Printer " << Resource::name() << " will print " << to_print_ << std::endl;
    }

    AttributeMap do_command(const AttributeMap& command) {
        std::cout << "Received DoCommand request for Printer " << Resource::name() << std::endl;
        std::cout << "Printer " << Resource::name() << " has printed " << to_print_ << std::endl;
        return command;
    }

    static std::string find_to_print(ResourceConfig cfg) {
        auto& printer_name = cfg.name();
        auto to_print = cfg.attributes()->find("to_print");
        if (to_print == cfg.attributes()->end()) {
            std::ostringstream buffer;
            buffer << printer_name << ": Required parameter `to_print` not found in configuration";
            throw std::invalid_argument(buffer.str());
        }
        const auto* const to_print_string = to_print->second->get<std::string>();
        if (!to_print_string || to_print_string->empty()) {
            std::ostringstream buffer;
            buffer << printer_name
                   << ": Required non-empty string parameter `to_print` is either not a string "
                      "or is an empty string";
            throw std::invalid_argument(buffer.str());
        }
        return *to_print_string;
    }

   private:
    std::string to_print_;
};

int main(int argc, char** argv) try {
    API generic = API::get<GenericService>();
    Model m("viam", "generic", "printer");

    std::shared_ptr<ModelRegistration> mr = std::make_shared<ModelRegistration>(
        generic,
        m,
        [](Dependencies deps, ResourceConfig cfg) { return std::make_unique<Printer>(deps, cfg); },
        // Custom validation can be done by specifying a validate function like
        // this one. Validate functions can `throw` exceptions that will be
        // returned to the parent through gRPC. Validate functions can also return
        // a vector of strings representing the implicit dependencies of the resource.
        [](ResourceConfig cfg) -> std::vector<std::string> {
            // find_to_print will throw an error if the `to_print` attribute
            // is missing, is not a string or is an empty string.
            Printer::find_to_print(cfg);
            return {};
        });

    std::vector<std::shared_ptr<ModelRegistration>> mrs = {mr};
    auto my_mod = std::make_shared<ModuleService>(argc, argv, mrs);
    my_mod->serve();

    return EXIT_SUCCESS;
} catch (const viam::sdk::Exception& ex) {
    std::cerr << "main failed with exception: " << ex.what() << "\n";
    return EXIT_FAILURE;
}
