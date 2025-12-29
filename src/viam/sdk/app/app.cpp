#include <utility> // For std::move
#include <optional>
#include <vector>
#include <viam/sdk/app/app.hpp> // For SDK app types

#include <viam/sdk/common/proto/app.pb.h>

namespace viam {
namespace sdk {

// ModuleMetadata
ModuleMetadata::ModuleMetadata(std::string name, std::string version, std::string URL, std::optional<ModuleSourceType> source_type, std::optional<ModuleLanguage> language)
    : name_(std::move(name)),
      version_(std::move(version)),
      URL_(std::move(URL)),
      source_type_(std::move(source_type)),
      language_(std::move(language)) {}

std::optional<ModuleSourceType> ModuleMetadata::source_type() const {
    return source_type_;
}

std::optional<ModuleLanguage> ModuleMetadata::language() const {
    return language_;
}

ModuleMetadata& ModuleMetadata::with_source_type(ModuleSourceType source_type) {
    source_type_ = std::move(source_type);
    return *this;
}

ModuleMetadata& ModuleMetadata::with_language(ModuleLanguage language) {
    language_ = std::move(language);
    return *this;
}

// ListRegistryItemsRequest
ListRegistryItemsRequest::ListRegistryItemsRequest(std::vector<ModuleSourceType> module_source_types, std::vector<ModuleLanguage> module_languages)
    : module_source_types_(std::move(module_source_types)),
      module_languages_(std::move(module_languages)) {}

std::vector<ModuleSourceType> ListRegistryItemsRequest::module_source_types() const {
    return module_source_types_;
}

std::vector<ModuleLanguage> ListRegistryItemsRequest::module_languages() const {
    return module_languages_;
}

ListRegistryItemsRequest& ListRegistryItemsRequest::with_module_source_types(std::vector<ModuleSourceType> module_source_types) {
    module_source_types_ = std::move(module_source_types);
    return *this;
}

ListRegistryItemsRequest& ListRegistryItemsRequest::with_module_languages(std::vector<ModuleLanguage> module_languages) {
    module_languages_ = std::move(module_languages);
    return *this;
}

// UpdateModuleMetadata
UpdateModuleMetadata::UpdateModuleMetadata(std::optional<ModuleSourceType> source_type, std::optional<ModuleLanguage> language)
    : source_type_(std::move(source_type)),
      language_(std::move(language)) {}

std::optional<ModuleSourceType> UpdateModuleMetadata::source_type() const {
    return source_type_;
}

std::optional<ModuleLanguage> UpdateModuleMetadata::language() const {
    return language_;
}

UpdateModuleMetadata& UpdateModuleMetadata::with_source_type(ModuleSourceType source_type) {
    source_type_ = std::move(source_type);
    return *this;
}

UpdateModuleMetadata& UpdateModuleMetadata::with_language(ModuleLanguage language) {
    language_ = std::move(language);
    return *this;
}

}  // namespace sdk
}  // namespace viam
