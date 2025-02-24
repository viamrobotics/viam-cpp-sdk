#include <memory>
#include <signal.h>

#include <boost/log/trivial.hpp>
#include <grpcpp/grpcpp.h>
#include <grpcpp/server_context.h>

#include <viam/sdk/common/instance.hpp>
#include <viam/sdk/components/base.hpp>
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
    // Every Viam C++ SDK program must have one and only one Instance object which is created before
    // any other C++ SDK objects and stays alive until all Viam C++ SDK objects are destroyed.
    Instance inst;

    Model mybase_model("viam", "base", "mybase");

    // Make sure to explicity register resources with custom APIs.
    inst.registry()->register_resource_server<GizmoServer>();
    inst.registry()->register_resource_server<SummationServer>();

    std::shared_ptr<ModelRegistration> mybase_mr = std::make_shared<ModelRegistration>(
        API::get<Base>(),
        mybase_model,
        [](Dependencies deps, ResourceConfig cfg) { return std::make_unique<MyBase>(deps, cfg); },
        MyBase::validate);

    Model mygizmo_model("viam", "gizmo", "mygizmo");
    std::shared_ptr<ModelRegistration> mygizmo_mr = std::make_shared<ModelRegistration>(
        API::get<Gizmo>(),
        mygizmo_model,
        [](Dependencies deps, ResourceConfig cfg) { return std::make_unique<MyGizmo>(deps, cfg); },
        MyGizmo::validate);

    Model mysummation_model("viam", "summation", "mysummation");

    std::shared_ptr<ModelRegistration> mysummation_mr = std::make_shared<ModelRegistration>(
        API::get<Summation>(), mysummation_model, [](Dependencies deps, ResourceConfig cfg) {
            return std::make_unique<MySummation>(deps, cfg);
        });

    std::vector<std::shared_ptr<ModelRegistration>> mrs = {mybase_mr, mygizmo_mr, mysummation_mr};
    auto my_mod = std::make_shared<ModuleService>(argc, argv, mrs, inst.registry());
    my_mod->serve();

    return EXIT_SUCCESS;
};
