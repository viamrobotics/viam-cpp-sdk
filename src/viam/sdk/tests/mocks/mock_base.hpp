#pragma once

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/base/v1/base.grpc.pb.h>
#include <viam/api/component/base/v1/base.pb.h>

#include <viam/sdk/components/base/base.hpp>
#include <viam/sdk/components/base/client.hpp>
#include <viam/sdk/components/base/server.hpp>

namespace viam {
namespace sdktests {
namespace base {

class MockBase : public viam::sdk::Base {
   public:
    void move_straight(int64_t distance_mm, double mm_per_sec) override;
    void spin(double angle_deg, double degs_per_sec) override;
    void set_power(std::array<double, 3> linear, std::array<double, 3> angular) override;
    void set_velocity(std::array<double, 3> linear, std::array<double, 3> angular) override;
    grpc::StatusCode stop() override;
    bool is_moving() override;
    viam::sdk::AttributeMap do_command(viam::sdk::AttributeMap command) override;
    static std::shared_ptr<MockBase> get_mock_base();

    int64_t peek_move_straight_distance_mm;
    double peek_move_straight_mm_per_sec;
    double peek_spin_angle_deg, peek_spin_degs_per_sec;
    std::array<double, 3> peek_set_power_linear, peek_set_power_angular;
    std::array<double, 3> peek_set_velocity_linear, peek_set_velocity_angular;
    bool peek_stop_called;
    viam::sdk::AttributeMap peek_do_command_command;

    MockBase(std::string name) : Base(std::move(name)){};

   private:
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<viam::sdk::ProtoType>>> map;
};

}  // namespace base
}  // namespace sdktests
}  // namespace viam
