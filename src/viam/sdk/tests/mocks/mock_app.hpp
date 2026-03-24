#pragma once

#include <string>
#include <vector>

#include <grpcpp/grpcpp.h>

#include <viam/api/app/v1/app.grpc.pb.h>
#include <viam/api/app/v1/app.pb.h>

namespace viam {
namespace sdktests {
namespace app {

/**
 * @brief MockAppService is a mock gRPC service for testing the AppService client.
 * It inherits from the auto-generated AppService::Service and overrides the virtual methods.
 * It stores the request data in public members for verification in tests.
 */
class MockAppService : public viam::app::v1::AppService::Service {
   public:
    grpc::Status UploadDevicePushToken(grpc::ServerContext* context,
                                       const viam::app::v1::UploadDevicePushTokenRequest* request,
                                       viam::app::v1::UploadDevicePushTokenResponse* response) override;
    grpc::Status DeleteDevicePushToken(grpc::ServerContext* context,
                                       const viam::app::v1::DeleteDevicePushTokenRequest* request,
                                       viam::app::v1::DeleteDevicePushTokenResponse* response) override;
    grpc::Status GetDevicePushTokens(grpc::ServerContext* context,
                                     const viam::app::v1::GetDevicePushTokensRequest* request,
                                     viam::app::v1::GetDevicePushTokensResponse* response) override;
    grpc::Status SetFirebaseConfig(grpc::ServerContext* context,
                                   const viam::app::v1::SetFirebaseConfigRequest* request,
                                   viam::app::v1::SetFirebaseConfigResponse* response) override;
    grpc::Status GetFirebaseConfig(grpc::ServerContext* context,
                                   const viam::app::v1::GetFirebaseConfigRequest* request,
                                   viam::app::v1::GetFirebaseConfigResponse* response) override;
    grpc::Status DeleteFirebaseConfig(grpc::ServerContext* context,
                                      const viam::app::v1::DeleteFirebaseConfigRequest* request,
                                      viam::app::v1::DeleteFirebaseConfigResponse* response) override;

    // Mock data storage for verification
    std::string mock_upload_app_id;
    std::string mock_upload_device_token;
    std::string mock_upload_device_uuid;

    std::string mock_delete_app_id;
    std::string mock_delete_device_uuid;

    std::string mock_get_app_id;
    std::vector<std::string> mock_device_tokens_response;

    std::string mock_set_firebase_org_id;
    std::string mock_set_firebase_app_id;
    std::string mock_set_firebase_config_json;

    std::string mock_get_firebase_org_id;
    std::string mock_get_firebase_app_id_response;

    std::string mock_delete_firebase_org_id;
    std::string mock_delete_firebase_app_id;

    /**
     * @brief Get a shared pointer to a MockAppService instance.
     * @return A shared pointer to MockAppService.
     */
    static std::shared_ptr<MockAppService> get_mock_app();
};

}  // namespace app
}  // namespace sdktests
}  // namespace viam
