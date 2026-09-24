#include <viam/sdk/services/private/ml_training_client.hpp>

#include <grpcpp/channel.h>

#include <viam/api/app/mltraining/v1/ml_training.grpc.pb.h>
#include <viam/api/app/mltraining/v1/ml_training.pb.h>
#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/exception.hpp>

namespace viam {
namespace sdk {
namespace impl {

using sdk::from_proto;
using sdk::to_proto;

MLTrainingServiceClient::MLTrainingServiceClient(std::string name, const ViamChannel& channel)
    : MLTrainingService(std::move(name)),
      stub_(viam::app::mltraining::v1::MLTrainingService::NewStub(channel.channel())),
      channel_(&channel) {}

std::unordered_map<std::string, MLTrainingService::Container>
MLTrainingServiceClient::list_supported_containers(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::ListSupportedContainers)
        .with(extra)
        .invoke([](auto& response) {
            std::unordered_map<std::string, MLTrainingService::Container> result;
            for (const auto& pair : response.container_map()) {
                result.emplace(pair.first, from_proto(pair.second));
            }
            return result;
        });
}

std::vector<MLTrainingService::Container>
MLTrainingServiceClient::list_containers(const std::string& organization_id,
                                         const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::ListContainers)
        .with(extra, [&](auto& request) { request.set_organization_id(organization_id); })
        .invoke([](auto& response) {
            std::vector<MLTrainingService::Container> result;
            for (const auto& container_proto : response.containers()) {
                result.push_back(from_proto(container_proto));
            }
            return result;
        });
}

std::string MLTrainingServiceClient::register_custom_training_container(
    const std::string& organization_id, const std::string& image_uri, const std::string& description,
    const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::RegisterCustomTrainingContainer)
        .with(extra, [&](auto& request) {
            request.set_organization_id(organization_id);
            request.set_image_uri(image_uri);
            request.set_description(description);
        })
        .invoke([](auto& response) { return response.id(); });
}

void MLTrainingServiceClient::delete_custom_training_container(const std::string& id,
                                                               const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::DeleteCustomTrainingContainer)
        .with(extra, [&](auto& request) { request.set_id(id); })
        .invoke();
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam