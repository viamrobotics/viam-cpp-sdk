#include <viam/sdk/rpc/dial.hpp>
#include <viam/sdk/app/billing.hpp>

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

    std::shared_ptr<BillingClient> billing_client();

   private:
    ViamClient(ViamChannel channel);

    ViamChannel channel_;
    std::shared_ptr<BillingClient> billing_client_;
};

}  // namespace sdk
}  // namespace viam