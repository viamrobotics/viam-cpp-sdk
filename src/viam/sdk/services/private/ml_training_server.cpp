#include <viam/sdk/services/private/ml_training_server.hpp>

#include <viam/api/app/mltraining/v1/ml_training.pb.h>
#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/private/service_helper.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/common/utils.hpp>

namespace viam {
namespace sdk {
namespace impl {

using sdk::from_proto;
using sdk::to_proto;

MLTrainingServiceServer::MLTrainingServiceServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(std::move(manager)) {}

::grpc::Status MLTrainingServiceServer::ListSupportedContainers(
    ::grpc::ServerContext* context,
    const ::viam::app::mltraining::v1::ListSupportedContainersRequest* request,
    ::viam::app::mltraining::v1::ListSupportedContainersResponse* response) noexcept {
    return make_service_helper<MLTrainingService>(
        "MLTrainingServiceServer::ListSupportedContainers", this, context, request)(
        [&](auto& helper, auto& service) {
            const auto result = service->list_supported_containers(helper.getExtra());
            for (const auto& pair : result) {
                (*response->mutable_container_map())[pair.first] = to_proto(pair.second);
            }
        });
}

::grpc::Status MLTrainingServiceServer::ListContainers(
    ::grpc::ServerContext* context,
    const ::viam::app::mltraining::v1::ListContainersRequest* request,
    ::viam::app::mltraining::v1::ListContainersResponse* response) noexcept {
    return make_service_helper<MLTrainingService>(
        "MLTrainingServiceServer::ListContainers", this, context, request)(
        [&](auto& helper, auto& service) {
            const auto result = service->list_containers(request->organization_id(), helper.getExtra());
            for (const auto& container : result) {
                *response->add_containers() = to_proto(container);
            }
        });
}

::grpc::Status MLTrainingServiceServer::RegisterCustomTrainingContainer(
    ::grpc::ServerContext* context,
    const ::viam::app::mltraining::v1::RegisterCustomTrainingContainerRequest* request,
    ::viam::app::mltraining::v1::RegisterCustomTrainingContainerResponse* response) noexcept {
    return make_service_helper<MLTrainingService>(
        "MLTrainingServiceServer::RegisterCustomTrainingContainer", this, context, request)(
        [&](auto& helper, auto& service) {
            const std::string id = service->register_custom_training_container(
                request->organization_id(), request->image_uri(), request->description(), helper.getExtra());
            response->set_id(id);
        });
}

::grpc::Status MLTrainingServiceServer::DeleteCustomTrainingContainer(
    ::grpc::ServerContext* context,
    const ::viam::app::mltraining::v1::DeleteCustomTrainingContainerRequest* request,
    ::viam::app::mltraining::v1::DeleteCustomTrainingContainerResponse* response) noexcept {
    return make_service_helper<MLTrainingService>(
        "MLTrainingServiceServer::DeleteCustomTrainingContainer", this, context, request)(
        [&](auto& helper, auto& service) {
            service->delete_custom_training_container(request->id(), helper.getExtra());
        });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam