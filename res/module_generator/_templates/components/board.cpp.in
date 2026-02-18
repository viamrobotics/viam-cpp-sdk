
#include <iostream>
#include <memory>
#include <vector>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/instance.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/components/board.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/log/logging.hpp>
#include <viam/sdk/module/service.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/reconfigurable.hpp>

    
class {{ .ModelPascal }} : public viam::sdk::Board, public viam::sdk::Reconfigurable {
public:
    {{ .ModelPascal }}(const viam::sdk::Dependencies& deps, const viam::sdk::ResourceConfig& cfg) : Board(cfg.name()) {
        this->reconfigure(deps, cfg);
    }


    static std::vector<std::string> validate(const viam::sdk::ResourceConfig&)
    {
        throw std::runtime_error("\"validate\" not implemented");
    }

    void reconfigure(const viam::sdk::Dependencies&, const viam::sdk::ResourceConfig&) override
    {
        throw std::runtime_error("\"reconfigure\" not implemented");
    }

    bool get_gpio(
        const std::string & pin,
        const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"get_gpio\" not implemented");
    }

    void set_gpio(
        const std::string & pin,
        bool high,
        const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"set_gpio\" not implemented");
    }

    double get_pwm_duty_cycle(
        const std::string & pin,
        const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"get_pwm_duty_cycle\" not implemented");
    }

    void set_pwm_duty_cycle(
        const std::string & pin,
        double duty_cycle_pct,
        const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"set_pwm_duty_cycle\" not implemented");
    }

    uint64_t get_pwm_frequency(
        const std::string & pin,
        const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"get_pwm_frequency\" not implemented");
    }

    void set_pwm_frequency(
        const std::string & pin,
        uint64_t frequency_hz,
        const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"set_pwm_frequency\" not implemented");
    }

    viam::sdk::Board::analog_response read_analog(
        const std::string & analog_reader_name,
        const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"read_analog\" not implemented");
    }

    void write_analog(
        const std::string & pin,
        int value,
        const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"write_analog\" not implemented");
    }

    viam::sdk::Board::digital_value read_digital_interrupt(
        const std::string & digital_interrupt_name,
        const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"read_digital_interrupt\" not implemented");
    }

    void stream_ticks(
        const std::vector<std::string> & digital_interrupt_names,
        const std::function<bool (Tick &&)> & tick_handler,
        const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"stream_ticks\" not implemented");
    }

    void set_power_mode(
        viam::sdk::Board::power_mode power_mode,
        const viam::sdk::ProtoStruct & extra,
        const boost::optional<std::chrono::microseconds> & duration) override
    {
        throw std::logic_error("\"set_power_mode\" not implemented");
    }

    viam::sdk::ProtoStruct do_command(const viam::sdk::ProtoStruct & command) override
    {
        throw std::logic_error("\"do_command\" not implemented");
    }

    std::vector<viam::sdk::GeometryConfig> get_geometries(const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"get_geometries\" not implemented");
    }

};

