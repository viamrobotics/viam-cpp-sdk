
#include <iostream>
#include <memory>
#include <vector>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/instance.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/components/pose_tracker.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/log/logging.hpp>
#include <viam/sdk/module/service.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/reconfigurable.hpp>

    
class {{ .ModelPascal }} : public viam::sdk::PoseTracker, public viam::sdk::Reconfigurable {
public:
    {{ .ModelPascal }}(const viam::sdk::Dependencies& deps, const viam::sdk::ResourceConfig& cfg) : PoseTracker(cfg.name()) {
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

    viam::sdk::PoseTracker::pose_map get_poses(
        const std::vector<std::string> & body_names,
        const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"get_poses\" not implemented");
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

