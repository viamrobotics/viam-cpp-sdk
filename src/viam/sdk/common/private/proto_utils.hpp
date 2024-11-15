/// @file common/proto_utils.hpp
///
/// @brief Utils that require generated proto includes. These should be #included
/// in cpp implementation files, but not in wrapper headers consumed by third party code.
#pragma once

#include <viam/api/common/v1/common.pb.h>

namespace viam::sdk::impl {

/// @brief Copies elements from a protobuf repeated pointer array into a std::vector. Src type
/// must have a `to_proto` method.
template <typename Src, typename Dst>
void vecToRepeatedPtr(const std::vector<Src>& vec, google::protobuf::RepeatedPtrField<Dst>& dest) {
    dest.Clear();
    dest.Reserve(vec.size());
    for (auto& x : vec) {
        *dest.Add() = x.to_proto();
    }
}

/// @brief Non-member to_proto() version. (necessary for moving generated types out of wrapper
/// headers). Takes explicit `to_proto`.
template <typename Src, typename Dst>
void vecToRepeatedPtr(const std::vector<Src>& vec,
                      google::protobuf::RepeatedPtrField<Dst>& dest,
                      Dst to_proto(const Src&)) {
    dest.Clear();
    dest.Reserve(vec.size());
    for (auto& x : vec) {
        *dest.Add() = to_proto(x);
    }
}

/// @brief Copies elements from a std::vector into a protobuf repeated pointer array. Dst type
/// must have a `from_proto` static method.
template <typename Src, typename Dst>
void repeatedPtrToVec(const google::protobuf::RepeatedPtrField<Src>& src, std::vector<Dst>& vec) {
    vec.clear();
    vec.reserve(src.size());
    for (auto& x : src) {
        vec.push_back(Dst::from_proto(x));
    }
}

/// @brief Non-member from_proto() version. (necessary for moving generated types out of wrapper
/// headers). Takes explicit `from_proto`.
template <typename Src, typename Dst>
void repeatedPtrToVec(const google::protobuf::RepeatedPtrField<Src>& src,
                      std::vector<Dst>& vec,
                      Dst from_proto(const Src&)) {
    vec.clear();
    vec.reserve(src.size());
    for (auto& x : src) {
        vec.push_back(from_proto(x));
    }
}

}  // namespace viam::sdk::impl
