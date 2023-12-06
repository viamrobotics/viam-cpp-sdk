#include "impl.hpp"

#include <vector>

#include <grpcpp/support/status.h>

#include <viam/sdk/components/component.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/resource/resource.hpp>

using namespace viam::sdk;

// Returns value of "subtract" in cfg.attributes() or `false` if subtract is not
// in attributes or is not a boolean value.
bool find_subtract(ResourceConfig cfg) {
    auto subtract = cfg.attributes()->find("subtract");
    if (subtract == cfg.attributes()->end()) {
        return false;
    }
    const bool* const subtract_bool = subtract->second->get<bool>();
    if (!subtract_bool) {
        return false;
    }
    return *subtract_bool;
}

void MySummation::reconfigure(Dependencies deps, ResourceConfig cfg) {
    subtract_ = find_subtract(cfg);
}

double MySummation::sum(std::vector<double> numbers) {
    if (numbers.empty()) {
        throw std::runtime_error("MySummation requires at least one number to sum");
    }

    double result = 0.0;
    for (double number : numbers) {
        if (subtract_) {
            result -= number;
        } else {
            result += number;
        }
    }

    return result;
}
