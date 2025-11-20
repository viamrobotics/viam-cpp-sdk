#include <viam/sdk/common/mesh.hpp>

#include <viam/sdk/common/utils.hpp>

#include <common/v1/common.pb.h>

namespace viam {
namespace sdk {

namespace proto_convert_details {
void to_proto_impl<mesh>::operator()(const mesh& self, common::v1::Mesh* proto) const {
    proto->set_content_type(self.content_type);
    proto->set_mesh(bytes_to_string(self.data));
}

mesh from_proto_impl<common::v1::Mesh>::operator()(const common::v1::Mesh* proto) const {
    return mesh{proto->content_type(), string_to_bytes(proto->mesh())};
}

}  // namespace proto_convert_details
}  // namespace sdk
}  // namespace viam
