#pragma once

#include <type_traits>

#include <boost/callable_traits/args.hpp>
#include <boost/callable_traits/return_type.hpp>
#include <boost/mp11/algorithm.hpp>

namespace viam {
namespace sdk {

namespace proto_convert_details {

// This is copied from range-v3 to allow the definition of callable object instances without
// ODR/linkage issues. It is obviated in C++17 and onwards by constexpr inline.
template <typename T>
struct static_const {
    static constexpr const T value{};
};

template <typename T>
constexpr const T static_const<T>::value;

// This struct should be explicitly specialized with a
//      void operator()(const SdkType&, common::v1::ApiType*) const
// to provide API/ABI insulated proto conversion
template <typename SdkType>
struct to_proto;

// This struct should be explicitly specialized with a
//      SdkType operator()(const ProtoType*) const
// to provided API/ABI insulated construction from proto
template <typename ProtoType>
struct from_proto;

// This is a helper type trait for deducing corresponding API types from a to_proto specialization.
// We use boost::callable_traits to generate a tuple of the arguments to the to_proto call operator,
// of which the last entry (mp_back) is a pointer to the API type.
template <typename Callable>
using ProtoArgType = std::remove_pointer_t<
    boost::mp11::mp_back<boost::callable_traits::args_t<Callable, boost::mp11::mp_list>>>;

// Implementation struct for the omni-to_proto callable defined below.
struct to_proto_impl {
    template <typename SdkType>
    auto operator()(const SdkType& t) const {
        using ProtoReturnType = ProtoArgType<to_proto<SdkType>>;

        ProtoReturnType ret;
        to_proto<SdkType>{}(t, &ret);

        return ret;
    }
};

// Implementation struct for the omni-from_proto callable defined below.
struct from_proto_impl {
    template <typename ProtoType>
    auto operator()(const ProtoType& proto) const {  // NOLINT(misc-no-recursion)
        return from_proto<ProtoType>{}(&proto);
    }
};

}  // namespace proto_convert_details

namespace v2 {

namespace {

/// @brief Function object implementing conversion from an SDK type to an API type.
/// This callable works for any type with a proto_convert_details::to_proto specialization as
/// described above.
constexpr auto& to_proto =
    proto_convert_details::static_const<proto_convert_details::to_proto_impl>::value;

/// @brief Function object implementing conversion from an API type to an SDK type.
/// This callable works for any type with a proto_convert_details::from_proto specialization as
/// described above.
constexpr auto& from_proto =
    proto_convert_details::static_const<proto_convert_details::from_proto_impl>::value;

}  // namespace

}  // namespace v2

/// @brief Type alias for the API type corresponding to a given SDK type.
/// This is the return type of calling to_proto on an instance of SdkType.
template <typename SdkType>
using EquivalentApiType =
    proto_convert_details::ProtoArgType<proto_convert_details::to_proto<SdkType>>;

/// @brief Type alias for the SDK type corresponding to a given API type.
/// This is the return type of calling from_proto on an instance of ApiType.
template <typename ApiType>
using EquivalentSdkType =
    boost::callable_traits::return_type_t<proto_convert_details::from_proto<ApiType>>;

}  // namespace sdk
}  // namespace viam
