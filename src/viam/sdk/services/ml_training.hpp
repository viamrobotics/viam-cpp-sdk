/// @file services/ml_training.hpp
///
/// @brief Defines an `MLTrainingService` service.
#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include <boost/optional/optional.hpp>

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/resource/resource_api.hpp>
#include <viam/sdk/services/service.hpp>

namespace viam {
namespace app {
namespace mltraining {
namespace v1 {

class Container;
class ListSupportedContainersResponse;
class ListContainersRequest;
class ListContainersResponse;
class RegisterCustomTrainingContainerRequest;
class RegisterCustomTrainingContainerResponse;
class DeleteCustomTrainingContainerRequest;
class DeleteCustomTrainingContainerResponse;

}  // namespace v1
}  // namespace mltraining
}  // namespace app
}  // namespace viam

namespace viam {
namespace sdk {

/// @class MLTrainingService ml_training.hpp "services/ml_training.hpp"
/// @brief The `MLTrainingService` service provides methods to manage ML training jobs.
/// @ingroup MLTrainingService
///
/// This acts as an abstract base class to be inherited from by any drivers representing
/// specific MLTrainingService implementations. This class cannot be used on its own.
class MLTrainingService : public Service {
   public:
    /// @struct Container
    /// @brief Represents a container used for ML training.
    struct Container {
        std::string id;
        std::string organization_id;
        std::string image_uri;
        std::string description;
        std::string visibility;

        friend bool operator==(const Container& lhs, const Container& rhs);
    };

    /// @struct ListContainersResponse
    /// @brief Response for listing containers.
    struct ListContainersResponse {
        std::vector<Container> containers;
    };

    /// @struct RegisterCustomTrainingContainerResponse
    /// @brief Response for registering a custom training container.
    struct RegisterCustomTrainingContainerResponse {
        std::string id;
    };

    /// @struct DeleteCustomTrainingContainerResponse
    /// @brief Response for deleting a custom training container.
    struct DeleteCustomTrainingContainerResponse {};

    API api() const override;

    /// @brief ListSupportedContainers gets all the Viam-supported containers available for a custom training job.
    /// @return A map of container names to Container objects.
    inline std::unordered_map<std::string, Container> list_supported_containers() {
        return list_supported_containers({});
    }

    /// @brief ListSupportedContainers gets all the Viam-supported containers available for a custom training job.
    /// @param extra Any additional arguments to the method.
    /// @return A map of container names to Container objects.
    virtual std::unordered_map<std::string, Container> list_supported_containers(
        const ProtoStruct& extra) = 0;

    /// @brief ListContainers lists the containers available for a custom training job for a given organization.
    /// @param organization_id The ID of the organization.
    /// @return A list of Container objects.
    inline std::vector<Container> list_containers(const std::string& organization_id) {
        return list_containers(organization_id, {});
    }

    /// @brief ListContainers lists the containers available for a custom training job for a given organization.
    /// @param organization_id The ID of the organization.
    /// @param extra Any additional arguments to the method.
    /// @return A list of Container objects.
    virtual std::vector<Container> list_containers(const std::string& organization_id,
                                                   const ProtoStruct& extra) = 0;

    /// @brief RegisterCustomTrainingContainer registers a custom container in the database for custom training jobs.
    /// @param organization_id The ID of the organization.
    /// @param image_uri The URI of the container image.
    /// @param description A description of the container.
    /// @return The ID of the registered container.
    inline std::string register_custom_training_container(const std::string& organization_id,
                                                          const std::string& image_uri,
                                                          const std::string& description) {
        return register_custom_training_container(organization_id, image_uri, description, {});
    }

    /// @brief RegisterCustomTrainingContainer registers a custom container in the database for custom training jobs.
    /// @param organization_id The ID of the organization.
    /// @param image_uri The URI of the container image.
    /// @param description A description of the container.
    /// @param extra Any additional arguments to the method.
    /// @return The ID of the registered container.
    virtual std::string register_custom_training_container(const std::string& organization_id,
                                                           const std::string& image_uri,
                                                           const std::string& description,
                                                           const ProtoStruct& extra) = 0;

    /// @brief DeleteCustomTrainingContainer deletes a custom container from the database.
    /// @param id The ID of the container to delete.
    inline void delete_custom_training_container(const std::string& id) {
        return delete_custom_training_container(id, {});
    }

    /// @brief DeleteCustomTrainingContainer deletes a custom container from the database.
    /// @param id The ID of the container to delete.
    /// @param extra Any additional arguments to the method.
    virtual void delete_custom_training_container(const std::string& id,
                                                  const ProtoStruct& extra) = 0;

   protected:
    explicit MLTrainingService(std::string name);
};

template <>
struct API::traits<MLTrainingService> {
    static API api();
};

bool operator==(const MLTrainingService::Container& lhs, const MLTrainingService::Container& rhs);

namespace proto_convert_details {

template <>
struct to_proto_impl<MLTrainingService::Container> {
    void operator()(const MLTrainingService::Container&,
                    viam::app::mltraining::v1::Container*) const;
};

template <>
struct from_proto_impl<viam::app::mltraining::v1::Container> {
    MLTrainingService::Container operator()(const viam::app::mltraining::v1::Container*) const;
};

template <>
struct to_proto_impl<MLTrainingService::ListContainersResponse> {
    void operator()(const MLTrainingService::ListContainersResponse&,
                    viam::app::mltraining::v1::ListContainersResponse*) const;
};

template <>
struct from_proto_impl<viam::app::mltraining::v1::ListContainersResponse> {
    MLTrainingService::ListContainersResponse operator()(
        const viam::app::mltraining::v1::ListContainersResponse*) const;
};

template <>
struct to_proto_impl<MLTrainingService::RegisterCustomTrainingContainerResponse> {
    void operator()(const MLTrainingService::RegisterCustomTrainingContainerResponse&,
                    viam::app::mltraining::v1::RegisterCustomTrainingContainerResponse*) const;
};

template <>
struct from_proto_impl<viam::app::mltraining::v1::RegisterCustomTrainingContainerResponse> {
    MLTrainingService::RegisterCustomTrainingContainerResponse operator()(
        const viam::app::mltraining::v1::RegisterCustomTrainingContainerResponse*) const;
};

template <>
struct to_proto_impl<MLTrainingService::DeleteCustomTrainingContainerResponse> {
    void operator()(const MLTrainingService::DeleteCustomTrainingContainerResponse&,
                    viam::app::mltraining::v1::DeleteCustomTrainingContainerResponse*) const;
};

template <>
struct from_proto_impl<viam::app::mltraining::v1::DeleteCustomTrainingContainerResponse> {
    MLTrainingService::DeleteCustomTrainingContainerResponse operator()(
        const viam::app::mltraining::v1::DeleteCustomTrainingContainerResponse*) const;
};

}  // namespace proto_convert_details

}  // namespace sdk
}  // namespace viam