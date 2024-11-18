#include <viam/sdk/services/navigation.hpp>

#include <viam/api/service/navigation/v1/navigation.pb.h>
#include <viam/sdk/common/private/proto_utils.hpp>
#include <viam/sdk/common/utils.hpp>

namespace viam {
namespace sdk {

Navigation::Navigation(std::string name) : Service(std::move(name)){};

API Navigation::api() const {
    return API::get<Navigation>();
}

API API::traits<Navigation>::api() {
    return {kRDK, kService, "navigation"};
}

}  // namespace sdk
}  // namespace viam
