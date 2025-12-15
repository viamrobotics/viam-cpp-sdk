#include <viam/sdk/rpc/dial.hpp>

namespace viam {
namespace sdk {

class ViamClient {
   public:
    /// @brief Create a ViamClient connecting to @param uri using the VIAM_API_KEY and
    /// VIAM_API_KEY_ID environment variables.
    static ViamClient from_env(const char* uri);

    /// @brief Create a ViamClient connecting to app.viam.com using the VIAM_API_KEY and
    /// VIAM_API_KEY_ID environment variables.
    static ViamClient from_env();

    const ViamChannel& channel() const;

   private:
    ViamClient(ViamChannel channel);

    ViamChannel channel_;
};

}  // namespace sdk
}  // namespace viam
