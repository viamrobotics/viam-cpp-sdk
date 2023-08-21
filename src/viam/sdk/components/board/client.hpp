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
    AttributeMap do_command(const AttributeMap& command) override;
    status get_status() override;
    status get_status(const AttributeMap& extra) override;
    void set_gpio(const std::string& pin, bool high) override;
    void set_gpio(const std::string& pin, bool high, const AttributeMap& extra) override;
    bool get_gpio(const std::string& pin) override;
    bool get_gpio(const std::string& pin, const AttributeMap& extra) override;
    double get_pwm_duty_cycle(const std::string& pin) override;
    double get_pwm_duty_cycle(const std::string& pin, const AttributeMap& extra) override;
    void set_pwm_duty_cycle(const std::string& pin, double duty_cycle_pct) override;
    void set_pwm_duty_cycle(const std::string& pin,
                            double duty_cycle_pct,
                            const AttributeMap& extra) override;
    uint64_t get_pwm_frequency(const std::string& pin) override;
    uint64_t get_pwm_frequency(const std::string& pin, const AttributeMap& extra) override;
    void set_pwm_frequency(const std::string& pin, uint64_t frequency_hz) override;
    void set_pwm_frequency(const std::string& pin,
                           uint64_t frequency_hz,
                           const AttributeMap& extra) override;
    analog_value read_analog(const std::string& analog_reader_name) override;
    analog_value read_analog(const std::string& analog_reader_name,
                             const AttributeMap& extra) override;
    digital_value read_digital_interrupt(const std::string& digital_interrupt_name) override;
    digital_value read_digital_interrupt(const std::string& digital_interrupt_name,
                                         const AttributeMap& extra) override;
    void set_power_mode(power_mode power_mode,
                        const boost::optional<std::chrono::microseconds>& duration) override;
    void set_power_mode(power_mode power_mode,
                        const AttributeMap& extra,
                        const boost::optional<std::chrono::microseconds>& duration) override;
    std::vector<GeometryConfig> get_geometries() override;
    std::vector<GeometryConfig> get_geometries(const AttributeMap& extra) override;

   private:
    std::unique_ptr<viam::component::board::v1::BoardService::StubInterface> stub_;
    const std::shared_ptr<grpc::Channel> channel_;
};

}  // namespace sdk
}  // namespace viam
