#include <iostream>
#include <sstream>
#include <memory>
#include <signal.h>

#include <boost/log/trivial.hpp>
#include <grpcpp/grpcpp.h>
#include <grpcpp/server_context.h>

#include <viam/api/common/v1/common.grpc.pb.h>
#include <viam/api/component/generic/v1/generic.grpc.pb.h>
#include <viam/api/robot/v1/robot.pb.h>

#include <viam/sdk/components/component.hpp>
#include <viam/sdk/components/generic/generic.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/module/module.hpp>
#include <viam/sdk/module/service.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/rpc/dial.hpp>
#include <viam/sdk/rpc/server.hpp>

using viam::component::generic::v1::GenericService;
using namespace viam::sdk;

// Printer is a modular resource that can print a to_print value to STDOUT when
// a DoCommand request is received or when reconfiguring. The to_print value
// must be provided as an attribute in the config.
class Printer : public Generic {
   public:
    void reconfigure(Dependencies deps, ResourceConfig cfg) {
        std::cout << "Printer " << Resource::name() << " is reconfiguring" << std::endl;
        for (auto& dep : deps) {
            std::cout << "dependency: " << dep.first.to_string() << std::endl;
        }
        to_print_ = find_to_print(cfg);
        std::cout << "Printer " << Resource::name() << " will now print " << to_print_ << std::endl;
    }

    Printer(Dependencies deps, ResourceConfig cfg) : Generic(cfg.name()) {
        std::cout << "Creating Printer " + Resource::name() << std::endl;
        to_print_ = find_to_print(cfg);
        std::cout << "Printer " << Resource::name() << " will print " << to_print_ << std::endl;
    }

    AttributeMap do_command(AttributeMap command) {
        std::cout << "Received DoCommand request for Printer " << Resource::name() << std::endl;
        std::cout << "Printer " << Resource::name() << " has printed " << to_print_ << std::endl;
        return command;
    }

    std::vector<GeometryConfig> get_geometries() {
        return std::vector<GeometryConfig>();
    }

    static std::string find_to_print(ResourceConfig cfg) {
        auto printer_name = cfg.name();
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

int main(int argc, char** argv) {
    if (argc < 2) {
        throw std::invalid_argument("Need socket path as command line argument");
    }
    std::string socket_addr = argv[1];

    // Use set_logger_severity_from_args to set the boost trivial logger's
    // severity depending on commandline arguments.
    set_logger_severity_from_args(argc, argv);
    BOOST_LOG_TRIVIAL(debug) << "Starting module with debug level logging";

    // C++ modules must handle SIGINT and SIGTERM. You can use the SignalManager
    // class and its wait method to handle the correct signals.
    SignalManager signals;

    API generic = Generic::static_api();
    Model m("viam", "generic", "printer");

    std::shared_ptr<ModelRegistration> mr = std::make_shared<ModelRegistration>(
        ResourceType("Printer"),
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

    Registry::register_model(mr);

    // The `ModuleService_` must outlive the Server, so the declaration order
    // here matters.
    auto my_mod = std::make_shared<ModuleService_>(socket_addr);
    auto server = std::make_shared<Server>();

    my_mod->add_model_from_registry(server, generic, m);
    my_mod->start(server);
    BOOST_LOG_TRIVIAL(info) << "Module serving model " << m.to_string() << ", listening on "
                            << socket_addr;

    server->start();
    int sig = 0;
    auto result = signals.wait(&sig);
    server->shutdown();
    return 0;
};
