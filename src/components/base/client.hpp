#pragma once

#include <grpcpp/channel.h>

#include <component/base/v1/base.grpc.pb.h>

#include <components/base/base.hpp>
#include <components/base/server.hpp>
#include <config/resource.hpp>
#include <robot/client.hpp>

class BaseClient : public Base {
   public:
    AttributeMap do_command(AttributeMap command) override;
    void move_straight(int64_t distance_mm, double mm_per_sec) override;
    void spin(double angle_deg, double degs_per_sec) override;
    void stop() override;
    void set_power(const viam::common::v1::Vector3& linear,
                   const viam::common::v1::Vector3& angular) override;
    void set_velocity(const viam::common::v1::Vector3& linear,
                      const viam::common::v1::Vector3& angular) override;
    bool is_moving() override;
    BaseClient(std::string name, std::shared_ptr<grpc::Channel> channel)
        : Base(std::move(name)),
          stub_(viam::component::base::v1::BaseService::NewStub(channel)),
          channel_(std::move(channel)){};

   protected:
    // This constructor leaves the `channel_` as a nullptr. This is useful for testing
    // purposes, but renders it unusable for production use. Care should be taken to
    // avoid use of this constructor outside of tests.
    BaseClient(std::string name,
               std::unique_ptr<viam::component::base::v1::BaseService::StubInterface> stub)
        : Base(std::move(name)), stub_(std::move(stub)){};

   private:
    std::unique_ptr<viam::component::base::v1::BaseService::StubInterface> stub_;
    std::shared_ptr<grpc::Channel> channel_;
};
