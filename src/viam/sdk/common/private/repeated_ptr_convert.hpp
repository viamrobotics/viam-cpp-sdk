#pragma once

#include <vector>

#include <google/protobuf/repeated_field.h>

#include <viam/sdk/common/proto_convert.hpp>

namespace viam {
namespace sdk {
namespace impl {

template <typename T, typename = EquivalentApiType<T>>
auto to_repeated_field(const std::vector<T>& v) {
    ::google::protobuf::RepeatedPtrField<EquivalentApiType<T>> result;
    result.Reserve(v.size());

    for (const auto& elem : v) {
        *(result.Add()) = to_proto(elem);
    }

    return result;
}

template <typename T, typename = EquivalentSdkType<T>>
auto from_repeated_field(const ::google::protobuf::RepeatedPtrField<T>& v) {
    std::vector<EquivalentSdkType<T>> result;
    result.reserve(v.size());

    for (const auto& elem : v) {
        result.push_back(from_proto(elem));
    }

    return result;
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
