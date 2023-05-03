/// @file components/board/client.hpp
///
/// @brief Implements a gRPC client for the `Board` component.
#pragma once

#include <grpcpp/channel.h>

#include <viam/api/component/board/v1/board.grpc.pb.h>

#include <viam/sdk/components/board/board.hpp>
#include <viam/sdk/components/board/server.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {

/// @class BoardClient
/// @brief gRPC client implementation of a `Board` component.
/// @ingroup Board
class BoardClient : public Board {
   public:
    BoardClient(std::string name, std::shared_ptr<grpc::Channel> channel);

    status get_status() override;
    void set_gpio(const std::string& pin, bool high) override;
    bool get_gpio(const std::string& pin) override;
    double get_pwm_duty_cycle(const std::string& pin) override;
    void set_pwm_duty_cycle(const std::string& pin, double duty_cycle_pct) override;
    uint64_t get_pwm_frequency(const std::string& pin) override;
    void set_pwm_frequency(const std::string& pin, uint64_t frequency_hz) override;
    AttributeMap do_command(AttributeMap command) override;
    analog_value read_analog(const std::string& analog_reader_name) override;
    digital_value read_digital_interrupt(const std::string& digital_interrupt_name) override;
    std::vector<std::string> get_analog_reader_names() override;
    std::vector<std::string> get_digital_interrupt_names() override;
    void set_power_mode(power_mode power_mode,
                        const std::chrono::duration<double>& duration) override;

   private:
    std::unique_ptr<viam::component::board::v1::BoardService::StubInterface> stub_;
    const std::shared_ptr<grpc::Channel> channel_;
};

}  // namespace sdk
}  // namespace viam
