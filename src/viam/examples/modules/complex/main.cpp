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
    API base_api = Base::static_api();
    Model mybase_model("viam", "base", "mybase");

    std::shared_ptr<ModelRegistration> mybase_mr = std::make_shared<ModelRegistration>(
        base_api,
        mybase_model,
        [](Dependencies deps, ResourceConfig cfg) { return std::make_unique<MyBase>(deps, cfg); },
        MyBase::validate);

    API gizmo_api = Gizmo::static_api();
    Model mygizmo_model("viam", "gizmo", "mygizmo");
    // Make sure to explicity register resources with custom APIs. Note that
    // this must be done in `main` and not in resource implementation files due
    // to order of static initialization.
    Registry::register_resource(gizmo_api, Gizmo::resource_registration());
    std::shared_ptr<ModelRegistration> mygizmo_mr = std::make_shared<ModelRegistration>(
        gizmo_api,
        mygizmo_model,
        [](Dependencies deps, ResourceConfig cfg) { return std::make_unique<MyGizmo>(deps, cfg); },
        MyGizmo::validate);

    API summation_api = Summation::static_api();
    Model mysummation_model("viam", "summation", "mysummation");
    // Make sure to explicity register resources with custom APIs. Note that
    // this must be done in `main` and not in resource implementation files due
    // to order of static initialization.
    Registry::register_resource(summation_api, Summation::resource_registration());

    std::shared_ptr<ModelRegistration> mysummation_mr = std::make_shared<ModelRegistration>(
        summation_api, mysummation_model, [](Dependencies deps, ResourceConfig cfg) {
            return std::make_unique<MySummation>(deps, cfg);
        });

    std::vector<std::shared_ptr<ModelRegistration>> mrs = {mybase_mr, mygizmo_mr, mysummation_mr};
    auto my_mod = std::make_shared<ModuleService>(argc, argv, mrs);
    // register_cleanup_function can be used to run custom logic at the time of
    // shutdown. It must be called before `serve`.
    my_mod->register_cleanup_function(
        []() { BOOST_LOG_TRIVIAL(info) << "C++ complex module example cleaning up"; });
    my_mod->serve();

    return EXIT_SUCCESS;
};
