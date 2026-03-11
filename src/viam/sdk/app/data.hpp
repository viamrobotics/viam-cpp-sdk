// Copyright 2024 Viam.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <optional>
#include <string>
#include <vector>

namespace viam {
namespace sdk {

/// @brief Filter for tags.
struct TagsFilter {
    /// @brief List of tags to filter by.
    std::vector<std::string> tags;
    /// @brief Whether to exclude data with these tags.
    bool exclude = false;

    // Default constructor
    TagsFilter() = default;
    // Constructor for convenience
    TagsFilter(std::vector<std::string> tags, bool exclude) : tags(std::move(tags)), exclude(exclude) {}
};

/// @brief Filter for deleting tabular data.
struct DeleteTabularFilter {
    /// @brief List of location IDs to filter by.
    std::vector<std::string> location_ids;
    /// @brief Optional robot ID to filter by.
    std::optional<std::string> robot_id;
    /// @brief Optional part ID to filter by.
    std::optional<std::string> part_id;
    /// @brief Optional component type to filter by.
    std::optional<std::string> component_type;
    /// @brief Optional component name to filter by.
    std::optional<std::string> component_name;
    /// @brief Optional method to filter by.
    std::optional<std::string> method;
    /// @brief Optional tags filter.
    std::optional<TagsFilter> tags_filter;
};

}  // namespace sdk
}  // namespace viam