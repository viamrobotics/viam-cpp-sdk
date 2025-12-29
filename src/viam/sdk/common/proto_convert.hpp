#pragma once

#include <optional>
#include <vector>
#include <viam/api/app/v1/app.pb.h> // For proto enum and message definitions
#include <viam/sdk/app/app.hpp> // For SDK app types

#include <type_traits>

#include <boost/callable_traits/args.hpp>
#include <boost/callable_traits/return_type.hpp>
#include <boost/mp11/algorithm.hpp>

namespace viam {
namespace sdk {

namespace proto_convert_details {

// This struct should be explicitly specialized with a
//      void operator()(const SdkType&, common::v1::ApiType*) const
// to provide API/ABI insulated proto conversion
template <typename SdkType>
struct to_proto_impl;

// This struct should be explicitly specialized with a
//      SdkType operator()(const ProtoType*) const
// to provided API/ABI insulated construction from proto
template <typename ProtoType>
struct from_proto_impl;

// Forward declarations for new enum specializations
template <>
struct to_proto_impl<viam::sdk::app::ModuleSourceType> {
    void operator()(const viam::sdk::app::ModuleSourceType& sdk_type,
                    viam::app::v1::ModuleSourceType* proto_type) const;
};

template <>
struct from_proto_impl<viam::app::v1::ModuleSourceType> {
    viam::sdk::app::ModuleSourceType operator()(
        const viam::app::v1::ModuleSourceType* proto_type) const;
};

template <>
struct to_proto_impl<viam::sdk::app::ModuleLanguage> {
    void operator()(const viam::sdk::app::ModuleLanguage& sdk_type,
                    viam::app::v1::ModuleLanguage* proto_type) const;
};

template <>
struct from_proto_impl<viam::app::v1::ModuleLanguage> {
    viam::sdk::app::ModuleLanguage operator()(
        const viam::app::v1::ModuleLanguage* proto_type) const;
};

// This is a helper type trait for deducing corresponding API types from a to_proto specialization.
// We use boost::callable_traits to generate a tuple of the arguments to the to_proto call operator,
// of which the last entry (mp_back) is a pointer to the API type.
template <typename Callable>
using ProtoArgType = std::remove_pointer_t<
    boost::mp11::mp_back<boost::callable_traits::args_t<Callable, boost::mp11::mp_list>>>;

// Specializations for ModuleMetadata
template <>
struct to_proto_impl<viam::sdk::app::ModuleMetadata> {
    void operator()(const viam::sdk::app::ModuleMetadata& sdk_type,
                    viam::app::v1::ModuleMetadata* proto_type) const;
};

template <>
struct from_proto_impl<viam::app::v1::ModuleMetadata> {
    viam::sdk::app::ModuleMetadata operator()(
        const viam::app::v1::ModuleMetadata* proto_type) const;
};

// Specializations for ListRegistryItemsRequest
template <>
struct to_proto_impl<viam::sdk::app::ListRegistryItemsRequest> {
    void operator()(const viam::sdk::app::ListRegistryItemsRequest& sdk_type,
                    viam::app::v1::ListRegistryItemsRequest* proto_type) const;
};

template <>
struct from_proto_impl<viam::app::v1::ListRegistryItemsRequest> {
    viam::sdk::app::ListRegistryItemsRequest operator()(
        const viam::app::v1::ListRegistryItemsRequest* proto_type) const;
};

// Specializations for UpdateModuleMetadata
template <>
struct to_proto_impl<viam::sdk::app::UpdateModuleMetadata> {
    void operator()(const viam::sdk::app::UpdateModuleMetadata& sdk_type,
                    viam::app::v1::UpdateModuleMetadata* proto_type) const;
};

template <>
struct from_proto_impl<viam::app::v1::UpdateModuleMetadata> {
    viam::sdk::app::UpdateModuleMetadata operator()(
        const viam::app::v1::UpdateModuleMetadata* proto_type) const;
};

}  // namespace proto_convert_details

/// @brief Convert an SDK type to its corresponding API type.
/// @remark Only participates in overload resolution if to_proto_impl<SdkType> has been specialized.
template <typename SdkType,
          typename = decltype(sizeof(proto_convert_details::to_proto_impl<SdkType>))>
auto to_proto(const SdkType& t) {  // NOLINT(misc-no-recursion)
    namespace pcd = proto_convert_details;
    using ProtoReturnType = pcd::ProtoArgType<pcd::to_proto_impl<SdkType>>;

    ProtoReturnType ret;
    pcd::to_proto_impl<SdkType>{}(t, &ret);

    return ret;
}

/// @brief Convert an API type to its corresponding SDK type.
/// @remark Only participates in overload resolution if from_proto_impl<ApiType> has been
/// specialized.
template <typename ApiType,
          typename = decltype(sizeof(proto_convert_details::from_proto_impl<ApiType>))>
auto from_proto(const ApiType& proto) {  // NOLINT(misc-no-recursion)
    return proto_convert_details::from_proto_impl<ApiType>{}(&proto);
}

/// @brief Type alias for the API type corresponding to a given SDK type.
/// This is the return type of calling to_proto on an instance of SdkType.
template <typename SdkType>
using EquivalentApiType =
    proto_convert_details::ProtoArgType<proto_convert_details::to_proto_impl<SdkType>>;

/// @brief Type alias for the SDK type corresponding to a given API type.
/// This is the return type of calling from_proto on an instance of ApiType.
template <typename ApiType>
using EquivalentSdkType =
    boost::callable_traits::return_type_t<proto_convert_details::from_proto_impl<ApiType>>;

// Implementations for ModuleMetadata
inline void proto_convert_details::to_proto_impl<viam::sdk::app::ModuleMetadata>::operator()(
    const viam::sdk::app::ModuleMetadata& sdk_type,
    viam::app::v1::ModuleMetadata* proto_type) const {
    proto_type->set_name(sdk_type.name());
    proto_type->set_version(sdk_type.version());
    if (sdk_type.source_type()) {
        proto_type->set_source_type(to_proto(*sdk_type.source_type()));
    }
    if (sdk_type.language()) {
        proto_type->set_language(to_proto(*sdk_type.language()));
    }
}

inline viam::sdk::app::ModuleMetadata
proto_convert_details::from_proto_impl<viam::app::v1::ModuleMetadata>::operator()(
    const viam::app::v1::ModuleMetadata* proto_type) const {
    viam::sdk::app::ModuleMetadata sdk_type;
    sdk_type.with_name(proto_type->name());
    sdk_type.with_version(proto_type->version());
    if (proto_type->has_source_type()) {
        sdk_type.with_source_type(from_proto(proto_type->source_type()));
    }
    if (proto_type->has_language()) {
        sdk_type.with_language(from_proto(proto_type->language()));
    }
    return sdk_type;
}

// Implementations for ListRegistryItemsRequest
inline void proto_convert_details::to_proto_impl<viam::sdk::app::ListRegistryItemsRequest>::operator()(
    const viam::sdk::app::ListRegistryItemsRequest& sdk_type,
    viam::app::v1::ListRegistryItemsRequest* proto_type) const {
    proto_type->set_name(sdk_type.name());
    for (const auto& type : sdk_type.module_source_types()) {
        proto_type->add_module_source_types(to_proto(type));
    }
    for (const auto& lang : sdk_type.module_languages()) {
        proto_type->add_module_languages(to_proto(lang));
    }
}

inline viam::sdk::app::ListRegistryItemsRequest
proto_convert_details::from_proto_impl<viam::app::v1::ListRegistryItemsRequest>::operator()(
    const viam::app::v1::ListRegistryItemsRequest* proto_type) const {
    viam::sdk::app::ListRegistryItemsRequest sdk_type;
    sdk_type.with_name(proto_type->name());

    std::vector<viam::sdk::app::ModuleSourceType> source_types;
    for (const auto& type : proto_type->module_source_types()) {
        source_types.push_back(from_proto(type));
    }
    sdk_type.with_module_source_types(std::move(source_types));

    std::vector<viam::sdk::app::ModuleLanguage> languages;
    for (const auto& lang : proto_type->module_languages()) {
        languages.push_back(from_proto(lang));
    }
    sdk_type.with_module_languages(std::move(languages));
    return sdk_type;
}

// Implementations for UpdateModuleMetadata
inline void proto_convert_details::to_proto_impl<viam::sdk::app::UpdateModuleMetadata>::operator()(
    const viam::sdk::app::UpdateModuleMetadata& sdk_type,
    viam::app::v1::UpdateModuleMetadata* proto_type) const {
    proto_type->set_name(sdk_type.name());
    proto_type->set_version(sdk_type.version());
    if (sdk_type.source_type()) {
        proto_type->set_source_type(to_proto(*sdk_type.source_type()));
    }
    if (sdk_type.language()) {
        proto_type->set_language(to_proto(*sdk_type.language()));
    }
}

inline viam::sdk::app::UpdateModuleMetadata
proto_convert_details::from_proto_impl<viam::app::v1::UpdateModuleMetadata>::operator()(
    const viam::app::v1::UpdateModuleMetadata* proto_type) const {
    viam::sdk::app::UpdateModuleMetadata sdk_type;
    sdk_type.with_name(proto_type->name());
    sdk_type.with_version(proto_type->version());
    if (proto_type->has_source_type()) {
        sdk_type.with_source_type(from_proto(proto_type->source_type()));
    }
    if (proto_type->has_language()) {
        sdk_type.with_language(from_proto(proto_type->language()));
    }
    return sdk_type;
}

// Implementations for ModuleSourceType enum
inline void proto_convert_details::to_proto_impl<viam::sdk::app::ModuleSourceType>::operator()(
    const viam::sdk::app::ModuleSourceType& sdk_type,
    viam::app::v1::ModuleSourceType* proto_type) const {
    switch (sdk_type) {
        case viam::sdk::app::ModuleSourceType::MODULE_SOURCE_TYPE_UNSPECIFIED:
            *proto_type = viam::app::v1::MODULE_SOURCE_TYPE_UNSPECIFIED;
            break;
        case viam::sdk::app::ModuleSourceType::MODULE_SOURCE_TYPE_GIT:
            *proto_type = viam::app::v1::MODULE_SOURCE_TYPE_GIT;
            break;
        case viam::sdk::app::ModuleSourceType::MODULE_SOURCE_TYPE_LOCAL:
            *proto_type = viam::app::v1::MODULE_SOURCE_TYPE_LOCAL;
            break;
        case viam::sdk::app::ModuleSourceType::MODULE_SOURCE_TYPE_REGISTRY:
            *proto_type = viam::app::v1::MODULE_SOURCE_TYPE_REGISTRY;
            break;
        default:
            *proto_type = viam::app::v1::MODULE_SOURCE_TYPE_UNSPECIFIED;
            break;
    }
}

inline viam::sdk::app::ModuleSourceType
proto_convert_details::from_proto_impl<viam::app::v1::ModuleSourceType>::operator()(
    const viam::app::v1::ModuleSourceType* proto_type) const {
    switch (*proto_type) {
        case viam::app::v1::MODULE_SOURCE_TYPE_UNSPECIFIED:
            return viam::sdk::app::ModuleSourceType::MODULE_SOURCE_TYPE_UNSPECIFIED;
        case viam::app::v1::MODULE_SOURCE_TYPE_GIT:
            return viam::sdk::app::ModuleSourceType::MODULE_SOURCE_TYPE_GIT;
        case viam::app::v1::MODULE_SOURCE_TYPE_LOCAL:
            return viam::sdk::app::ModuleSourceType::MODULE_SOURCE_TYPE_LOCAL;
        case viam::app::v1::MODULE_SOURCE_TYPE_REGISTRY:
            return viam::sdk::app::ModuleSourceType::MODULE_SOURCE_TYPE_REGISTRY;
        default:
            return viam::sdk::app::ModuleSourceType::MODULE_SOURCE_TYPE_UNSPECIFIED;
    }
}

// Implementations for ModuleLanguage enum
inline void proto_convert_details::to_proto_impl<viam::sdk::app::ModuleLanguage>::operator()(
    const viam::sdk::app::ModuleLanguage& sdk_type,
    viam::app::v1::ModuleLanguage* proto_type) const {
    switch (sdk_type) {
        case viam::sdk::app::ModuleLanguage::MODULE_LANGUAGE_UNSPECIFIED:
            *proto_type = viam::app::v1::MODULE_LANGUAGE_UNSPECIFIED;
            break;
        case viam::sdk::app::ModuleLanguage::MODULE_LANGUAGE_PYTHON:
            *proto_type = viam::app::v1::MODULE_LANGUAGE_PYTHON;
            break;
        case viam::sdk::app::ModuleLanguage::MODULE_LANGUAGE_GO:
            *proto_type = viam::app::v1::MODULE_LANGUAGE_GO;
            break;
        case viam::sdk::app::ModuleLanguage::MODULE_LANGUAGE_OTHER:
            *proto_type = viam::app::v1::MODULE_LANGUAGE_OTHER;
            break;
        default:
            *proto_type = viam::app::v1::MODULE_LANGUAGE_UNSPECIFIED;
            break;
    }
}

inline viam::sdk::app::ModuleLanguage
proto_convert_details::from_proto_impl<viam::app::v1::ModuleLanguage>::operator()(
    const viam::app::v1::ModuleLanguage* proto_type) const {
    switch (*proto_type) {
        case viam::app::v1::MODULE_LANGUAGE_UNSPECIFIED:
            return viam::sdk::app::ModuleLanguage::MODULE_LANGUAGE_UNSPECIFIED;
        case viam::app::v1::MODULE_LANGUAGE_PYTHON:
            return viam::sdk::app::ModuleLanguage::MODULE_LANGUAGE_PYTHON;
        case viam::app::v1::MODULE_LANGUAGE_GO:
            return viam::sdk::app::ModuleLanguage::MODULE_LANGUAGE_GO;
        case viam::app::v1::MODULE_LANGUAGE_OTHER:
            return viam::sdk::app::ModuleLanguage::MODULE_LANGUAGE_OTHER;
        default:
            return viam::sdk::app::ModuleLanguage::MODULE_LANGUAGE_UNSPECIFIED;
    }
}

}  // namespace sdk
}  // namespace viam