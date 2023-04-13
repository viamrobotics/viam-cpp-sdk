#pragma once

#include <string>

namespace viam {
namespace cppsdk {

class ResourceType {
   public:
    std::string to_string();
    friend bool operator==(const ResourceType& lhs, const ResourceType& rhs);
    ResourceType(std::string type) : type(std::move(type)){};

   private:
    std::string type;
};

}  // namespace cppsdk
}  // namespace viam
