#pragma once

#include <vector>

#include <boost/range/iterator_range.hpp>

#include <viam/sdk/common/proto_convert.hpp>

namespace google {
namespace protobuf {

template <typename T>
class RepeatedPtrField;

}
}  // namespace google

namespace viam {
namespace sdk {
namespace proto_convert_details {

template <typename SdkType>
struct to_proto<std::vector<SdkType>> {
    void operator()(
        const std::vector<SdkType>& vec,
        ::google::protobuf::RepeatedPtrField<EquivalentApiType<SdkType>>* ptr_field) const {
        ptr_field->Reserve(vec.size());
        for (const auto& elem : vec) {
            to_proto<SdkType>{}(elem, ptr_field->Add());
        }
    }
};

template <typename ApiType>
struct from_proto<::google::protobuf::RepeatedPtrField<ApiType>> {
    auto operator()(const ::google::protobuf::RepeatedPtrField<ApiType>* ptr_field) const {
        std::vector<EquivalentSdkType<ApiType>> result;
        result.reserve(ptr_field->size());

        for (const auto* elem :
             boost::make_iterator_range(ptr_field->pointer_begin(), ptr_field->pointer_end())) {
            result.push_back(from_proto<ApiType>{}(elem));
        }

        return result;
    }
};

}  // namespace proto_convert_details
}  // namespace sdk
}  // namespace viam
