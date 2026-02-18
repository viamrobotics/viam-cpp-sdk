
#include <iostream>
#include <memory>
#include <vector>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/instance.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/components/motor.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/log/logging.hpp>
#include <viam/sdk/module/service.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/reconfigurable.hpp>

    
class {{ .ModelPascal }} : public viam::sdk::Motor, public viam::sdk::Reconfigurable {
public:
    {{ .ModelPascal }}(const viam::sdk::Dependencies& deps, const viam::sdk::ResourceConfig& cfg) : Motor(cfg.name()) {
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

    void set_power(
        double power_pct,
        const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"set_power\" not implemented");
    }

    void go_for(
        double rpm,
        double revolutions,
        const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"go_for\" not implemented");
    }

    void go_to(
        double rpm,
        double position_revolutions,
        const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"go_to\" not implemented");
    }

    void set_rpm(
        double rpm,
        const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"set_rpm\" not implemented");
    }

    void reset_zero_position(
        double offset,
        const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"reset_zero_position\" not implemented");
    }

    viam::sdk::Motor::position get_position(const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"get_position\" not implemented");
    }

    viam::sdk::Motor::properties get_properties(const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"get_properties\" not implemented");
    }

    viam::sdk::Motor::power_status get_power_status(const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"get_power_status\" not implemented");
    }

    bool is_moving() override
    {
        throw std::logic_error("\"is_moving\" not implemented");
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

