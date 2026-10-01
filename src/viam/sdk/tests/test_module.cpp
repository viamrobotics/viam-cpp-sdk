#define BOOST_TEST_MODULE test module test_module
#include <viam/sdk/module/service.hpp>

#include <pthread.h>
#include <unistd.h>

#include <chrono>
#include <csignal>
#include <cstdlib>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include <boost/test/included/unit_test.hpp>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include <viam/api/module/v1/module.grpc.pb.h>
#include <viam/api/module/v1/module.pb.h>

#include <viam/sdk/components/motor.hpp>
#include <viam/sdk/components/sensor.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/rpc/server.hpp>
#include <viam/sdk/services/framesystem.hpp>
#include <viam/sdk/tests/mocks/mock_motor.hpp>
#include <viam/sdk/tests/mocks/mock_robot.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace module {

using namespace viam::sdk;

// A sensor that remembers the dependencies every instance was constructed with, so a test can
// see exactly what ModuleService handed to the model constructor.
class DepsSensor : public Sensor {
   public:
    DepsSensor(Dependencies deps, const ResourceConfig& cfg) : Sensor(cfg.name()) {
        const std::lock_guard<std::mutex> lock(lock_());
        constructed_().push_back(std::move(deps));
    }

    static std::vector<Dependencies> constructed() {
        const std::lock_guard<std::mutex> lock(lock_());
        return constructed_();
    }

    ProtoStruct do_command(const ProtoStruct& command) override {
        return command;
    }
    ProtoStruct get_status() override {
        return {};
    }
    std::vector<GeometryConfig> get_geometries(const ProtoStruct&) override {
        return {};
    }
    ProtoStruct get_readings(const ProtoStruct&) override {
        return {};
    }

   private:
    static std::mutex& lock_() {
        static std::mutex lock;
        return lock;
    }
    static std::vector<Dependencies>& constructed_() {
        static std::vector<Dependencies> constructed;
        return constructed;
    }
};

// Runs ModuleService::serve on its own thread and stops it again on destruction, so a failed
// assertion in the test body unwinds through here instead of terminating on a joinable thread.
class ServeThread {
   public:
    explicit ServeThread(ModuleService& module) : thread_([&module] { module.serve(); }) {}

    ~ServeThread() {
        // serve blocks in sigwait for SIGTERM, so aim the signal at that thread alone: the parent
        // server's grpc threads were created before ModuleService blocked SIGTERM and a process
        // wide signal could land on one of them and kill the test.
        pthread_kill(thread_.native_handle(), SIGTERM);
        thread_.join();
    }

   private:
    std::thread thread_;
};

// Stands up a MockRobotService as the parent on one unix socket and a ModuleService on another,
// then drives the module over gRPC the way viam-server does: Ready first, then whatever the test
// case sends through the module stub it is handed.
template <typename F>
void module_to_mock_parent_pipeline(const Model& model, F&& test_case) {
    char dir_template[] = "/tmp/viam_cpp_sdk_test_module_XXXXXX";
    BOOST_REQUIRE(mkdtemp(dir_template));
    const std::string dir(dir_template);
    const std::string parent_sock = dir + "/parent.sock";
    const std::string module_sock = dir + "/module.sock";

    // the mock parent only reports resources whose model is registered, like test_robot does
    Registry::get().register_model(std::make_shared<ModelRegistration>(
        API::get<Motor>(), Model("fake", "fake", "mock_motor"), [](Dependencies, ResourceConfig) {
            return motor::MockMotor::get_mock_motor();
        }));

    auto rm = std::make_shared<ResourceManager>();
    rm->add(std::string("mock_motor"), motor::MockMotor::get_mock_motor());
    auto parent_server = std::make_shared<sdk::Server>();
    robot::MockRobotService parent(rm, *parent_server);
    parent_server->add_listening_port("unix:" + parent_sock);
    parent_server->start();

    {
        ModuleService module(module_sock);
        module.add_model_from_registry(API::get<Sensor>(), model);
        // ModuleService blocked SIGTERM on this thread when it was constructed and the serving
        // thread inherits that mask, which is what lets serve pick the signal up with sigwait.
        ServeThread serving(module);

        auto channel =
            grpc::CreateChannel("unix:" + module_sock, grpc::InsecureChannelCredentials());
        auto stub = viam::module::v1::ModuleService::NewStub(channel);

        {
            grpc::ClientContext ctx;
            ctx.set_wait_for_ready(true);
            ctx.set_deadline(std::chrono::system_clock::now() + std::chrono::seconds(30));
            viam::module::v1::ReadyRequest request;
            request.set_raw_parent_address(parent_sock);
            viam::module::v1::ReadyResponse response;
            const auto status = stub->Ready(&ctx, request, &response);
            BOOST_REQUIRE_MESSAGE(status.ok(), status.error_message());
            BOOST_CHECK(response.ready());
        }

        std::forward<F>(test_case)(*stub, parent);
    }

    parent_server->shutdown();
    unlink(parent_sock.c_str());
    unlink(module_sock.c_str());
    rmdir(dir.c_str());
}

