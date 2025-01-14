#pragma once

#include <type_traits>

#include <boost/callable_traits/args.hpp>
#include <boost/callable_traits/return_type.hpp>
#include <boost/mp11/algorithm.hpp>

namespace viam {
namespace sdk {

namespace proto_convert_details {

// This struct should be explicitly specialized with a
//      void operator()(const SdkType&, common::v1::ApiType*) const
// to provide API/ABI insulated proto conversion
template <typename SdkType>
struct to_proto_impl;

// This struct should be explicitly specialized with a
//      SdkType operator()(const ProtoType*) const
// to provided API/ABI insulated construction from proto
template <typename ProtoType>
struct from_proto_impl;

// This is a helper type trait for deducing corresponding API types from a to_proto specialization.
// We use boost::callable_traits to generate a tuple of the arguments to the to_proto call operator,
// of which the last entry (mp_back) is a pointer to the API type.
template <typename Callable>
using ProtoArgType = std::remove_pointer_t<
    boost::mp11::mp_back<boost::callable_traits::args_t<Callable, boost::mp11::mp_list>>>;

}  // namespace proto_convert_details

/// @brief Convert an SDK type to its corresponding API type.
/// @remark Only participates in overload resolution if to_proto_impl<SdkType> has been specialized.
template <typename SdkType,
          typename = decltype(sizeof(proto_convert_details::to_proto_impl<SdkType>))>
auto to_proto(const SdkType& t) {  // NOLINT(misc-no-recursion)
    namespace pcd = proto_convert_details;
    using ProtoReturnType = pcd::ProtoArgType<pcd::to_proto_impl<SdkType>>;

    ProtoReturnType ret;
    pcd::to_proto_impl<SdkType>{}(t, &ret);

    return ret;
}

/// @brief Convert an API type to its corresponding SDK type.
/// @remark Only participates in overload resolution if from_proto_impl<ApiType> has been
/// specialized.
template <typename ApiType,
          typename = decltype(sizeof(proto_convert_details::from_proto_impl<ApiType>))>
auto from_proto(const ApiType& proto) {  // NOLINT(misc-no-recursion)
    return proto_convert_details::from_proto_impl<ApiType>{}(&proto);
}

/// @brief Type alias for the API type corresponding to a given SDK type.
/// This is the return type of calling to_proto on an instance of SdkType.
template <typename SdkType>
using EquivalentApiType =
    proto_convert_details::ProtoArgType<proto_convert_details::to_proto_impl<SdkType>>;

/// @brief Type alias for the SDK type corresponding to a given API type.
/// This is the return type of calling from_proto on an instance of ApiType.
template <typename ApiType>
using EquivalentSdkType =
    boost::callable_traits::return_type_t<proto_convert_details::from_proto_impl<ApiType>>;

}  // namespace sdk
}  // namespace viam
