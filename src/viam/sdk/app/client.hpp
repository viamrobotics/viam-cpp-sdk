#include <viam/sdk/app/app.hpp>
#include <viam/sdk/common/utils.hpp>

#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include "viam/sdk/app/app.pb.h"
#include "viam/sdk/app/app.grpc.pb.h"

namespace viam {
namespace sdk {

AppClient::AppClient(std::shared_ptr<grpc::Channel> channel)
    : stub_(AppService::NewStub(channel)) {}

AppClient::AppClient(const std::string& host, const std::string& port)
    : AppClient(grpc::CreateChannel(host + ":" + port, grpc::InsecureChannelCredentials())) {}

AppClient::AppClient(const std::string& address)
    : AppClient(address.substr(0, address.find(":")), address.substr(address.find(":") + 1)) {}

AppClient::~AppClient() {}

CreateComponentResponse AppClient::CreateComponent(const CreateComponentRequest& request,
                                                 const AttributeMap& extra) {
    grpc::ClientContext context;
    // TODO: Add extra attributes to context.
    CreateComponentResponse response;
    grpc::Status status = stub_->CreateComponent(&context, request, &response);

    if (!status.ok()) {
        throw std::runtime_error("Failed to create component: " + status.error_message());
    }
    return response;
}

DeleteComponentResponse AppClient::DeleteComponent(const DeleteComponentRequest& request,
                                                 const AttributeMap& extra) {
    grpc::ClientContext context;
    // TODO: Add extra attributes to context.
    DeleteComponentResponse response;
    grpc::Status status = stub_->DeleteComponent(&context, request, &response);

    if (!status.ok()) {
        throw std::runtime_error("Failed to delete component: " + status.error_message());
    }
    return response;
}

GetComponentResponse AppClient::GetComponent(const GetComponentRequest& request,
                                             const AttributeMap& extra) {
    grpc::ClientContext context;
    // TODO: Add extra attributes to context.
    GetComponentResponse response;
    grpc::Status status = stub_->GetComponent(&context, request, &response);

    if (!status.ok()) {
        throw std::runtime_error("Failed to get component: " + status.error_message());
    }
    return response;
}

ListComponentsResponse AppClient::ListComponents(const ListComponentsRequest& request,
                                                 const AttributeMap& extra) {
    grpc::ClientContext context;
    // TODO: Add extra attributes to context.
    ListComponentsResponse response;
    grpc::Status status = stub_->ListComponents(&context, request, &response);

    if (!status.ok()) {
        throw std::runtime_error("Failed to list components: " + status.error_message());
    }
    return response;
}

UpdateComponentResponse AppClient::UpdateComponent(const UpdateComponentRequest& request,
                                                   const AttributeMap& extra) {
    grpc::ClientContext context;
    // TODO: Add extra attributes to context.
    UpdateComponentResponse response;
    grpc::Status status = stub_->UpdateComponent(&context, request, &response);

    if (!status.ok()) {
        throw std::runtime_error("Failed to update component: " + status.error_message());
    }
    return response;
}

CreateFragmentResponse AppClient::CreateFragment(const CreateFragmentRequest& request,
                                                 const AttributeMap& extra) {
    grpc::ClientContext context;
    // TODO: Add extra attributes to context.
    CreateFragmentResponse response;
    grpc::Status status = stub_->CreateFragment(&context, request, &response);

    if (!status.ok()) {
        throw std::runtime_error("Failed to create fragment: " + status.error_message());
    }
    return response;
}

DeleteFragmentResponse AppClient::DeleteFragment(const DeleteFragmentRequest& request,
                                                 const AttributeMap& extra) {
    grpc::ClientContext context;
    // TODO: Add extra attributes to context.
    DeleteFragmentResponse response;
    grpc::Status status = stub_->DeleteFragment(&context, request, &response);

    if (!status.ok()) {
        throw std::runtime_error("Failed to delete fragment: " + status.error_message());
    }
    return response;
}

GetFragmentResponse AppClient::GetFragment(const GetFragmentRequest& request,
                                           const AttributeMap& extra) {
    grpc::ClientContext context;
    // TODO: Add extra attributes to context.
    GetFragmentResponse response;
    grpc::Status status = stub_->GetFragment(&context, request, &response);

    if (!status.ok()) {
        throw std::runtime_error("Failed to get fragment: " + status.error_message());
    }
    return response;
}

ListFragmentsResponse AppClient::ListFragments(const ListFragmentsRequest& request,
                                               const AttributeMap& extra) {
    grpc::ClientContext context;
    // TODO: Add extra attributes to context.
    ListFragmentsResponse response;
    grpc::Status status = stub_->ListFragments(&context, request, &response);

    if (!status.ok()) {
        throw std::runtime_error("Failed to list fragments: " + status.error_message());
    }
    return response;
}

UpdateFragmentResponse AppClient::UpdateFragment(const UpdateFragmentRequest& request,
                                                 const AttributeMap& extra) {
    grpc::ClientContext context;
    // TODO: Add extra attributes to context.
    UpdateFragmentResponse response;
    grpc::Status status = stub_->UpdateFragment(&context, request, &response);

    if (!status.ok()) {
        throw std::runtime_error("Failed to update fragment: " + status.error_message());
    }
    return response;
}

CreateRegistryItemResponse AppClient::CreateRegistryItem(const CreateRegistryItemRequest& request,
                                                         const AttributeMap& extra) {
    grpc::ClientContext context;
    // TODO: Add extra attributes to context.
    CreateRegistryItemResponse response;
    grpc::Status status = stub_->CreateRegistryItem(&context, request, &response);

    if (!status.ok()) {
        throw std::runtime_error("Failed to create registry item: " + status.error_message());
    }
    return response;
}

DeleteRegistryItemResponse AppClient::DeleteRegistryItem(const DeleteRegistryItemRequest& request,
                                                         const AttributeMap& extra) {
    grpc::ClientContext context;
    // TODO: Add extra attributes to context.
    DeleteRegistryItemResponse response;
    grpc::Status status = stub_->DeleteRegistryItem(&context, request, &response);

    if (!status.ok()) {
        throw std::runtime_error("Failed to delete registry item: " + status.error_message());
    }
    return response;
}

GetRegistryItemResponse AppClient::GetRegistryItem(const GetRegistryItemRequest& request,
                                                   const AttributeMap& extra) {
    grpc::ClientContext context;
    // TODO: Add extra attributes to context.
    GetRegistryItemResponse response;
    grpc::Status status = stub_->GetRegistryItem(&context, request, &response);

    if (!status.ok()) {
        throw std::runtime_error("Failed to get registry item: " + status.error_message());
    }
    return response;
}

ListRegistryItemsResponse AppClient::ListRegistryItems(const ListRegistryItemsRequest& request,
                                                       const AttributeMap& extra) {
    grpc::ClientContext context;
    // TODO: Add extra attributes to context.
    ListRegistryItemsResponse response;
    grpc::Status status = stub_->ListRegistryItems(&context, request, &response);

    if (!status.ok()) {
        throw std::runtime_error("Failed to list registry items: " + status.error_message());
    }
    return response;
}

UpdateRegistryItemResponse AppClient::UpdateRegistryItem(const UpdateRegistryItemRequest& request,
                                                         const AttributeMap& extra) {
    grpc::ClientContext context;
    // TODO: Add extra attributes to context.
    UpdateRegistryItemResponse response;
    grpc::Status status = stub_->UpdateRegistryItem(&context, request, &response);

    if (!status.ok()) {
        throw std::runtime_error("Failed to update registry item: " + status.error_message());
    }
    return response;
}

TransferRegistryItemResponse AppClient::TransferRegistryItem(const TransferRegistryItemRequest& request,
                                                             const AttributeMap& extra) {
    grpc::ClientContext context;
    // TODO: Add extra attributes to context.
    TransferRegistryItemResponse response;
    grpc::Status status = stub_->TransferRegistryItem(&context, request, &response);

    if (!status.ok()) {
        throw std::runtime_error("Failed to transfer registry item: " + status.error_message());
    }
    return response;
}

/// @brief Deprecate a registry item.
/// @param request The request to deprecate a registry item.
/// @param extra Additional arguments for the RPC call.
/// @return The response from deprecating the registry item.
DeprecateRegistryItemResponse AppClient::DeprecateRegistryItem(
    const DeprecateRegistryItemRequest& request, const AttributeMap& extra) {
    grpc::ClientContext context;
    // TODO: Add extra attributes to context.
    DeprecateRegistryItemResponse response;
    grpc::Status status = stub_->DeprecateRegistryItem(&context, request, &response);

    if (!status.ok()) {
        throw std::runtime_error("Failed to deprecate registry item: " + status.error_message());
    }
    return response;
}

/// @brief Undeprecate a registry item.
/// @param request The request to undeprecate a registry item.
/// @param extra Additional arguments for the RPC call.
/// @return The response from undeprecating the registry item.
UndeprecateRegistryItemResponse AppClient::UndeprecateRegistryItem(
    const UndeprecateRegistryItemRequest& request, const AttributeMap& extra) {
    grpc::ClientContext context;
    // TODO: Add extra attributes to context.
    UndeprecateRegistryItemResponse response;
    grpc::Status status = stub_->UndeprecateRegistryItem(&context, request, &response);

    if (!status.ok()) {
        throw std::runtime_error("Failed to undeprecate registry item: " + status.error_message());
    }
    return response;
}

/// @brief Deprecate a specific version of a registry item.
/// @param request The request to deprecate a registry item version.
/// @param extra Additional arguments for the RPC call.
/// @return The response from deprecating the registry item version.
DeprecateRegistryItemVersionResponse AppClient::DeprecateRegistryItemVersion(
    const DeprecateRegistryItemVersionRequest& request, const AttributeMap& extra) {
    grpc::ClientContext context;
    // TODO: Add extra attributes to context.
    DeprecateRegistryItemVersionResponse response;
    grpc::Status status = stub_->DeprecateRegistryItemVersion(&context, request, &response);

    if (!status.ok()) {
        throw std::runtime_error("Failed to deprecate registry item version: " + status.error_message());
    }
    return response;
}

/// @brief Undeprecate a specific version of a registry item.
/// @param request The request to undeprecate a registry item version.
/// @param extra Additional arguments for the RPC call.
/// @return The response from undeprecating the registry item version.
UndeprecateRegistryItemVersionResponse AppClient::UndeprecateRegistryItemVersion(
    const UndeprecateRegistryItemVersionRequest& request, const AttributeMap& extra) {
    grpc::ClientContext context;
    // TODO: Add extra attributes to context.
    UndeprecateRegistryItemVersionResponse response;
    grpc::Status status = stub_->UndeprecateRegistryItemVersion(&context, request, &response);

    if (!status.ok()) {
        throw std::runtime_error("Failed to undeprecate registry item version: " + status.error_message());
    }
    return response;
}

}  // namespace sdk
}  // namespace viam
