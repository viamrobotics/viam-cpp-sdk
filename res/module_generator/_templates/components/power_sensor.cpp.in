
#include <iostream>
#include <memory>
#include <vector>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/instance.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/components/power_sensor.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/log/logging.hpp>
#include <viam/sdk/module/service.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/reconfigurable.hpp>

    
class {{ .ModelPascal }} : public viam::sdk::PowerSensor, public viam::sdk::Reconfigurable {
public:
    {{ .ModelPascal }}(const viam::sdk::Dependencies& deps, const viam::sdk::ResourceConfig& cfg) : PowerSensor(cfg.name()) {
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

    viam::sdk::PowerSensor::voltage get_voltage(const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"get_voltage\" not implemented");
    }

    viam::sdk::PowerSensor::current get_current(const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"get_current\" not implemented");
    }

    double get_power(const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"get_power\" not implemented");
    }

    viam::sdk::ProtoStruct get_readings(const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"get_readings\" not implemented");
    }

    viam::sdk::ProtoStruct do_command(const viam::sdk::ProtoStruct & command) override
    {
        throw std::logic_error("\"do_command\" not implemented");
    }

};

