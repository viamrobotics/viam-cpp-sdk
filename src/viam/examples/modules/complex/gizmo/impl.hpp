/// @file complex/gizmo/impl.hpp
///
/// @brief Implements a `Gizmo` component `MyGizmo`.
///
/// MyGizmo inherits from the `Gizmo` class defined in `api.hpp` and implements
/// all relevant methods along with `reconfigure`. It also specifies a static
/// `validate` method that checks config validity.
#pragma once

#include <vector>

#include "api.hpp"

using namespace viam::sdk;

class MyGizmo : public Gizmo {
   public:
    MyGizmo(Dependencies deps, ResourceConfig cfg) : Gizmo(cfg.name()) {
        this->reconfigure(deps, cfg);
    };
    void reconfigure(Dependencies deps, ResourceConfig cfg) override;
    static std::vector<std::string> validate(ResourceConfig cfg);

    bool do_one(std::string arg1) override;
    bool do_one_client_stream(std::vector<std::string> arg1) override;
    std::vector<bool> do_one_server_stream(std::string arg1) override;
    std::vector<bool> do_one_bidi_stream(std::vector<std::string> arg1) override;
    std::string do_two(bool arg1) override;

   private:
    std::string arg1_;
};