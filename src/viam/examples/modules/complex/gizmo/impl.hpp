#pragma once

#include <vector>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/resource/reconfigurable.hpp>

#include "api.hpp"

using namespace viam::sdk;

// MyGizmo inherits from the `Gizmo` class defined in `api.hpp` and implements
// all relevant methods along with `reconfigure`. It also specifies a static
// `validate` method that checks config validity.
class MyGizmo : public Gizmo, public Reconfigurable {
   public:
    MyGizmo(std::string name, std::string arg1) : Gizmo(std::move(name)), arg1_(std::move(arg1)){};
    MyGizmo(Dependencies deps, ResourceConfig cfg) : Gizmo(cfg.name()) {
        this->reconfigure(deps, cfg);
    };
    void reconfigure(const Dependencies& deps, const ResourceConfig& cfg) override;
    static std::vector<std::string> validate(ResourceConfig cfg);

    bool do_one(std::string arg1) override;
    bool do_one_client_stream(std::vector<std::string> arg1) override;
    std::vector<bool> do_one_server_stream(std::string arg1) override;
    std::vector<bool> do_one_bidi_stream(std::vector<std::string> arg1) override;
    std::string do_two(bool arg1) override;

   private:
    std::string arg1_;
};
