#include <viam/sdk/rpc/dial.hpp>

namespace viam {
namespace sdk {

class AppClient {
   public:
    static AppClient connect(const char* uri,
                             const Credentials& credentials,
                             const std::string& auth_entity);

   private:
    AppClient(ViamChannel channel);

    ViamChannel channel_;
};

}  // namespace sdk
}  // namespace viam
