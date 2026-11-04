#pragma once

#include <viam/sdk/common/proto_convert.hpp>

#include <optional>
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
    /// @brief Content type of the mesh, e.g. `stl` or `ply`.
    std::string content_type;

    /// @brief Mesh bytes in the format named by `content_type`.
    std::vector<unsigned char> data;

    /// @brief Path the mesh was loaded from, relative to the kinematics file. Informational only.
    std::optional<std::string> original_file_source_path_diagnostic;

    friend bool operator==(const mesh&, const mesh&) = default;
};

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
