/// @file components/private/board_client.hpp
///
/// @brief Implements a gRPC client for the `Board` component.
#pragma once

#include <grpcpp/channel.h>

#include <viam/api/component/board/v1/board.grpc.pb.h>

#include <viam/sdk/components/board.hpp>
#include <viam/sdk/components/private/board_server.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {
namespace impl {

/// @class BoardClient
/// @brief gRPC client implementation of a `Board` component.
/// @ingroup Board
class BoardClient : public Board {
   public:
    using interface_type = Board;
    BoardClient(std::string name, std::shared_ptr<grpc::Channel> channel);
    ProtoStruct do_command(const ProtoStruct& command) override;
    void set_gpio(const std::string& pin, bool high, const ProtoStruct& extra) override;
    bool get_gpio(const std::string& pin, const ProtoStruct& extra) override;
    double get_pwm_duty_cycle(const std::string& pin, const ProtoStruct& extra) override;
    void set_pwm_duty_cycle(const std::string& pin,
                            double duty_cycle_pct,
                            const ProtoStruct& extra) override;
    uint64_t get_pwm_frequency(const std::string& pin, const ProtoStruct& extra) override;
    void set_pwm_frequency(const std::string& pin,
                           uint64_t frequency_hz,
                           const ProtoStruct& extra) override;
    analog_response read_analog(const std::string& analog_reader_name,
                                const ProtoStruct& extra) override;
    void write_analog(const std::string& pin, int value, const ProtoStruct& extra) override;
    digital_value read_digital_interrupt(const std::string& digital_interrupt_name,
                                         const ProtoStruct& extra) override;
    void set_power_mode(power_mode power_mode,
                        const ProtoStruct& extra,
                        const boost::optional<std::chrono::microseconds>& duration) override;
    std::vector<GeometryConfig> get_geometries(const ProtoStruct& extra) override;

    void stream_ticks(std::vector<std::string> const& digital_interrupt_names,
                      std::function<bool(Tick&& tick)> const& tick_handler,
                      const ProtoStruct& extra) override;

    // the `extra` param is frequently unnecessary but needs to be supported. Ideally, we'd
    // like to live in a world where implementers of derived classes don't need to go out of
    // their way to support two versions of a method (an `extra` version and a non-`extra`
    // version), and users don't need to pass an unnecessary parameters to all method calls.
    //
    // To do this, we define in the parent resource class a non-virtual version of the methods
    // that calls the virtual method and passes a `nullptr` by default in place of the `extra`
    // param. In order to access these versions of the methods within the client code, however,
    // we need to include these `using` lines.
    using Board::get_geometries;
    using Board::get_gpio;
    using Board::get_pwm_duty_cycle;
    using Board::get_pwm_frequency;
    using Board::read_analog;
    using Board::read_digital_interrupt;
    using Board::set_gpio;
    using Board::set_power_mode;
    using Board::set_pwm_duty_cycle;
    using Board::set_pwm_frequency;
    using Board::stream_ticks;
    using Board::write_analog;

   private:
    using StubType = viam::component::board::v1::BoardService::StubInterface;
    std::unique_ptr<StubType> stub_;
    const std::shared_ptr<grpc::Channel> channel_;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
