#include <optional>
#include <string>
#include <vector>

#include "viam/sdk/common/proto.hpp"
#include "viam/sdk/common/utils.hpp"

namespace viam {
namespace sdk {
namespace app {

// Represents a module that can be used in a Viam application.
struct ModuleMetadata {
    std::string name;
    std::string version;
    std::string description;
    std::vector<std::string> tags;
    std::string url;
    std::string digest;
    std::optional<ModuleSourceType> source_type_;
    std::optional<ModuleLanguage> language_;

    ModuleMetadata(std::string name,
                   std::string version,
                   std::string description,
                   std::vector<std::string> tags,
                   std::string url,
                   std::string digest,
                   std::optional<ModuleSourceType> source_type = std::nullopt,
                   std::optional<ModuleLanguage> language = std::nullopt)
        : name(std::move(name)),
          version(std::move(version)),
          description(std::move(description)),
          tags(std::move(tags)),
          url(std::move(url)),
          digest(std::move(digest)),
          source_type_(std::move(source_type)),
          language_(std::move(language)) {}

    std::optional<ModuleSourceType> source_type() const;
    std::optional<ModuleLanguage> language() const;

    ModuleMetadata& with_source_type(ModuleSourceType source_type);
    ModuleMetadata& with_language(ModuleLanguage language);
};

// Represents a request to list registry items.
struct ListRegistryItemsRequest {
    std::string name_filter;
    std::vector<std::string> tag_filter;
    std::vector<ModuleSourceType> module_source_types_;
    std::vector<ModuleLanguage> module_languages_;

    ListRegistryItemsRequest(std::string name_filter,
                             std::vector<std::string> tag_filter,
                             std::vector<ModuleSourceType> module_source_types = {},
                             std::vector<ModuleLanguage> module_languages = {})
        : name_filter(std::move(name_filter)),
          tag_filter(std::move(tag_filter)),
          module_source_types_(std::move(module_source_types)),
          module_languages_(std::move(module_languages)) {}

    std::vector<ModuleSourceType> module_source_types() const;
    std::vector<ModuleLanguage> module_languages() const;

    ListRegistryItemsRequest& with_module_source_types(std::vector<ModuleSourceType> module_source_types);
    ListRegistryItemsRequest& with_module_languages(std::vector<ModuleLanguage> module_languages);
};

// Represents an update to module metadata.
struct UpdateModuleMetadata {
    std::string name;
    std::string version;
    std::optional<std::string> description;
    std::optional<std::vector<std::string>> tags;
    std::optional<std::string> url;
    std::optional<std::string> digest;
    std::optional<ModuleSourceType> source_type_;
    std::optional<ModuleLanguage> language_;

    UpdateModuleMetadata(std::string name,
                         std::string version,
                         std::optional<std::string> description,
                         std::optional<std::vector<std::string>> tags,
                         std::optional<std::string> url,
                         std::optional<std::string> digest,
                         std::optional<ModuleSourceType> source_type = std::nullopt,
                         std::optional<ModuleLanguage> language = std::nullopt)
        : name(std::move(name)),
          version(std::move(version)),
          description(std::move(description)),
          tags(std::move(tags)),
          url(std::move(url)),
          digest(std::move(digest)),
          source_type_(std::move(source_type)),
          language_(std::move(language)) {}

    std::optional<ModuleSourceType> source_type() const;
    std::optional<ModuleLanguage> language() const;

    UpdateModuleMetadata& with_source_type(ModuleSourceType source_type);
    UpdateModuleMetadata& with_language(ModuleLanguage language);
};

}  // namespace app
}  // namespace sdk
}  // namespace viam
