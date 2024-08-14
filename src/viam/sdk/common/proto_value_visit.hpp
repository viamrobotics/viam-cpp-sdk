#pragma once

#include <viam/sdk/common/proto_value.hpp>

#include <type_traits>

namespace viam {
namespace sdk {

// It might seem that we could write these all with a single template, but this ends up requiring
// some annoying gymnastics with type traits that only saves around 15 lines. For more info see
// https://github.com/boostorg/json/issues/952 and the linked PR with the fix.

/// @defgroup ProtoValueVisit ProtoValue visit API
/// Invoke a function object with the contents of a ProtoValue. The function object must be callable
/// with all the possible types of ProtoValue. visit will inspect the ProtoValue to determine its
/// stored type, and then call the visitor on it with value category matching that with which the
/// ProtoValue was passed.
/// @{

template <typename Visitor>
auto visit(Visitor&& visitor, ProtoValue& value)
    -> decltype(std::forward<Visitor>(visitor)(std::declval<std::nullptr_t&>())) {
    switch (value.kind()) {
        case kind_t<bool>{}:
            return std::forward<Visitor>(visitor)(value.get_unchecked<bool>());
        case kind_t<int>{}:
            return std::forward<Visitor>(visitor)(value.get_unchecked<int>());
        case kind_t<double>{}:
            return std::forward<Visitor>(visitor)(value.get_unchecked<double>());
        case kind_t<std::string>{}:
            return std::forward<Visitor>(visitor)(value.get_unchecked<std::string>());
        case kind_t<std::vector<ProtoValue>>{}:
            return std::forward<Visitor>(visitor)(value.get_unchecked<std::vector<ProtoValue>>());
        case kind_t<ProtoStruct>{}:
            return std::forward<Visitor>(visitor)(value.get_unchecked<ProtoStruct>());
        default:
        case kind_t<std::nullptr_t>{}: {
            auto np = nullptr;
            return std::forward<Visitor>(visitor)(np);
        }
    }
}

template <typename Visitor>
auto visit(Visitor&& visitor, const ProtoValue& value)
    -> decltype(std::forward<Visitor>(visitor)(std::declval<const std::nullptr_t&>())) {
    switch (value.kind()) {
        case kind_t<bool>{}:
            return std::forward<Visitor>(visitor)(value.get_unchecked<bool>());
        case kind_t<int>{}:
            return std::forward<Visitor>(visitor)(value.get_unchecked<int>());
        case kind_t<double>{}:
            return std::forward<Visitor>(visitor)(value.get_unchecked<double>());
        case kind_t<std::string>{}:
            return std::forward<Visitor>(visitor)(value.get_unchecked<std::string>());
        case kind_t<std::vector<ProtoValue>>{}:
            return std::forward<Visitor>(visitor)(value.get_unchecked<std::vector<ProtoValue>>());
        case kind_t<ProtoStruct>{}:
            return std::forward<Visitor>(visitor)(value.get_unchecked<ProtoStruct>());
        default:
        case kind_t<std::nullptr_t>{}: {
            const auto np = nullptr;
            return std::forward<Visitor>(visitor)(np);
        }
    }
}

template <typename Visitor>
auto visit(Visitor&& visitor, ProtoValue&& value)
    -> decltype(std::forward<Visitor>(visitor)(std::declval<std::nullptr_t&&>())) {
    switch (value.kind()) {
        case kind_t<bool>{}:
            return std::forward<Visitor>(visitor)(std::move(value.get_unchecked<bool>()));
        case kind_t<int>{}:
            return std::forward<Visitor>(visitor)(std::move(value.get_unchecked<int>()));
        case kind_t<double>{}:
            return std::forward<Visitor>(visitor)(std::move(value.get_unchecked<double>()));
        case kind_t<std::string>{}:
            return std::forward<Visitor>(visitor)(std::move(value.get_unchecked<std::string>()));
        case kind_t<std::vector<ProtoValue>>{}:
            return std::forward<Visitor>(visitor)(
                std::move(value.get_unchecked<std::vector<ProtoValue>>()));
        case kind_t<ProtoStruct>{}:
            return std::forward<Visitor>(visitor)(std::move(value.get_unchecked<ProtoStruct>()));
        default:
        case kind_t<std::nullptr_t>{}:
            return std::forward<Visitor>(visitor)(std::nullptr_t());
    }
}

/// @}

}  // namespace sdk
}  // namespace viam
