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
    std::cout << "at beginning!" << std::endl;
    Registry::initialize();
    API base_api = API::get<Base>();
    Model mybase_model("viam", "base", "mybase");

    std::shared_ptr<ModelRegistration> mybase_mr = std::make_shared<ModelRegistration>(
        base_api,
        mybase_model,
        [](Dependencies deps, ResourceConfig cfg) { return std::make_unique<MyBase>(deps, cfg); },
        MyBase::validate);

    API gizmo_api = API::get<Gizmo>();
    Model mygizmo_model("viam", "gizmo", "mygizmo");
    // Make sure to explicity register resources with custom APIs. Note that
    // this must be done in `main` and not in resource implementation files due
    // to order of static initialization.
    std::cout << "initialized registry, about to register gizmo" << std::endl;
    Registry::register_resource(gizmo_api, Gizmo::resource_registration());
    std::shared_ptr<ModelRegistration> mygizmo_mr = std::make_shared<ModelRegistration>(
        gizmo_api,
        mygizmo_model,
        [](Dependencies deps, ResourceConfig cfg) { return std::make_unique<MyGizmo>(deps, cfg); },
        MyGizmo::validate);

    API summation_api = API::get<Summation>();
    Model mysummation_model("viam", "summation", "mysummation");
    // Make sure to explicity register resources with custom APIs. Note that
    // this must be done in `main` and not in resource implementation files due
    // to order of static initialization.
    std::cout << "registered gizmo, about to register summation" << std::endl;
    Registry::register_resource<SummationClient, SummationServer, SummationService>(summation_api);
    // Registry::register_resource(summation_api, Summation::resource_registration());

    std::shared_ptr<ModelRegistration> mysummation_mr = std::make_shared<ModelRegistration>(
        summation_api, mysummation_model, [](Dependencies deps, ResourceConfig cfg) {
            return std::make_unique<MySummation>(deps, cfg);
        });

    std::vector<std::shared_ptr<ModelRegistration>> mrs = {mybase_mr, mygizmo_mr, mysummation_mr};
    std::cout << "creating modules service " << std::endl;
    auto my_mod = std::make_shared<ModuleService>(argc, argv, mrs);
    std::cout << "serving module " << std::endl;
    my_mod->serve();

    return EXIT_SUCCESS;
};
