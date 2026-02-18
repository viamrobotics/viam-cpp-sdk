
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
#include <viam/sdk/services/motion.hpp>
    
class {{ .ModelPascal }} : public viam::sdk::Motion, public viam::sdk::Reconfigurable {
public:
    {{ .ModelPascal }}(const viam::sdk::Dependencies& deps, const viam::sdk::ResourceConfig& cfg) : Motion(cfg.name()) {
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

    bool move(
        const viam::sdk::pose_in_frame & destination,
        const std::string & name,
        const std::shared_ptr<viam::sdk::WorldState> & world_state,
        const std::shared_ptr<viam::sdk::Motion::constraints> & constraints,
        const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"move\" not implemented");
    }

    std::string move_on_map(
        const viam::sdk::pose & destination,
        const std::string & component_name,
        const std::string & slam_name,
        const std::shared_ptr<viam::sdk::motion_configuration> & motion_configuration,
        const std::vector<viam::sdk::GeometryConfig> & obstacles,
        const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"move_on_map\" not implemented");
    }

    std::string move_on_globe(
        const viam::sdk::geo_point & destination,
        const boost::optional<double> & heading,
        const std::string & component_name,
        const std::string & movement_sensor_name,
        const std::vector<viam::sdk::geo_geometry> & obstacles,
        const std::shared_ptr<viam::sdk::motion_configuration> & motion_configuration,
        const std::vector<viam::sdk::geo_geometry> & bounding_regions,
        const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"move_on_globe\" not implemented");
    }

    viam::sdk::pose_in_frame get_pose(
        const std::string & component_name,
        const std::string & destination_frame,
        const std::vector<viam::sdk::WorldState::transform> & supplemental_transforms,
        const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"get_pose\" not implemented");
    }

    void stop_plan(
        const std::string & component_name,
        const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"stop_plan\" not implemented");
    }

    viam::sdk::Motion::plan_with_status get_latest_plan(
        const std::string & component_name,
        const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"get_latest_plan\" not implemented");
    }

    std::pair<viam::sdk::Motion::plan_with_status, std::vector<viam::sdk::Motion::plan_with_status>>
    get_latest_plan_with_replan_history(
        const std::string & component_name,
        const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"get_latest_plan_with_replan_history\" not implemented");
    }

    viam::sdk::Motion::plan_with_status get_plan(
        const std::string & component_name,
        const std::string & execution_id,
        const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"get_plan\" not implemented");
    }

    std::pair<viam::sdk::Motion::plan_with_status, std::vector<viam::sdk::Motion::plan_with_status>>
    get_plan_with_replan_history(
        const std::string & component_name,
        const std::string & execution_id,
        const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"get_plan_with_replan_history\" not implemented");
    }

    std::vector<viam::sdk::Motion::plan_status_with_id> list_plan_statuses(const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"list_plan_statuses\" not implemented");
    }

    std::vector<viam::sdk::Motion::plan_status_with_id> list_active_plan_statuses(const viam::sdk::ProtoStruct & extra) override
    {
        throw std::logic_error("\"list_active_plan_statuses\" not implemented");
    }

    viam::sdk::ProtoStruct do_command(const viam::sdk::ProtoStruct & command) override
    {
        throw std::logic_error("\"do_command\" not implemented");
    }

};

