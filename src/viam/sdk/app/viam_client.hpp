#include <viam/sdk/rpc/dial.hpp>

namespace viam {
namespace sdk {

class ViamClient {
   public:
    static ViamClient from_env();

    const ViamChannel& channel() const;

   private:
    ViamClient(ViamChannel channel);

    ViamChannel channel_;
};

}  // namespace sdk
}  // namespace viam