BOOST_AUTO_TEST_CASE(test_framesystem_seeded_into_dependencies) {
    Model model("viam", "test", "deps_sensor");
    Registry::get().register_model(std::make_shared<ModelRegistration>(
        API::get<Sensor>(), model, [](Dependencies deps, ResourceConfig cfg) {
            return std::make_shared<DepsSensor>(std::move(deps), cfg);
        }));

    module_to_mock_parent_pipeline(
        model, [&](viam::module::v1::ModuleService::StubInterface& stub, robot::MockRobotService&) {
            ResourceConfig cfg("sensor", "deps_sensor", "rdk", {}, "rdk:component:sensor", model);
            const Name motor_name = Name::from_string("rdk:component:motor/mock_motor");

            auto check_deps = [&](const Dependencies& deps) {
                // the frame system rides along with the one dependency viam-server actually sent
                BOOST_REQUIRE_EQUAL(deps.size(), 2);
                BOOST_CHECK(deps.find(motor_name) != deps.end());

                auto fs = deps.find(FrameSystem::public_name());
                BOOST_REQUIRE(fs != deps.end());
                auto frame_system = std::dynamic_pointer_cast<FrameSystem>(fs->second);
                BOOST_REQUIRE(frame_system);

                // and it is wired to the parent, not a placeholder
                BOOST_CHECK_EQUAL(frame_system->get_pose("mock_motor"),
                                  robot::mock_get_pose_response("world"));
            };

            {
                grpc::ClientContext ctx;
                viam::module::v1::AddResourceRequest request;
                *request.mutable_config() = to_proto(cfg);
                request.add_dependencies(motor_name.to_string());
                viam::module::v1::AddResourceResponse response;
                const auto status = stub.AddResource(&ctx, request, &response);
                BOOST_REQUIRE_MESSAGE(status.ok(), status.error_message());
            }

            auto constructed = DepsSensor::constructed();
            BOOST_REQUIRE_EQUAL(constructed.size(), 1);
            check_deps(constructed.back());

            {
                grpc::ClientContext ctx;
                viam::module::v1::ReconfigureResourceRequest request;
                *request.mutable_config() = to_proto(cfg);
                request.add_dependencies(motor_name.to_string());
                viam::module::v1::ReconfigureResourceResponse response;
                const auto status = stub.ReconfigureResource(&ctx, request, &response);
                BOOST_REQUIRE_MESSAGE(status.ok(), status.error_message());
            }

            constructed = DepsSensor::constructed();
            BOOST_REQUIRE_EQUAL(constructed.size(), 2);
            check_deps(constructed.back());
        });
}

}  // namespace module
}  // namespace sdktests
}  // namespace viam
