#pragma once

#include <optional>
#include <string>
#include <vector>

namespace viam {
namespace sdk {
namespace impl {

// A link or a joint as the tree checks see it, so that a model held as the sdk type and one held
// as the message it is read from are checked by the same code.
struct kinematic_model_node {
    // "link" or "joint"
    const char* kind;
    std::string id;
    std::string parent;

    // the joint this one mimics, for a mimic joint
    std::optional<std::string> mimics;

    // whether a joint carries any limit of its own
    bool has_limits = false;
};

// Checks the references a model's author can get wrong. viam-server treats a parent it does not
// know as the world, so a misspelled parent has to be caught here or the link quietly moves to
// the root. A model with no nodes passes.
// Throws viam::sdk::Exception naming the link or joint at fault.
void validate_kinematic_model_nodes(const std::vector<kinematic_model_node>& nodes);

}  // namespace impl
}  // namespace sdk
}  // namespace viam
