#include <utility> // For std::move
#include <optional>
#include <vector>
#include <viam/sdk/app/app.hpp> // For SDK app types
#include <viam/api/app/v1/app.pb.h> // For proto enum and message definitions
#include <viam/sdk/common/proto_convert.hpp> // For proto_convert_details

namespace viam {
namespace sdk {

// ModuleSourceType conversions
template <>
void to_proto_impl<viam::sdk::app::ModuleSourceType>::operator()(
    const viam::sdk::app::ModuleSourceType& sdk_type,
    viam::app::v1::ModuleSourceType* proto_type) const {
    *proto_type = static_cast<viam::app::v1::ModuleSourceType>(sdk_type);
}

template <>
viam::sdk::app::ModuleSourceType from_proto_impl<viam::app::v1::ModuleSourceType>::operator()(
    const viam::app::v1::ModuleSourceType* proto_type) const {
    return static_cast<viam::sdk::app::ModuleSourceType>(*proto_type);
}

// ModuleLanguage conversions
template <>
void to_proto_impl<viam::sdk::app::ModuleLanguage>::operator()(
    const viam::sdk::app::ModuleLanguage& sdk_type,
    viam::app::v1::ModuleLanguage* proto_type) const {
    *proto_type = static_cast<viam::app::v1::ModuleLanguage>(sdk_type);
}

template <>
viam::sdk::app::ModuleLanguage from_proto_impl<viam::app::v1::ModuleLanguage>::operator()(
    const viam::app::v1::ModuleLanguage* proto_type) const {
    return static_cast<viam::sdk::app::ModuleLanguage>(*proto_type);
}

// ModuleMetadata conversions
template <>
void to_proto_impl<viam::sdk::app::ModuleMetadata>::operator()(
    const viam::sdk::app::ModuleMetadata& sdk_type,
    viam::app::v1::ModuleMetadata* proto_type) const {
    proto_type->set_name(sdk_type.name());
    proto_type->set_version(sdk_type.version());
    proto_type->set_description(sdk_type.description());
    proto_type->set_author(sdk_type.author());
    proto_type->set_license(sdk_type.license());
    proto_type->set_url(sdk_type.url());
    proto_type->set_repository_url(sdk_type.repository_url());
    proto_type->set_icon(sdk_type.icon());
    proto_type->set_labels(sdk_type.labels());
    if (sdk_type.source_type()) {
        proto_type->set_source_type(to_proto(*sdk_type.source_type()));
    }
    if (sdk_type.language()) {
        proto_type->set_language(to_proto(*sdk_type.language()));
    }
}

template <>
viam::sdk::app::ModuleMetadata from_proto_impl<viam::app::v1::ModuleMetadata>::operator()(
    const viam::app::v1::ModuleMetadata* proto_type) const {
    viam::sdk::app::ModuleMetadata sdk_type;
    sdk_type.with_name(proto_type->name());
    sdk_type.with_version(proto_type->version());
    sdk_type.with_description(proto_type->description());
    sdk_type.with_author(proto_type->author());
    sdk_type.with_license(proto_type->license());
    sdk_type.with_url(proto_type->url());
    sdk_type.with_repository_url(proto_type->repository_url());
    sdk_type.with_icon(proto_type->icon());
    sdk_type.with_labels(proto_type->labels());
    if (proto_type->has_source_type()) {
        sdk_type.with_source_type(from_proto(proto_type->source_type()));
    }
    if (proto_type->has_language()) {
        sdk_type.with_language(from_proto(proto_type->language()));
    }
    return sdk_type;
}

// ListRegistryItemsRequest conversions
template <>
void to_proto_impl<viam::sdk::app::ListRegistryItemsRequest>::operator()(
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

template <>
viam::sdk::app::ListRegistryItemsRequest from_proto_impl<viam::app::v1::ListRegistryItemsRequest>::operator()(
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

// UpdateModuleMetadata conversions
template <>
void to_proto_impl<viam::sdk::app::UpdateModuleMetadata>::operator()(
    const viam::sdk::app::UpdateModuleMetadata& sdk_type,
    viam::app::v1::UpdateModuleMetadata* proto_type) const {
    proto_type->set_name(sdk_type.name());
    proto_type->set_version(sdk_type.version());
    proto_type->set_description(sdk_type.description());
    proto_type->set_author(sdk_type.author());
    proto_type->set_license(sdk_type.license());
    proto_type->set_url(sdk_type.url());
    proto_type->set_repository_url(sdk_type.repository_url());
    proto_type->set_icon(sdk_type.icon());
    proto_type->set_labels(sdk_type.labels());
    if (sdk_type.source_type()) {
        proto_type->set_source_type(to_proto(*sdk_type.source_type()));
    }
    if (sdk_type.language()) {
        proto_type->set_language(to_proto(*sdk_type.language()));
    }
}

template <>
viam::sdk::app::UpdateModuleMetadata from_proto_impl<viam::app::v1::UpdateModuleMetadata>::operator()(
    const viam::app::v1::UpdateModuleMetadata* proto_type) const {
    viam::sdk::app::UpdateModuleMetadata sdk_type;
    sdk_type.with_name(proto_type->name());
    sdk_type.with_version(proto_type->version());
    sdk_type.with_description(proto_type->description());
    sdk_type.with_author(proto_type->author());
    sdk_type.with_license(proto_type->license());
    sdk_type.with_url(proto_type->url());
    sdk_type.with_repository_url(proto_type->repository_url());
    sdk_type.with_icon(proto_type->icon());
    sdk_type.with_labels(proto_type->labels());
    if (proto_type->has_source_type()) {
        sdk_type.with_source_type(from_proto(proto_type->source_type()));
    }
    if (proto_type->has_language()) {
        sdk_type.with_language(from_proto(proto_type->language()));
    }
    return sdk_type;
}

}  // namespace proto_convert_details
}  // namespace sdk
}  // namespace viam
