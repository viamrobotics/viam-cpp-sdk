#include <viam/sdk/common/mesh.hpp>

#include <tuple>

#include <viam/sdk/common/utils.hpp>

#include <common/v1/common.pb.h>

namespace viam {
namespace sdk {

bool operator==(const mesh& lhs, const mesh& rhs) {
    return std::tie(lhs.content_type, lhs.data, lhs.original_file_source_path_diagnostic) ==
           std::tie(rhs.content_type, rhs.data, rhs.original_file_source_path_diagnostic);
}

namespace proto_convert_details {
void to_proto_impl<mesh>::operator()(const mesh& self, common::v1::Mesh* proto) const {
    proto->set_content_type(self.content_type);
    proto->set_mesh(bytes_to_string(self.data));
    if (self.original_file_source_path_diagnostic) {
        proto->set_original_file_source_path_diagnostic(*self.original_file_source_path_diagnostic);
    }
}

mesh from_proto_impl<common::v1::Mesh>::operator()(const common::v1::Mesh* proto) const {
    mesh result{proto->content_type(), string_to_bytes(proto->mesh())};
    if (proto->has_original_file_source_path_diagnostic()) {
        result.original_file_source_path_diagnostic = proto->original_file_source_path_diagnostic();
    }
    return result;
}

}  // namespace proto_convert_details
}  // namespace sdk
}  // namespace viam
