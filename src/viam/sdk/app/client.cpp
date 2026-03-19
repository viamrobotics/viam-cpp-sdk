#include <viam/sdk/app/client.hpp>

#include <boost/optional.hpp>
#include <google/protobuf/timestamp.pb.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/proto_utils.hpp>
#include <viam/sdk/common/utils.hpp>

#include "app/v1/app.grpc.pb.h"
#include "app/v1/app.pb.h"

using namespace viam::sdk;
using namespace google::protobuf;

// Helper to create a gRPC client stub and manage its lifecycle.
template <typename RequestType, typename ResponseType, typename StubType>
class ClientHelper {
   public:
    ClientHelper(AppClient* client, StubType& stub,
                 std::function<grpc::Status(StubType*, grpc::ClientContext*, const RequestType&,
                                            ResponseType*)>
                     method)
        : client_(client), stub_(stub), method_(std::move(method)) {}

    ClientHelper& with(const AttributeMap& extra,
                       std::function<void(RequestType&)> request_modifier) {
        request_modifier_ = std::move(request_modifier);
        extra_ = extra;
        return *this;
    }

    template <typename ResponseHandler>
    auto invoke(ResponseHandler handler) {
        RequestType request;
        grpc::ClientContext context;

        // Set deadline from extra attributes if provided
        if (extra_.count("timeout")) {
            auto timeout_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::duration<double>(extra_["timeout"].as<double>()));
            context.set_deadline(
                std::chrono::system_clock::now() + timeout_ms);
        }

        if (request_modifier_) {
            request_modifier_(request);
        }

        ResponseType response;
        const auto status = method_(&stub_, &context, request, &response);

        if (!status.ok()) {
            throw std::runtime_error("gRPC call failed: " + status.error_message());
        }

        return handler(response);
    }

   private:
    AppClient* client_;
    StubType& stub_;
    std::function<grpc::Status(StubType*, grpc::ClientContext*, const RequestType&,
                               ResponseType*)>
        method_;
    std::function<void(RequestType&)> request_modifier_;
    AttributeMap extra_;
};

template <typename StubType>
auto make_client_helper(AppClient* client, StubType& stub,
                        std::function<grpc::Status(StubType*, grpc::ClientContext*,
                                                   const typename StubType::RequestType&,
                                                   typename StubType::ResponseType*)>
                            method) {
    return ClientHelper<typename StubType::RequestType, typename StubType::ResponseType,
                        StubType>(client, stub, std::move(method));
}

void AppClient::CreateRobotPart(const std::string& robot_id, const std::string& name,
                                const AttributeMap& extra) {
    auto& stub = stub_;
    return make_client_helper(this, *stub, &StubType::CreateRobotPart)
        .with(extra, [&](viam::app::v1::CreateRobotPartRequest& request) {
            request.set_robot_id(robot_id);
            request.set_name(name);
        })
        .invoke([](const viam::app::v1::CreateRobotPartResponse& response) {});
}

void AppClient::DeleteRobotPart(const std::string& id, const AttributeMap& extra) {
    auto& stub = stub_;
    return make_client_helper(this, *stub, &StubType::DeleteRobotPart)
        .with(extra, [&](viam::app::v1::DeleteRobotPartRequest& request) {
            request.set_id(id);
        })
        .invoke([](const viam::app::v1::DeleteRobotPartResponse& response) {});
}

void AppClient::UpdateRobotPart(const std::string& id,
                               const boost::optional<std::string>& name,
                               const boost::optional<ProtoStruct>& robot_config,
                               const boost::optional<std::string>& robot_config_json,
                               const boost::optional<google::protobuf::Timestamp>& last_known_update,
                               const AttributeMap& extra) {
    auto& stub = stub_;
    return make_client_helper(this, *stub, &StubType::UpdateRobotPart)
        .with(extra, [&](viam::app::v1::UpdateRobotPartRequest& request) {
            request.set_id(id);
            if (name) {
                request.set_name(*name);
            }
            if (robot_config) {
                *request.mutable_robot_config() = to_proto(*robot_config);
            }
            if (robot_config_json) {
                request.set_robot_config_json(*robot_config_json);
            }
            if (last_known_update) {
                *request.mutable_last_known_update() = to_proto(*last_known_update);
            }
        })
        .invoke([](const viam::app::v1::UpdateRobotPartResponse& response) {});
}

std::vector<RobotPart> AppClient::ListRobotParts(const std::string& robot_id,
                                                 const AttributeMap& extra) {
    auto& stub = stub_;
    std::vector<RobotPart> robot_parts;
    make_client_helper(this, *stub, &StubType::ListRobotParts)
        .with(extra, [&](viam::app::v1::ListRobotPartsRequest& request) {
            request.set_robot_id(robot_id);
        })
        .invoke([&](const viam::app::v1::ListRobotPartsResponse& response) {
            for (const auto& part : response.parts()) {
                robot_parts.emplace_back(part);
            }
        });
    return robot_parts;
}

std::vector<Robot> AppClient::ListRobots(const AttributeMap& extra) {
    auto& stub = stub_;
    std::vector<Robot> robots;
    make_client_helper(this, *stub, &StubType::ListRobots)
        .with(extra, [](viam::app::v1::ListRobotsRequest& request) {})
        .invoke([&](const viam::app::v1::ListRobotsResponse& response) {
            for (const auto& robot : response.robots()) {
                robots.emplace_back(robot);
            }
        });
    return robots;
}

void AppClient::DeleteRobot(const std::string& id, const AttributeMap& extra) {
    auto& stub = stub_;
    return make_client_helper(this, *stub, &StubType::DeleteRobot)
        .with(extra, [&](viam::app::v1::DeleteRobotRequest& request) {
            request.set_id(id);
        })
        .invoke([](const viam::app::v1::DeleteRobotResponse& response) {});
}

void AppClient::CreateRobot(const std::string& name, const AttributeMap& extra) {
    auto& stub = stub_;
    return make_client_helper(this, *stub, &StubType::CreateRobot)
        .with(extra, [&](viam::app::v1::CreateRobotRequest& request) {
            request.set_name(name);
        })
        .invoke([](const viam::app::v1::CreateRobotResponse& response) {});
}

void AppClient::UpdateRobot(const std::string& id, const std::string& name,
                            const AttributeMap& extra) {
    auto& stub = stub_;
    return make_client_helper(this, *stub, &StubType::UpdateRobot)
        .with(extra, [&](viam::app::v1::UpdateRobotRequest& request) {
            request.set_id(id);
            request.set_name(name);
        })
        .invoke([](const viam::app::v1::UpdateRobotResponse& response) {});
}

AppClient::AppClient(const std::string& address, const std::string& auth_token,
                   const std::string& auth_namespace)
    : address_(address),
      auth_token_(auth_token),
      auth_namespace_(auth_namespace),
      channel_(grpc::CreateChannel(address_, grpc::InsecureChannelCredentials())),
      stub_(viam::app::v1::App::NewStub(channel_)) {}

AppClient::~AppClient() {}
