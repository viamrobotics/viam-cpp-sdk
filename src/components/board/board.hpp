#pragma once

#include <common/proto_type.hpp>
#include <common/utils.hpp>
#include <common/v1/common.pb.h>
#include <component/board/v1/board.pb.h>
#include <config/resource.hpp>
#include <google/protobuf/duration.pb.h>
#include <google/protobuf/struct.pb.h>
#include <map>
#include <registry/registry.hpp>
#include <string>
#include <subtype/subtype.hpp>

class BoardSubtype : public ResourceSubtype {
   public:
    std::shared_ptr<ResourceServerBase> create_resource_server(
        std::shared_ptr<SubtypeService> svc) override;
    std::shared_ptr<ResourceBase> create_rpc_client(std::string name,
                                                    std::shared_ptr<grpc::Channel> chan) override;
    BoardSubtype(const google::protobuf::ServiceDescriptor* service_descriptor)
        : ResourceSubtype(service_descriptor){};
};

class Board : public ComponentBase {
   public:
    struct analog_status {
        int32_t value;
    };
    struct digital_interrupt_status {
        int64_t value;
    };
    struct board_status {
        std::map<std::string, analog_status> analogs;
        std::map<std::string, digital_interrupt_status> digital_interrupts;
    };
    enum power_mode { POWER_MODE_UNSPECIFIED, POWER_MODE_NORMAL, POWER_MODE_OFFLINE_DEEP };

    static std::shared_ptr<ResourceSubtype> resource_subtype();
    static Subtype subtype();
    static board_status from_proto(viam::common::v1::BoardStatus proto);
    static analog_status from_proto(viam::common::v1::AnalogStatus proto);
    static digital_interrupt_status from_proto(viam::common::v1::DigitalInterruptStatus proto);
    static power_mode from_proto(viam::component::board::v1::PowerMode proto);
    static viam::common::v1::BoardStatus to_proto(board_status);
    static viam::common::v1::AnalogStatus to_proto(analog_status);
    static viam::common::v1::DigitalInterruptStatus to_proto(digital_interrupt_status);
    static viam::component::board::v1::PowerMode to_proto(power_mode);
    virtual AttributeMap do_command(AttributeMap command) = 0;
    virtual board_status get_status() = 0;
    virtual void set_gpio(std::string pin, bool high) = 0;
    virtual bool get_gpio(std::string pin) = 0;
    virtual double get_pwm(std::string pin) = 0;
    virtual void set_pwm(std::string pin, double duty_cycle_pct) = 0;
    virtual uint64_t get_pwm_frequency(std::string pin) = 0;
    virtual void set_pwm_frequency(std::string pin, uint64_t frequency_hz) = 0;
    virtual int32_t read_analog_reader(std::string analog_reader_name) = 0;
    virtual int64_t get_digital_interrupt_value(std::string digital_interrupt_name) = 0;
    // TODO(zack) do not expose protobuf duration
    virtual void set_power_mode(power_mode power_mode,
                                const google::protobuf::Duration& duration) = 0;

   protected:
    explicit Board(std::string name) : ComponentBase(std::move(name)){};
};
bool operator==(const Board::analog_status& lhs, const Board::analog_status& rhs);
bool operator==(const Board::digital_interrupt_status& lhs,
                const Board::digital_interrupt_status& rhs);
bool operator==(const Board::board_status& lhs, const Board::board_status& rhs);
