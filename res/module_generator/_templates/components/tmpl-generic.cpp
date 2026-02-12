
#include <iostream>
#include <memory>
#include <vector>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/instance.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/components/generic.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/log/logging.hpp>
#include <viam/sdk/module/service.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/reconfigurable.hpp>

    
class {{ .ModelPascal }} : public viam::sdk::Generic, public viam::sdk::Reconfigurable {
public:
    {{ .ModelPascal }}(const viam::sdk::Dependencies& deps, const viam::sdk::ResourceConfig& cfg) : Generic(cfg.name()) {
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

};

