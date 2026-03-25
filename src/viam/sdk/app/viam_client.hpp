#include <viam/sdk/rpc/dial.hpp>
#include <viam/sdk/core/proto_utils.hpp>

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

    /// @brief Upload a device push token for the user.
    /// @param app_id The app ID.
    /// @param device_token The device push token.
    /// @param device_uuid The device UUID.
    /// @param extra Optional arguments.
    void UploadDevicePushToken(const std::string& app_id, const std::string& device_token, const std::string& device_uuid, const ProtoStruct& extra = {});

    /// @brief Delete a device push token for the user.
    /// @param app_id The app ID.
    /// @param device_uuid The device UUID.
    /// @param extra Optional arguments.
    void DeleteDevicePushToken(const std::string& app_id, const std::string& device_uuid, const ProtoStruct& extra = {});

    /// @brief Gets all device push tokens for the user for the given app ID.
    /// @param app_id The app ID.
    /// @param extra Optional arguments.
    /// @return A vector of device tokens.
    std::vector<std::string> GetDevicePushTokens(const std::string& app_id, const ProtoStruct& extra = {});

    /// @brief Set the Firebase config JSON for a specific app ID.
    /// @param org_id The organization ID.
    /// @param app_id The app ID.
    /// @param config_json The Firebase config JSON.
    /// @param extra Optional arguments.
    void SetFirebaseConfig(const std::string& org_id, const std::string& app_id, const std::string& config_json, const ProtoStruct& extra = {});

    /// @brief Read the app ID for an organization.
    /// This returns only the app_id configured for the org, not the Firebase config JSON.
    /// @param org_id The organization ID.
    /// @param extra Optional arguments.
    /// @return The app ID.
    std::string GetFirebaseConfig(const std::string& org_id, const ProtoStruct& extra = {});

    /// @brief Deletes a Firebase config JSON for a specific app ID.
    /// @param org_id The organization ID.
    /// @param app_id The app ID.
    /// @param extra Optional arguments.
    void DeleteFirebaseConfig(const std::string& org_id, const std::string& app_id, const ProtoStruct& extra = {});

   private:
    ViamClient(ViamChannel channel);

    ViamChannel channel_;
};

}  // namespace sdk
}  // namespace viam