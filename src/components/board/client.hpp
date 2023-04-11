#pragma once

#include <grpcpp/channel.h>

#include <component/board/v1/board.grpc.pb.h>

#include <components/board/board.hpp>
#include <config/resource.hpp>
#include <robot/client.hpp>

class BoardClient : public Board {
   public:
    AttributeMap do_command(AttributeMap command) override;
    Board::board_status get_status() override;
    void set_gpio(std::string pin, bool high) override;
    bool get_gpio(std::string pin) override;
    double get_pwm(std::string pin) override;
    void set_pwm(std::string pin, double duty_cycle_pct) override;
    uint64_t get_pwm_frequency(std::string pin) override;
    void set_pwm_frequency(std::string pin, uint64_t frequency_hz) override;
    int32_t read_analog_reader(std::string analog_reader_name) override;
    int64_t get_digital_interrupt_value(std::string digital_interrupt_name) override;
    void set_power_mode(Board::power_mode power_mode,
                        const google::protobuf::Duration& duration) override;
    BoardClient(std::string name, std::shared_ptr<grpc::Channel> channel)
        : Board(std::move(name)),
          stub_(viam::component::board::v1::BoardService::NewStub(channel)),
          channel_(std::move(channel)){};

   protected:
    // This constructor leaves the channel_ as a nullptr. This is useful for testing
    // purposes, but renders it unusable for production use. Care should be taken to
    // avoid use of this constructor outside of tests.
    BoardClient(std::string name,
                std::unique_ptr<viam::component::board::v1::BoardService::StubInterface> stub)
        : Board(std::move(name)), stub_(std::move(stub)){};

   private:
    std::unique_ptr<viam::component::board::v1::BoardService::StubInterface> stub_;
    std::shared_ptr<grpc::Channel> channel_;
};
