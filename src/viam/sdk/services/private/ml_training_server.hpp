/// @file services/private/ml_training_server.hpp
///
/// @brief Implements a gRPC server for the `MLTrainingService` service.
#pragma once

#include <memory>

#include <viam/api/app/mltraining/v1/ml_training.grpc.pb.h>

#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/resource/resource_server_base.hpp>
#include <viam/sdk/rpc/server.hpp>
#include <viam/sdk/services/ml_training.hpp>

namespace viam {
namespace sdk {
namespace impl {

/// @class MLTrainingServiceServer
/// @brief gRPC server implementation of an `MLTrainingService` service.
/// @ingroup MLTrainingService
class MLTrainingServiceServer : public ResourceServer,
                                public viam::app::mltraining::v1::MLTrainingService::Service {
   public:
    using interface_type = MLTrainingService;
    using service_type = viam::app::mltraining::v1::MLTrainingService;
    explicit MLTrainingServiceServer(std::shared_ptr<ResourceManager> manager);

    ::grpc::Status ListSupportedContainers(
        ::grpc::ServerContext* context,
        const ::viam::app::mltraining::v1::ListSupportedContainersRequest* request,
        ::viam::app::mltraining::v1::ListSupportedContainersResponse* response) noexcept override;

    ::grpc::Status ListContainers(
        ::grpc::ServerContext* context,
        const ::viam::app::mltraining::v1::ListContainersRequest* request,
        ::viam::app::mltraining::v1::ListContainersResponse* response) noexcept override;

    ::grpc::Status RegisterCustomTrainingContainer(
        ::grpc::ServerContext* context,
        const ::viam::app::mltraining::v1::RegisterCustomTrainingContainerRequest* request,
        ::viam::app::mltraining::v1::RegisterCustomTrainingContainerResponse* response)
        noexcept override;

    ::grpc::Status DeleteCustomTrainingContainer(
        ::grpc::ServerContext* context,
        const ::viam::app::mltraining::v1::DeleteCustomTrainingContainerRequest* request,
        ::viam::app::mltraining::v1::DeleteCustomTrainingContainerResponse* response)
        noexcept override;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam