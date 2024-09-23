#pragma once

#include <type_traits>

#include <viam/sdk/common/proto_value.hpp>

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
        case ProtoValue::Kind::k_bool:
            return std::forward<Visitor>(visitor)(value.get_unchecked<bool>());
        case ProtoValue::Kind::k_double:
            return std::forward<Visitor>(visitor)(value.get_unchecked<double>());
        case ProtoValue::Kind::k_string:
            return std::forward<Visitor>(visitor)(value.get_unchecked<std::string>());
        case ProtoValue::Kind::k_list:
            return std::forward<Visitor>(visitor)(value.get_unchecked<ProtoList>());
        case ProtoValue::Kind::k_struct:
            return std::forward<Visitor>(visitor)(value.get_unchecked<ProtoStruct>());
        case ProtoValue::Kind::k_null: {
            auto np = nullptr;
            return std::forward<Visitor>(visitor)(np);
        }
    }
}

template <typename Visitor>
auto visit(Visitor&& visitor, const ProtoValue& value)
    -> decltype(std::forward<Visitor>(visitor)(std::declval<const std::nullptr_t&>())) {
    switch (value.kind()) {
        case ProtoValue::Kind::k_bool:
            return std::forward<Visitor>(visitor)(value.get_unchecked<bool>());
        case ProtoValue::Kind::k_double:
            return std::forward<Visitor>(visitor)(value.get_unchecked<double>());
        case ProtoValue::Kind::k_string:
            return std::forward<Visitor>(visitor)(value.get_unchecked<std::string>());
        case ProtoValue::Kind::k_list:
            return std::forward<Visitor>(visitor)(value.get_unchecked<ProtoList>());
        case ProtoValue::Kind::k_struct:
            return std::forward<Visitor>(visitor)(value.get_unchecked<ProtoStruct>());
        case ProtoValue::Kind::k_null: {
            const auto np = nullptr;
            return std::forward<Visitor>(visitor)(np);
        }
    }
}

template <typename Visitor>
auto visit(Visitor&& visitor, ProtoValue&& value)
    -> decltype(std::forward<Visitor>(visitor)(std::declval<std::nullptr_t&&>())) {
    switch (value.kind()) {
        case ProtoValue::Kind::k_bool:
            return std::forward<Visitor>(visitor)(std::move(value.get_unchecked<bool>()));
        case ProtoValue::Kind::k_double:
            return std::forward<Visitor>(visitor)(std::move(value.get_unchecked<double>()));
        case ProtoValue::Kind::k_string:
            return std::forward<Visitor>(visitor)(std::move(value.get_unchecked<std::string>()));
        case ProtoValue::Kind::k_list:
            return std::forward<Visitor>(visitor)(std::move(value.get_unchecked<ProtoList>()));
        case ProtoValue::Kind::k_struct:
            return std::forward<Visitor>(visitor)(std::move(value.get_unchecked<ProtoStruct>()));
        case ProtoValue::Kind::k_null:
            return std::forward<Visitor>(visitor)(std::nullptr_t());
    }
}

/// @}

}  // namespace sdk
}  // namespace viam
