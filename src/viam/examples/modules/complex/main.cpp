#include <memory>
#include <signal.h>

#include <boost/log/trivial.hpp>
#include <grpcpp/grpcpp.h>
#include <grpcpp/server_context.h>

#include <viam/api/common/v1/common.grpc.pb.h>
#include <viam/api/component/generic/v1/generic.grpc.pb.h>
#include <viam/api/robot/v1/robot.pb.h>

#include <viam/sdk/components/base/base.hpp>
#include <viam/sdk/components/component.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/module/module.hpp>
#include <viam/sdk/module/service.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/rpc/dial.hpp>
#include <viam/sdk/rpc/server.hpp>

#include "base/impl.hpp"
#include "gizmo/api.hpp"
#include "gizmo/impl.hpp"
#include "summation/api.hpp"
#include "summation/impl.hpp"

using namespace viam::sdk;

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Need socket path as command line argument" << std::endl;
        return EXIT_FAILURE;
    }
    std::string socket_addr = argv[1];

    // Use set_logger_severity_from_args to set the boost trivial logger's
    // severity depending on commandline arguments.
    set_logger_severity_from_args(argc, argv);
    BOOST_LOG_TRIVIAL(debug) << "Starting module with debug level logging";

    // C++ modules must handle SIGINT and SIGTERM. You can use the SignalManager
    // class and its wait method to handle the correct signals.
    SignalManager signals;

    API base_api = Base::static_api();
    Model mybase_model("viam", "base", "mybase");

    std::shared_ptr<ModelRegistration> mybase_mr = std::make_shared<ModelRegistration>(
        ResourceType("Base"),
        base_api,
        mybase_model,
        [](Dependencies deps, ResourceConfig cfg) { return std::make_unique<MyBase>(deps, cfg); },
        MyBase::validate);

    Registry::register_model(mybase_mr);

    API gizmo_api = Gizmo::static_api();
    Model mygizmo_model("viam", "gizmo", "mygizmo");
    // Make sure to explicity register resources with custom APIs. Note that
    // this must be done in `main` and not in resource implementation files due
    // to order of static initialization.
    Registry::register_resource(gizmo_api, Gizmo::resource_registration());

    std::shared_ptr<ModelRegistration> mygizmo_mr = std::make_shared<ModelRegistration>(
        ResourceType("Gizmo"),
        gizmo_api,
        mygizmo_model,
        [](Dependencies deps, ResourceConfig cfg) { return std::make_unique<MyGizmo>(deps, cfg); },
        MyGizmo::validate);

    Registry::register_model(mygizmo_mr);

    API summation_api = Summation::static_api();
    Model mysummation_model("viam", "summation", "mysummation");
    // Make sure to explicity register resources with custom APIs. Note that
    // this must be done in `main` and not in resource implementation files due
    // to order of static initialization.
    Registry::register_resource(summation_api, Summation::resource_registration());

    std::shared_ptr<ModelRegistration> mysummation_mr =
        std::make_shared<ModelRegistration>(ResourceType("Summation"),
                                            summation_api,
                                            mysummation_model,
                                            [](Dependencies deps, ResourceConfig cfg) {
                                                return std::make_unique<MySummation>(deps, cfg);
                                            });

    Registry::register_model(mysummation_mr);

    // The `ModuleService_` must outlive the Server, so the declaration order
    // here matters.
    auto my_mod = std::make_shared<ModuleService_>(socket_addr);
    auto server = std::make_shared<Server>();

    my_mod->add_model_from_registry(server, base_api, mybase_model);
    my_mod->add_model_from_registry(server, gizmo_api, mygizmo_model);
    my_mod->add_model_from_registry(server, summation_api, mysummation_model);
    my_mod->start(server);
    BOOST_LOG_TRIVIAL(info) << "Complex example module listening on " << socket_addr;

    server->start();
    int sig = 0;
    auto result = signals.wait(&sig);
    server->shutdown();
    return EXIT_SUCCESS;
};
