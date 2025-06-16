#pragma once

#include <vector>

#include <viam/sdk/resource/reconfigurable.hpp>

#include "api.hpp"

using namespace viam::sdk;

// MySummation inherits from the `Summation` class defined in `api.hpp` and
// implements all relevant methods along with `reconfigure`.
class MySummation : public Summation, public Reconfigurable {
   public:
    MySummation(std::string name, bool subtract)
        : Summation(std::move(name)), subtract_(subtract) {}
    MySummation(const Dependencies& deps, const ResourceConfig& cfg) : Summation(cfg.name()) {
        this->reconfigure(deps, cfg);
    }
    void reconfigure(const Dependencies& deps, const ResourceConfig& cfg) override;
    static std::vector<std::string> validate(ResourceConfig cfg);

    double sum(std::vector<double> numbers) override;

   private:
    bool subtract_;
};
