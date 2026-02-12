
#include <iostream>
#include <memory>
#include <vector>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/instance.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/components/movement_sensor.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/log/logging.hpp>
#include <viam/sdk/module/service.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/reconfigurable.hpp>

    
class {{ .ModelPascal }} : public viam::sdk::MovementSensor, public viam::sdk::Reconfigurable {
public:
    {{ .ModelPascal }}(const viam::sdk::Dependencies& deps, const viam::sdk::ResourceConfig& cfg) : MovementSensor(cfg.name()) {
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

    viam::sdk::Vector3 get_linear_velocity(const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"get_linear_velocity\" not implemented");
    }

    viam::sdk::Vector3 get_angular_velocity(const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"get_angular_velocity\" not implemented");
    }

    viam::sdk::MovementSensor::compassheading get_compass_heading(const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"get_compass_heading\" not implemented");
    }

    viam::sdk::MovementSensor::orientation get_orientation(const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"get_orientation\" not implemented");
    }

    viam::sdk::MovementSensor::position get_position(const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"get_position\" not implemented");
    }

    viam::sdk::MovementSensor::properties get_properties(const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"get_properties\" not implemented");
    }

    std::unordered_map<std::string, float> get_accuracy(const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"get_accuracy\" not implemented");
    }

    viam::sdk::Vector3 get_linear_acceleration(const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"get_linear_acceleration\" not implemented");
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

