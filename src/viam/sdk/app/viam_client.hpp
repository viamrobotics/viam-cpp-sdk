#include <viam/api/app/v1/app.grpc.pb.h>
#include <viam/api/app/v1/app.pb.h>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/rpc/dial.hpp>

namespace viam {
namespace sdk {

class ViamClient {
   public:
    /**
     * @brief Upload a device push token for the user.
     * @param app_id The ID of the app.
     * @param device_token The device token to upload.
     * @param device_uuid The UUID of the device.
     * @param subtype Optional `RPCSubtype` for additional request parameters.
     */
    void UploadDevicePushToken(const std::string& app_id,
                               const std::string& device_token,
                               const std::string& device_uuid,
                               const RPCSubtype& subtype = {});

    /**
     * @brief Delete a device push token for the user.
     * @param app_id The ID of the app.
     * @param device_uuid The UUID of the device.
     * @param subtype Optional `RPCSubtype` for additional request parameters.
     */
    void DeleteDevicePushToken(const std::string& app_id,
                               const std::string& device_uuid,
                               const RPCSubtype& subtype = {});

    /**
     * @brief Gets all device push tokens for the user for the given app ID.
     * @param app_id The ID of the app.
     * @param subtype Optional `RPCSubtype` for additional request parameters.
     * @return A vector of device tokens.
     */
    std::vector<std::string> GetDevicePushTokens(const std::string& app_id,
                                                 const RPCSubtype& subtype = {});

    /**
     * @brief Set the Firebase config JSON for a specific app ID.
     * @param org_id The ID of the organization.
     * @param app_id The ID of the app.
     * @param config_json The Firebase config JSON string.
     * @param subtype Optional `RPCSubtype` for additional request parameters.
     */
    void SetFirebaseConfig(const std::string& org_id,
                           const std::string& app_id,
                           const std::string& config_json,
                           const RPCSubtype& subtype = {});

    /**
     * @brief Read the app ID for an organization.
     * This returns only the app_id configured for the org, not the Firebase config JSON.
     * @param org_id The ID of the organization.
     * @param subtype Optional `RPCSubtype` for additional request parameters.
     * @return The app ID.
     */
    std::string GetFirebaseConfig(const std::string& org_id, const RPCSubtype& subtype = {});

    /**
     * @brief Deletes a Firebase config JSON for a specific app ID.
     * @param org_id The ID of the organization.
     * @param app_id The ID of the app.
     * @param subtype Optional `RPCSubtype` for additional request parameters.
     */
    void DeleteFirebaseConfig(const std::string& org_id,
                              const std::string& app_id,
                              const RPCSubtype& subtype = {});

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
    std::unique_ptr<viam::app::v1::AppService::Stub> app_service_stub_;
};

}  // namespace sdk
}  // namespace viam