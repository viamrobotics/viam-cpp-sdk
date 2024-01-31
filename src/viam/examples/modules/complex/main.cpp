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
    Registry::initialize();
    API base_api = API::get<Base>();
    API gizmo_api = API::get<Gizmo>();
    API summation_api = API::get<Summation>();
    Model mybase_model("viam", "base", "mybase");

    // // CR erodkin: add note about how we've reduced the comment here.
    // Make sure to explicity register resources with custom APIs.
    Registry::register_resource<GizmoClient, GizmoServer, GizmoService>(gizmo_api);
    Registry::register_resource<SummationClient, SummationServer, SummationService>(summation_api);

    std::shared_ptr<ModelRegistration> mybase_mr = std::make_shared<ModelRegistration>(
        base_api,
        mybase_model,
        [](Dependencies deps, ResourceConfig cfg) { return std::make_unique<MyBase>(deps, cfg); },
        MyBase::validate);

    Model mygizmo_model("viam", "gizmo", "mygizmo");
    // Make sure to explicity register resources with custom APIs.
    std::shared_ptr<ModelRegistration> mygizmo_mr = std::make_shared<ModelRegistration>(
        gizmo_api,
        mygizmo_model,
        [](Dependencies deps, ResourceConfig cfg) { return std::make_unique<MyGizmo>(deps, cfg); },
        MyGizmo::validate);

    Model mysummation_model("viam", "summation", "mysummation");

    std::shared_ptr<ModelRegistration> mysummation_mr = std::make_shared<ModelRegistration>(
        summation_api, mysummation_model, [](Dependencies deps, ResourceConfig cfg) {
            return std::make_unique<MySummation>(deps, cfg);
        });

    std::vector<std::shared_ptr<ModelRegistration>> mrs = {mybase_mr, mygizmo_mr, mysummation_mr};
    auto my_mod = std::make_shared<ModuleService>(argc, argv, mrs);
    my_mod->serve();

    return EXIT_SUCCESS;
};
