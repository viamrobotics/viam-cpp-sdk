#pragma once

#include <vector>

#include <google/protobuf/repeated_field.h>

#include <viam/sdk/common/proto_convert.hpp>

namespace viam {
namespace sdk {
namespace impl {

struct to_repeated_field_ {
    template <typename T, typename = EquivalentApiType<T>>
    auto operator()(const std::vector<T>& v) const {
        ::google::protobuf::RepeatedPtrField<EquivalentApiType<T>> result;
        result.Reserve(v.size());

        for (const auto& elem : v) {
            *(result.Add()) = v2::to_proto(elem);
        }

        return result;
    }
};

struct from_repeated_field_ {
    template <typename T, typename = EquivalentSdkType<T>>
    auto operator()(const ::google::protobuf::RepeatedPtrField<T>& v) const {
        std::vector<EquivalentSdkType<T>> result;
        result.reserve(v.size());

        for (const auto& elem : v) {
            result.push_back(v2::from_proto(elem));
        }

        return result;
    }
};

constexpr to_repeated_field_ to_repeated_field;
constexpr from_repeated_field_ from_repeated_field;

}  // namespace impl
}  // namespace sdk
}  // namespace viam
