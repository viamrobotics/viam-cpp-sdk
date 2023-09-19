/// @file complex/summation/impl.hpp
///
/// @brief Implements a `Summation` service `MySummation`.
///
/// MySummation inherits from the `Summation` class defined in `api.hpp` and implements
/// all relevant methods along with `reconfigure`.
#pragma once

#include <vector>

#include "api.hpp"

using namespace viam::sdk;

class MySummation : public Summation {
   public:
    MySummation(Dependencies deps, ResourceConfig cfg) : Summation(cfg.name()) {
        this->reconfigure(deps, cfg);
    };
    void reconfigure(Dependencies deps, ResourceConfig cfg) override;
    static std::vector<std::string> validate(ResourceConfig cfg);

    double sum(std::vector<double> numbers) override;

   private:
    bool subtract_;
};
