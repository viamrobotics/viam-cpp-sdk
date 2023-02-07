#include <common/v1/common.pb.h>
#include <component/arm/v1/arm.grpc.pb.h>
#include <component/arm/v1/arm.pb.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/support/status.h>
#include <robot/v1/robot.grpc.pb.h>
#include <robot/v1/robot.pb.h>
#include <unistd.h>

#include <boost/optional.hpp>
#include <components/component_base.hpp>
#include <components/generic.hpp>
#include <cstddef>
#include <functional>
#include <iostream>
#include <memory>
#include <module/service.hpp>
#include <ostream>
#include <registry/registry.hpp>
#include <robot/client.hpp>
#include <robot/service.hpp>
#include <rpc/dial.hpp>
#include <string>
#include <vector>

using viam::component::arm::v1::ArmService;
using viam::robot::v1::Status;
using Viam::SDK::Credentials;
using Viam::SDK::Options;

class MyModule : public ArmService::Service, public Module, public ComponentBase {
   public:
    using Module::Module;
    MyModule(std::string addr) : Module(addr), ArmService::Service::Service(){};
    ::grpc::Status MoveToPosition(
        ::grpc::ServerContext* context,
        const ::viam::component::arm::v1::MoveToPositionRequest* request,
        ::viam::component::arm::v1::MoveToPositionResponse* response) override {
        return ::grpc::Status();
    }
    ~MyModule() override = default;

    ::grpc::Status MoveToJointPositions(
        ::grpc::ServerContext* context,
        const ::viam::component::arm::v1::MoveToJointPositionsRequest* request,
        ::viam::component::arm::v1::MoveToJointPositionsResponse* response) override {
        return grpc::Status();
    }

    ::grpc::Status IsMoving(::grpc::ServerContext* context,
                            const ::viam::component::arm::v1::IsMovingRequest* request,
                            ::viam::component::arm::v1::IsMovingResponse* response) override {
        return grpc::Status();
    }

    ::grpc::Status Stop(::grpc::ServerContext* context,
                        const ::viam::component::arm::v1::StopRequest* request,
                        ::viam::component::arm::v1::StopResponse* response) override {
        return grpc::Status();
    }

    ::grpc::Status GetEndPosition(
        ::grpc::ServerContext* context,
        const ::viam::component::arm::v1::GetEndPositionRequest* request,
        ::viam::component::arm::v1::GetEndPositionResponse* response) override {
        response->mutable_pose()->set_x(1);
        response->mutable_pose()->set_y(2);
        response->mutable_pose()->set_z(3);
        response->mutable_pose()->set_o_x(4);
        response->mutable_pose()->set_o_y(5);
        response->mutable_pose()->set_o_z(6);
        response->mutable_pose()->set_theta(7);
        return grpc::Status();
    };

    ::grpc::Status GetJointPositions(
        ::grpc::ServerContext* context,
        const ::viam::component::arm::v1::GetJointPositionsRequest* request,
        ::viam::component::arm::v1::GetJointPositionsResponse* response) override {
        return ::grpc::Status();
    }
};

// MyModule::MyModule(std::string addr) : Module(addr), ArmService::Service::Service(){};

int main() {
    const char* uri = "<your robot URI here>";
    Viam::SDK::DialOptions dial_options = Viam::SDK::DialOptions();
    std::string payload = "<your payload here>";
    Credentials credentials(payload);
    dial_options.credentials = credentials;
    boost::optional<Viam::SDK::DialOptions> opts(dial_options);
    std::string address(uri);
    Options options = Options(1, opts);
    std::shared_ptr<RobotClient> robot = RobotClient::at_address(address, options);
    robot->refresh();
    std::vector<ResourceName>* resource_names = robot->resource_names();
    ResourceName the_one_we_care_about = resource_names->at(0);
    for (ResourceName resource : *resource_names) {
        std::cout << "Resource name: " << resource.name() << resource.type() << resource.subtype()
                  << std::endl;
    }
    std::vector<Status> status_plural = robot->get_status();
    std::cout << "Status plural len " << status_plural.size() << std::endl;
    for (Status s : status_plural) {
        std::cout << " Status! " << s.name().subtype() << std::endl;
    }

    std::vector<ResourceName> just_one = std::vector<ResourceName>();
    just_one.push_back(the_one_we_care_about);
    std::vector<Status> status_singular = robot->get_status(just_one);
    std::cout << "Status singular len " << status_singular.size() << std::endl;
    for (Status s : status_singular) {
        std::cout << " Status! " << s.name().subtype() << std::endl;
    }

    std::shared_ptr<RobotService_> robot_service = RobotService_::create();

    std::shared_ptr<MyModule> my_module =
        std::make_shared<MyModule>(std::string("/tmp/abc123.sock"));
    *my_module->parent = robot;
    ModuleService_ ms(my_module);

    Registry reg;
    ComponentRegistration cr;
    cr.component_type = ComponentType("generic");
    cr.name = "my component";

    robot->close();
    return 0;
}

