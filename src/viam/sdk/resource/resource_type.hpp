#pragma once

#include <string>

namespace viam {
namespace sdk {

class ResourceType {
   public:
    const std::string to_string() const;
    friend bool operator==(const ResourceType& lhs, const ResourceType& rhs);
    ResourceType(std::string type) : type_(std::move(type)){};

   private:
    std::string type_;
};

}  // namespace sdk
}  // namespace viam
