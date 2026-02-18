
#include <iostream>
#include <memory>
#include <vector>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/instance.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/log/logging.hpp>
#include <viam/sdk/module/service.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/reconfigurable.hpp>
#include <viam/sdk/services/navigation.hpp>
    
class {{ .ModelPascal }} : public viam::sdk::Navigation, public viam::sdk::Reconfigurable {
public:
    {{ .ModelPascal }}(const viam::sdk::Dependencies& deps, const viam::sdk::ResourceConfig& cfg) : Navigation(cfg.name()) {
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

    viam::sdk::Navigation::Mode get_mode(const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"get_mode\" not implemented");
    }

    void set_mode(
        const viam::sdk::Navigation::Mode mode,
        const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"set_mode\" not implemented");
    }

    viam::sdk::Navigation::LocationResponse get_location(const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"get_location\" not implemented");
    }

    std::vector<viam::sdk::Navigation::Waypoint> get_waypoints(const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"get_waypoints\" not implemented");
    }

    void add_waypoint(
        const viam::sdk::geo_point & location,
        const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"add_waypoint\" not implemented");
    }

    void remove_waypoint(
        const std::string id,
        const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"remove_waypoint\" not implemented");
    }

    std::vector<viam::sdk::geo_geometry> get_obstacles(const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"get_obstacles\" not implemented");
    }

    std::vector<viam::sdk::Navigation::Path> get_paths(const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"get_paths\" not implemented");
    }

    viam::sdk::Navigation::Properties get_properties() override
    {
        throw std::logic_error("\"get_properties\" not implemented");
    }

    viam::sdk::ProtoStruct do_command(const viam::sdk::ProtoStruct & command) override
    {
        throw std::logic_error("\"do_command\" not implemented");
    }

};

