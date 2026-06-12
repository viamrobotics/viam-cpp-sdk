#pragma once

#include <viam/sdk/app/app.hpp>
#include <viam/sdk/common/proto_convert.hpp>
#include <viam/api/app/v1/app.pb.h>
#include <google/protobuf/timestamp.pb.h>

namespace viam {
namespace sdk {
namespace proto_convert_details {

// DeprecatedStatus
template <>
struct to_proto_impl<DeprecatedStatus> {
    void operator()(const DeprecatedStatus& sdk_type, viam::app::v1::DeprecatedStatus* proto_type) const;
};
template <>
struct from_proto_impl<viam::app::v1::DeprecatedStatus> {
    DeprecatedStatus operator()(const viam::app::v1::DeprecatedStatus* proto_type) const;
};

// DeprecateRegistryItemRequest
template <>
struct to_proto_impl<DeprecateRegistryItemRequest> {
    void operator()(const DeprecateRegistryItemRequest& sdk_type, viam::app::v1::DeprecateRegistryItemRequest* proto_type) const;
};
template <>
struct from_proto_impl<viam::app::v1::DeprecateRegistryItemRequest> {
    DeprecateRegistryItemRequest operator()(const viam::app::v1::DeprecateRegistryItemRequest* proto_type) const;
};

// DeprecateRegistryItemResponse
template <>
struct to_proto_impl<DeprecateRegistryItemResponse> {
    void operator()(const DeprecateRegistryItemResponse& sdk_type, viam::app::v1::DeprecateRegistryItemResponse* proto_type) const;
};
template <>
struct from_proto_impl<viam::app::v1::DeprecateRegistryItemResponse> {
    DeprecateRegistryItemResponse operator()(const viam::app::v1::DeprecateRegistryItemResponse* proto_type) const;
};

// UndeprecateRegistryItemRequest
template <>
struct to_proto_impl<UndeprecateRegistryItemRequest> {
    void operator()(const UndeprecateRegistryItemRequest& sdk_type, viam::app::v1::UndeprecateRegistryItemRequest* proto_type) const;
};
template <>
struct from_proto_impl<viam::app::v1::UndeprecateRegistryItemRequest> {
    UndeprecateRegistryItemRequest operator()(const viam::app::v1::UndeprecateRegistryItemRequest* proto_type) const;
};

// UndeprecateRegistryItemResponse
template <>
struct to_proto_impl<UndeprecateRegistryItemResponse> {
    void operator()(const UndeprecateRegistryItemResponse& sdk_type, viam::app::v1::UndeprecateRegistryItemResponse* proto_type) const;
};
template <>
struct from_proto_impl<viam::app::v1::UndeprecateRegistryItemResponse> {
    UndeprecateRegistryItemResponse operator()(const viam::app::v1::UndeprecateRegistryItemResponse* proto_type) const;
};

// DeprecateRegistryItemVersionRequest
template <>
struct to_proto_impl<DeprecateRegistryItemVersionRequest> {
    void operator()(const DeprecateRegistryItemVersionRequest& sdk_type, viam::app::v1::DeprecateRegistryItemVersionRequest* proto_type) const;
};
template <>
struct from_proto_impl<viam::app::v1::DeprecateRegistryItemVersionRequest> {
    DeprecateRegistryItemVersionRequest operator()(const viam::app::v1::DeprecateRegistryItemVersionRequest* proto_type) const;
};

// DeprecateRegistryItemVersionResponse
template <>
struct to_proto_impl<DeprecateRegistryItemVersionResponse> {
    void operator()(const DeprecateRegistryItemVersionResponse& sdk_type, viam::app::v1::DeprecateRegistryItemVersionResponse* proto_type) const;
};
template <>
struct from_proto_impl<viam::app::v1::DeprecateRegistryItemVersionResponse> {
    DeprecateRegistryItemVersionResponse operator()(const viam::app::v1::DeprecateRegistryItemVersionResponse* proto_type) const;
};

// UndeprecateRegistryItemVersionRequest
template <>
struct to_proto_impl<UndeprecateRegistryItemVersionRequest> {
    void operator()(const UndeprecateRegistryItemVersionRequest& sdk_type, viam::app::v1::UndeprecateRegistryItemVersionRequest* proto_type) const;
};
template <>
struct from_proto_impl<viam::app::v1::UndeprecateRegistryItemVersionRequest> {
    UndeprecateRegistryItemVersionRequest operator()(const viam::app::v1::UndeprecateRegistryItemVersionRequest* proto_type) const;
};

// UndeprecateRegistryItemVersionResponse
template <>
struct to_proto_impl<UndeprecateRegistryItemVersionResponse> {
    void operator()(const UndeprecateRegistryItemVersionResponse& sdk_type, viam::app::v1::UndeprecateRegistryItemVersionResponse* proto_type) const;
};
template <>
struct from_proto_impl<viam::app::v1::UndeprecateRegistryItemVersionResponse> {
    UndeprecateRegistryItemVersionResponse operator()(const viam::app::v1::UndeprecateRegistryItemVersionResponse* proto_type) const;
};

// ModuleVersion (add deprecated_status conversion)
template <>
struct to_proto_impl<ModuleVersion> {
    void operator()(const ModuleVersion& sdk_type, viam::app::v1::ModuleVersion* proto_type) const;
};
template <>
struct from_proto_impl<viam::app::v1::ModuleVersion> {
    ModuleVersion operator()(const viam::app::v1::ModuleVersion* proto_type) const;
};

// RegistryItem (add deprecated_status conversion)
template <>
struct to_proto_impl<RegistryItem> {
    void operator()(const RegistryItem& sdk_type, viam::app::v1::RegistryItem* proto_type) const;
};
template <>
struct from_proto_impl<viam::app::v1::RegistryItem> {
    RegistryItem operator()(const viam::app::v1::RegistryItem* proto_type) const;
};

// Module (add deprecated_status conversion)
template <>
struct to_proto_impl<Module> {
    void operator()(const Module& sdk_type, viam::app::v1::Module* proto_type) const;
};
template <>
struct from_proto_impl<viam::app::v1::Module> {
    Module operator()(const viam::app::v1::Module* proto_type) const;
};

// VersionHistory (add deprecated_status conversion)
template <>
struct to_proto_impl<VersionHistory> {
    void operator()(const VersionHistory& sdk_type, viam::app::v1::VersionHistory* proto_type) const;
};
template <>
struct from_proto_impl<viam::app::v1::VersionHistory> {
    VersionHistory operator()(const viam::app::v1::VersionHistory* proto_type) const;
};

}  // namespace proto_convert_details
}  // namespace sdk
}  // namespace viam
