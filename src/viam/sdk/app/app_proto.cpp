#include <viam/sdk/app/app_proto.hpp>
#include <viam/sdk/common/utils.hpp> // For AttributeMap and Timestamp conversions

namespace viam {
namespace sdk {
namespace proto_convert_details {

// DeprecatedStatus
void to_proto_impl<DeprecatedStatus>::operator()(const DeprecatedStatus& sdk_type, viam::app::v1::DeprecatedStatus* proto_type) const {
    if (sdk_type.deprecated_at) {
        *proto_type->mutable_deprecated_at() = to_proto(*sdk_type.deprecated_at);
    }
    proto_type->set_message(sdk_type.message);
}
DeprecatedStatus from_proto_impl<viam::app::v1::DeprecatedStatus>::operator()(const viam::app::v1::DeprecatedStatus* proto_type) const {
    DeprecatedStatus sdk_type;
    if (proto_type->has_deprecated_at()) {
        sdk_type.deprecated_at = from_proto(proto_type->deprecated_at());
    }
    sdk_type.message = proto_type->message();
    return sdk_type;
}

// DeprecateRegistryItemRequest
void to_proto_impl<DeprecateRegistryItemRequest>::operator()(const DeprecateRegistryItemRequest& sdk_type, viam::app::v1::DeprecateRegistryItemRequest* proto_type) const {
    proto_type->set_item_id(sdk_type.item_id);
    proto_type->set_message(sdk_type.message);
}
DeprecateRegistryItemRequest from_proto_impl<viam::app::v1::DeprecateRegistryItemRequest>::operator()(const viam::app::v1::DeprecateRegistryItemRequest* proto_type) const {
    DeprecateRegistryItemRequest sdk_type;
    sdk_type.item_id = proto_type->item_id();
    sdk_type.message = proto_type->message();
    return sdk_type;
}

// DeprecateRegistryItemResponse
void to_proto_impl<DeprecateRegistryItemResponse>::operator()(const DeprecateRegistryItemResponse& sdk_type, viam::app::v1::DeprecateRegistryItemResponse* proto_type) const {
    // Empty response, no fields to convert
}
DeprecateRegistryItemResponse from_proto_impl<viam::app::v1::DeprecateRegistryItemResponse>::operator()(const viam::app::v1::DeprecateRegistryItemResponse* proto_type) const {
    // Empty response, no fields to convert
    return {};
}

// UndeprecateRegistryItemRequest
void to_proto_impl<UndeprecateRegistryItemRequest>::operator()(const UndeprecateRegistryItemRequest& sdk_type, viam::app::v1::UndeprecateRegistryItemRequest* proto_type) const {
    proto_type->set_item_id(sdk_type.item_id);
}
UndeprecateRegistryItemRequest from_proto_impl<viam::app::v1::UndeprecateRegistryItemRequest>::operator()(const viam::app::v1::UndeprecateRegistryItemRequest* proto_type) const {
    UndeprecateRegistryItemRequest sdk_type;
    sdk_type.item_id = proto_type->item_id();
    return sdk_type;
}

// UndeprecateRegistryItemResponse
void to_proto_impl<UndeprecateRegistryItemResponse>::operator()(const UndeprecateRegistryItemResponse& sdk_type, viam::app::v1::UndeprecateRegistryItemResponse* proto_type) const {
    // Empty response, no fields to convert
}
UndeprecateRegistryItemResponse from_proto_impl<viam::app::v1::UndeprecateRegistryItemResponse>::operator()(const viam::app::v1::UndeprecateRegistryItemResponse* proto_type) const {
    // Empty response, no fields to convert
    return {};
}

// DeprecateRegistryItemVersionRequest
void to_proto_impl<DeprecateRegistryItemVersionRequest>::operator()(const DeprecateRegistryItemVersionRequest& sdk_type, viam::app::v1::DeprecateRegistryItemVersionRequest* proto_type) const {
    proto_type->set_item_id(sdk_type.item_id);
    proto_type->set_version(sdk_type.version);
    proto_type->set_message(sdk_type.message);
}
DeprecateRegistryItemVersionRequest from_proto_impl<viam::app::v1::DeprecateRegistryItemVersionRequest>::operator()(const viam::app::v1::DeprecateRegistryItemVersionRequest* proto_type) const {
    DeprecateRegistryItemVersionRequest sdk_type;
    sdk_type.item_id = proto_type->item_id();
    sdk_type.version = proto_type->version();
    sdk_type.message = proto_type->message();
    return sdk_type;
}

// DeprecateRegistryItemVersionResponse
void to_proto_impl<DeprecateRegistryItemVersionResponse>::operator()(const DeprecateRegistryItemVersionResponse& sdk_type, viam::app::v1::DeprecateRegistryItemVersionResponse* proto_type) const {
    // Empty response, no fields to convert
}
DeprecateRegistryItemVersionResponse from_proto_impl<viam::app::v1::DeprecateRegistryItemVersionResponse>::operator()(const viam::app::v1::DeprecateRegistryItemVersionResponse* proto_type) const {
    // Empty response, no fields to convert
    return {};
}

// UndeprecateRegistryItemVersionRequest
void to_proto_impl<UndeprecateRegistryItemVersionRequest>::operator()(const UndeprecateRegistryItemVersionRequest& sdk_type, viam::app::v1::UndeprecateRegistryItemVersionRequest* proto_type) const {
    proto_type->set_item_id(sdk_type.item_id);
    proto_type->set_version(sdk_type.version);
}
UndeprecateRegistryItemVersionRequest from_proto_impl<viam::app::v1::UndeprecateRegistryItemVersionRequest>::operator()(const viam::app::v1::UndeprecateRegistryItemVersionRequest* proto_type) const {
    UndeprecateRegistryItemVersionRequest sdk_type;
    sdk_type.item_id = proto_type->item_id();
    sdk_type.version = proto_type->version();
    return sdk_type;
}

// UndeprecateRegistryItemVersionResponse
void to_proto_impl<UndeprecateRegistryItemVersionResponse>::operator()(const UndeprecateRegistryItemVersionResponse& sdk_type, viam::app::v1::UndeprecateRegistryItemVersionResponse* proto_type) const {
    // Empty response, no fields to convert
}
UndeprecateRegistryItemVersionResponse from_proto_impl<viam::app::v1::UndeprecateRegistryItemVersionResponse>::operator()(const viam::app::v1::UndeprecateRegistryItemVersionResponse* proto_type) const {
    // Empty response, no fields to convert
    return {};
}

// ModuleVersion (add deprecated_status conversion)
void to_proto_impl<ModuleVersion>::operator()(const ModuleVersion& sdk_type, viam::app::v1::ModuleVersion* proto_type) const {
    // NOTE: This assumes existing conversion logic for other fields.
    // Add the following logic:
    if (sdk_type.deprecated_status) {
        *proto_type->mutable_deprecated_status() = to_proto(*sdk_type.deprecated_status);
    }
}
ModuleVersion from_proto_impl<viam::app::v1::ModuleVersion>::operator()(const viam::app::v1::ModuleVersion* proto_type) const {
    ModuleVersion sdk_type;
    // NOTE: This assumes existing conversion logic for other fields.
    // Add the following logic:
    if (proto_type->has_deprecated_status()) {
        sdk_type.deprecated_status = from_proto(proto_type->deprecated_status());
    }
    return sdk_type;
}

// RegistryItem (add deprecated_status conversion)
void to_proto_impl<RegistryItem>::operator()(const RegistryItem& sdk_type, viam::app::v1::RegistryItem* proto_type) const {
    // NOTE: This assumes existing conversion logic for other fields.
    // Add the following logic:
    if (sdk_type.deprecated_status) {
        *proto_type->mutable_deprecated_status() = to_proto(*sdk_type.deprecated_status);
    }
}
RegistryItem from_proto_impl<viam::app::v1::RegistryItem>::operator()(const viam::app::v1::RegistryItem* proto_type) const {
    RegistryItem sdk_type;
    // NOTE: This assumes existing conversion logic for other fields.
    // Add the following logic:
    if (proto_type->has_deprecated_status()) {
        sdk_type.deprecated_status = from_proto(proto_type->deprecated_status());
    }
    return sdk_type;
}

// Module (add deprecated_status conversion)
void to_proto_impl<Module>::operator()(const Module& sdk_type, viam::app::v1::Module* proto_type) const {
    // NOTE: This assumes existing conversion logic for other fields.
    // Add the following logic:
    if (sdk_type.deprecated_status) {
        *proto_type->mutable_deprecated_status() = to_proto(*sdk_type.deprecated_status);
    }
}
Module from_proto_impl<viam::app::v1::Module>::operator()(const viam::app::v1::Module* proto_type) const {
    Module sdk_type;
    // NOTE: This assumes existing conversion logic for other fields.
    // Add the following logic:
    if (proto_type->has_deprecated_status()) {
        sdk_type.deprecated_status = from_proto(proto_type->deprecated_status());
    }
    return sdk_type;
}

// VersionHistory (add deprecated_status conversion)
void to_proto_impl<VersionHistory>::operator()(const VersionHistory& sdk_type, viam::app::v1::VersionHistory* proto_type) const {
    // NOTE: This assumes existing conversion logic for other fields.
    // Add the following logic:
    if (sdk_type.deprecated_status) {
        *proto_type->mutable_deprecated_status() = to_proto(*sdk_type.deprecated_status);
    }
}
VersionHistory from_proto_impl<viam::app::v1::VersionHistory>::operator()(const viam::app::v1::VersionHistory* proto_type) const {
    VersionHistory sdk_type;
    // NOTE: This assumes existing conversion logic for other fields.
    // Add the following logic:
    if (proto_type->has_deprecated_status()) {
        sdk_type.deprecated_status = from_proto(proto_type->deprecated_status());
    }
    return sdk_type;
}

}  // namespace proto_convert_details
}  // namespace sdk
}  // namespace viam