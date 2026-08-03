#include <viam/sdk/common/exceptions.hpp>
#include <viam/sdk/common/proto_utils.hpp>
#include <viam/sdk/common/utils.hpp>

#include <google/protobuf/struct.pb.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include <viam/app/v1/app.grpc.pb.h>
#include <viam/app/v1/app.pb.h>

namespace viam {
namespace sdk {

using AttributeMap = google::protobuf::Struct;
using AttributeValue = google::protobuf::Value;

class AppClient {
   public:
    AppClient(std::string address, std::string auth_token) {
        auto creds = grpc::CreateChannelCredentials();
        auto channel = grpc::CreateChannel(address, creds);
        stub_ = viam::app::v1::App::NewStub(channel);
    }

    ~AppClient() = default;

    // Get a list of logs for a specific robot part.
    //
    // Args:
    //     name: The name of the robot part.
    //     limit: The maximum number of logs to return.
    //     offset: The number of logs to skip.
    //     range: An optional string specifying a time range for logs (e.g., "1h", "30m").
    //     order: An optional LogOrder enum value to specify the order of logs (ascending or descending).
    //     extra: Extra arguments to pass to the server.
    //
    // Returns:
    //     A vector of Log objects.
    std::vector<viam::app::v1::Log> GetRobotPartLogs(
        const std::string& name,
        std::optional<int64_t> limit = std::nullopt,
        std::optional<int64_t> offset = std::nullopt,
        std::optional<std::string> range = std::nullopt,
        std::optional<viam::app::v1::LogOrder> order = std::nullopt,
        const AttributeMap& extra = {}) {
        viam::app::v1::GetRobotPartLogsRequest request;
        request.set_name(name);
        if (limit.has_value()) {
            request.set_limit(limit.value());
        }
        if (offset.has_value()) {
            request.set_offset(offset.value());
        }
        if (range.has_value()) {
            request.set_range(range.value());
        }
        if (order.has_value()) {
            request.set_order(order.value());
        }
        for (const auto& pair : extra) {
            auto& value = *request.mutable_extra()->Add();
            value.set_key(pair.first);
            *value.mutable_value() = pair.second;
        }

        grpc::ClientContext context;
        viam::app::v1::GetRobotPartLogsResponse response;
        const grpc::Status status = stub_->GetRobotPartLogs(&context, request, &response);

        if (!status.ok()) {
            throw std::runtime_error(status.error_message());
        }

        return std::vector<viam::app::v1::Log>(response.logs().begin(), response.logs().end());
    }

    // Create a new robot part.
    //
    // Args:
    //     name: The name of the robot part.
    //     extra: Extra arguments to pass to the server.
    //
    // Returns:
    //     A CreateRobotPartResponse object.
    viam::app::v1::CreateRobotPartResponse CreateRobotPart(const std::string& name,
                                                           const AttributeMap& extra = {}) {
        viam::app::v1::CreateRobotPartRequest request;
        request.set_name(name);
        for (const auto& pair : extra) {
            auto& value = *request.mutable_extra()->Add();
            value.set_key(pair.first);
            *value.mutable_value() = pair.second;
        }

        grpc::ClientContext context;
        viam::app::v1::CreateRobotPartResponse response;
        const grpc::Status status = stub_->CreateRobotPart(&context, request, &response);

        if (!status.ok()) {
            throw std::runtime_error(status.error_message());
        }

        return response;
    }

    // Delete a robot part.
    //
    // Args:
    //     name: The name of the robot part.
    //     extra: Extra arguments to pass to the server.
    //
    // Returns:
    //     A DeleteRobotPartResponse object.
    viam::app::v1::DeleteRobotPartResponse DeleteRobotPart(const std::string& name,
                                                           const AttributeMap& extra = {}) {
        viam::app::v1::DeleteRobotPartRequest request;
        request.set_name(name);
        for (const auto& pair : extra) {
            auto& value = *request.mutable_extra()->Add();
            value.set_key(pair.first);
            *value.mutable_value() = pair.second;
        }

        grpc::ClientContext context;
        viam::app::v1::DeleteRobotPartResponse response;
        const grpc::Status status = stub_->DeleteRobotPart(&context, request, &response);

        if (!status.ok()) {
            throw std::runtime_error(status.error_message());
        }

        return response;
    }

   private:
    std::unique_ptr<viam::app::v1::App::Stub> stub_;
};

}  // namespace sdk
}  // namespace viam
