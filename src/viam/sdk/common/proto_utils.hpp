/// @file common/proto_utils.hpp
///
/// @brief Utils that require generated proto includes. These should be #included
/// in cpp implementation files, but not in wrapper headers consumed by third party code.
#pragma once

#include <viam/api/common/v1/common.pb.h>

namespace viam {
namespace sdk {

/// @brief Copies elements from a protobuf repeated pointer array into a std::vector. Src type
/// must be implicitly convertible to Dst (probably via operator on Src).
template <typename Src, typename Dst>
void vecToRepeatedPtr(const std::vector<Src>& vec, google::protobuf::RepeatedPtrField<Dst>& dest) {
    dest.Reserve(vec.size());
    for (auto& x : vec) {
        *dest.Add() = x.to_proto();
    }
}

/// @brief Non-member to_proto() version. (necessary for moving generated types out of wrapper headers).
/// Template param F is a function that converts from Src to Dst.
template <typename Src, typename Dst>
void vecToRepeatedPtr(const std::vector<Src>& vec, google::protobuf::RepeatedPtrField<Dst>& dest, Dst& from_proto(const Src&)) {
    dest.Reserve(vec.size());
    for (auto& x : vec) {
        *dest.Add() = from_proto(x);
    }
}

/// @brief Copies elements from a std::vector into a protobuf repeated pointer array. Src type
/// must be implicitly convertible to Dst (probably via constructor on Dst).
template <typename Src, typename Dst>
void repeatedPtrToVec(const google::protobuf::RepeatedPtrField<Src>& src, std::vector<Dst>& vec) {
    vec.reserve(src.size());
    for (auto& x : src) {
        vec.push_back(Dst::from_proto(x));
    }
}

/// @brief Non-member to_proto() version. (necessary for moving generated types out of wrapper headers).
/// Template param F is a function that converts from Src to Dst.
template <typename Src, typename Dst>
void repeatedPtrToVec(const google::protobuf::RepeatedPtrField<Src>& src, std::vector<Dst>& vec, Dst& to_proto(const Src&)) {
    vec.reserve(src.size());
    for (auto& x : src) {
        vec.push_back(to_proto(x));
    }
}

}  // namespace sdk
}  // namespace viam
