
#include <iostream>
#include <memory>
#include <vector>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/instance.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/components/encoder.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/log/logging.hpp>
#include <viam/sdk/module/service.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/reconfigurable.hpp>

    
class {{ .ModelPascal }} : public viam::sdk::Encoder, public viam::sdk::Reconfigurable {
public:
    {{ .ModelPascal }}(const viam::sdk::Dependencies& deps, const viam::sdk::ResourceConfig& cfg) : Encoder(cfg.name()) {
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

    viam::sdk::Encoder::position get_position(
        const viam::sdk::ProtoStruct & extra,
        viam::sdk::Encoder::position_type position_type) override
    {
        throw std::logic_error("\"get_position\" not implemented");
    }

    void reset_position(const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"reset_position\" not implemented");
    }

    viam::sdk::Encoder::properties get_properties(const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"get_properties\" not implemented");
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

