#include <viam/sdk/rpc/dial.hpp>
#include <viam/sdk/app/app_client.hpp>

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

    /// @brief Get the AppClient for interacting with the Viam App API.
    /// @return A reference to the AppClient.
    AppClient& app_client() const;

   private:
    ViamClient(ViamChannel channel);

    ViamChannel channel_;
    std::shared_ptr<AppClient> app_client_;
};

}  // namespace sdk
}  // namespace viam