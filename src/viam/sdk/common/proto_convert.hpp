#pragma once

#include <boost/callable_traits/args.hpp>
#include <boost/callable_traits/return_type.hpp>
#include <boost/mp11/algorithm.hpp>

#include <type_traits>

#define VIAM_SDK_API_FWD_NAMESPACE_BEGIN(module_ns_name) \
    namespace viam {                                     \
    namespace module_ns_name {                           \
    namespace v1 {

#define VIAM_SDK_API_FWD_NAMESPACE_END \
    }                                  \
    }                                  \
    }

namespace viam {
namespace sdk {

namespace proto_convert_details {

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

template <typename Callable>
using ProtoArgType =
    std::remove_pointer_t<boost::mp11::mp_back<boost::callable_traits::args_t<Callable>>>;

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

/// @brief Function object implementing conversion from an SDK type to an API type.
/// This callable works for any type with a proto_convert_details::to_proto specialization as
/// described above.
constexpr proto_convert_details::to_proto_impl to_proto{};

/// @brief Function object implementing conversion from an API type to an SDK type.
/// This callable works for any type with a proto_convert_details::from_proto specialization as
/// described above.
constexpr proto_convert_details::from_proto_impl from_proto{};

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
