#pragma once

#include <viam/sdk/common/proto_convert.hpp>

#include <string>
#include <vector>

namespace viam {
namespace common {
namespace v1 {

class Mesh;

}  // namespace v1
}  // namespace common
}  // namespace viam

namespace viam {
namespace sdk {

struct mesh {
    std::string content_type;
    std::vector<unsigned char> data;
};

bool operator==(const mesh& lhs, const mesh& rhs);

namespace proto_convert_details {

template <>
struct to_proto_impl<mesh> {
    void operator()(const mesh&, common::v1::Mesh*) const;
};

template <>
struct from_proto_impl<common::v1::Mesh> {
    mesh operator()(const common::v1::Mesh*) const;
};

}  // namespace proto_convert_details

}  // namespace sdk
}  // namespace viam
