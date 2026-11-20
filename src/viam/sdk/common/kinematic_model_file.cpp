#include <viam/sdk/common/private/kinematic_model_file.hpp>

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <map>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <google/protobuf/util/json_util.h>

#include <viam/api/common/v1/common.pb.h>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/kinematics.hpp>
#include <viam/sdk/common/private/kinematic_model_validation.hpp>

namespace viam {
namespace sdk {
namespace impl {

namespace {

using google::protobuf::FieldDescriptor;
using google::protobuf::Message;

std::string in_quotes(const std::string& s) {
    return "\"" + s + "\"";
}

std::string to_lower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return s;
}

bool read_file(const std::filesystem::path& path, std::string* out) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        return false;
    }
    // a directory opens fine and only fails once we read from it, which read reports as the bad
    // bit, where writing the stream buffer out would look the same as an empty file
    std::string contents;
    std::vector<char> buffer(1 << 16);
    while (file.read(buffer.data(), static_cast<std::streamsize>(buffer.size())) ||
           file.gcount() > 0) {
        contents.append(buffer.data(), static_cast<std::size_t>(file.gcount()));
    }
    if (file.bad()) {
        return false;
    }
    *out = std::move(contents);
    return true;
}

// the extension of a mesh path, lowercased and without the dot
std::string extension_of(const std::string& source) {
    const std::string extension = std::filesystem::path(source).extension().string();
    return to_lower(extension.empty() ? extension : extension.substr(1));
}

void check_finite_value(double value, const std::string& name) {
    // proto JSON reads the strings "Infinity", "-Infinity" and "NaN" into a double, and a file has
    // to leave a bound out to say unbounded
    if (std::isnan(value)) {
        throw Exception(name + " is not a number, leave a bound out to make it unbounded");
    }
    if (std::isinf(value)) {
        throw Exception(name + " is infinite, leave a bound out to make it unbounded");
    }
}

// NOLINTNEXTLINE(misc-no-recursion)
void check_finite(const Message& message, const std::string& prefix) {
    const auto* reflection = message.GetReflection();
    std::vector<const FieldDescriptor*> fields;
    reflection->ListFields(message, &fields);
    for (const auto* field : fields) {
        // newer protobuf returns a string view here, which does not add to a string
        const std::string name = prefix + std::string(field->name());
        const auto type = field->cpp_type();
        if (type != FieldDescriptor::CPPTYPE_MESSAGE && type != FieldDescriptor::CPPTYPE_DOUBLE &&
            type != FieldDescriptor::CPPTYPE_FLOAT) {
            continue;
        }
        if (!field->is_repeated()) {
            if (type == FieldDescriptor::CPPTYPE_MESSAGE) {
                check_finite(reflection->GetMessage(message, field), name + ".");
            } else if (type == FieldDescriptor::CPPTYPE_DOUBLE) {
                check_finite_value(reflection->GetDouble(message, field), name);
            } else {
                check_finite_value(reflection->GetFloat(message, field), name);
            }
            continue;
        }
        const int size = reflection->FieldSize(message, field);
        for (int i = 0; i < size; ++i) {
            const std::string element = name + "[" + std::to_string(i) + "]";
            if (type == FieldDescriptor::CPPTYPE_MESSAGE) {
                check_finite(reflection->GetRepeatedMessage(message, field, i), element + ".");
            } else if (type == FieldDescriptor::CPPTYPE_DOUBLE) {
                check_finite_value(reflection->GetRepeatedDouble(message, field, i), element);
            } else {
                check_finite_value(reflection->GetRepeatedFloat(message, field, i), element);
            }
        }
    }
}

void validate(const common::v1::KinematicModel& model) {
    std::vector<kinematic_model_node> nodes;
    nodes.reserve(static_cast<std::size_t>(model.links_size()) +
                  static_cast<std::size_t>(model.joints_size()));
    for (const auto& link : model.links()) {
        nodes.push_back({"link", link.id(), link.parent(), std::nullopt, false});
    }
    for (const auto& joint : model.joints()) {
        const auto& limits = joint.limits();
        nodes.push_back(
            {"joint",
             joint.id(),
             joint.parent(),
             joint.has_mimic() ? std::optional<std::string>(joint.mimic().joint()) : std::nullopt,
             limits.has_min() || limits.has_max() || limits.has_max_velocity() ||
                 limits.has_max_acceleration()});
    }
    // a model built in code may start out empty, but a file with nothing in it is a mistake
    if (nodes.empty()) {
        throw Exception("model has no links or joints");
    }
    validate_kinematic_model_nodes(nodes);
}

// A source path has to survive a module install and a trip over the wire, so it is relative to
// the model file, uses forward slashes and is not a URI.
void check_mesh_path(const std::string& source) {
    if (source.find("://") != std::string::npos) {
        throw Exception("mesh path " + in_quotes(source) +
                        " is a URI, paths are relative to the model file");
    }
    if (source.find('\\') != std::string::npos) {
        throw Exception("mesh path " + in_quotes(source) +
                        " uses backslashes, use forward slashes");
    }
    const bool drive = source.size() > 1 && source[1] == ':' &&
                       std::isalpha(static_cast<unsigned char>(source[0])) != 0;
    if (source[0] == '/' || drive) {
        throw Exception("mesh path " + in_quotes(source) +
                        " is absolute, paths are relative to the model file");
    }
}

class mesh_loader {
   public:
    explicit mesh_loader(std::filesystem::path dir) : dir_(std::move(dir)) {}

    // A collision mesh has to be in a format viam-server can parse, while a visual mesh is only
    // ever passed along, so any format with an extension goes. The content type comes back
    // lowercased in the extension spelling, with the MIME spelling of glb accepted.
    void load(common::v1::Mesh* mesh, bool collision) {
        const std::string source = mesh->original_file_source_path_diagnostic();
        const bool has_bytes = !mesh->mesh().empty();
        if (!has_bytes && source.empty()) {
            throw Exception("mesh has neither bytes nor a source path");
        }
        std::string content_type = to_lower(mesh->content_type());
        if (content_type == "model/gltf-binary") {
            content_type = "glb";
        }
        const std::string extension = extension_of(source);
        if (!has_bytes) {
            check_mesh_path(source);
            if (extension.empty()) {
                throw Exception("mesh path " + in_quotes(source) +
                                " has no extension to take the content type from");
            }
            if (!content_type.empty() && content_type != extension) {
                throw Exception("mesh content type " + in_quotes(mesh->content_type()) +
                                " disagrees with the extension of " + in_quotes(source));
            }
            content_type = extension;
        } else if (content_type.empty()) {
            // a mesh that carries its bytes is not read again, but the path it was written out
            // with can still name the content type
            content_type = extension;
        }
        if (content_type.empty()) {
            throw Exception("inline mesh bytes need a content type");
        }
        if (collision && content_type != "stl" && content_type != "ply") {
            throw Exception("collision mesh content type " + in_quotes(content_type) +
                            " is not stl or ply");
        }
        mesh->set_content_type(content_type);
        if (has_bytes) {
            return;
        }

        // the same file named on two links is read once
        auto cached = cache_.find(source);
        if (cached == cache_.end()) {
            const std::filesystem::path resolved = dir_ / source;
            std::string data;
            if (!read_file(resolved, &data)) {
                throw Exception("failed to read mesh " + in_quotes(source) + " at " +
                                resolved.string());
            }
            cached = cache_.emplace(source, std::move(data)).first;
        }
        mesh->set_mesh(cached->second);
    }

   private:
    std::filesystem::path dir_;
    std::map<std::string, std::string> cache_;
};

void load_meshes(common::v1::KinematicModel* model, const std::filesystem::path& dir) {
    mesh_loader loader(dir);
    const auto load = [&](common::v1::Geometry* geometry, bool collision, const std::string& at) {
        if (!geometry->has_mesh()) {
            return;
        }
        try {
            loader.load(geometry->mutable_mesh(), collision);
        } catch (const Exception& xcp) {
            throw Exception(at + ": " + xcp.what());
        }
    };
    for (auto& link : *model->mutable_links()) {
        for (int i = 0; i < link.collision_size(); ++i) {
            load(link.mutable_collision(i),
                 true,
                 "link " + in_quotes(link.id()) + " collision geometry " + std::to_string(i));
        }
        for (int i = 0; i < link.visual_size(); ++i) {
            load(link.mutable_visual(i),
                 false,
                 "link " + in_quotes(link.id()) + " visual geometry " + std::to_string(i));
        }
    }
    for (auto& joint : *model->mutable_joints()) {
        if (joint.has_geometry()) {
            load(joint.mutable_geometry(), true, "joint " + in_quotes(joint.id()) + " geometry");
        }
    }
}

}  // namespace

common::v1::KinematicModel read_kinematic_model_file_message(const std::filesystem::path& path) {
    std::string json;
    if (!read_file(path, &json)) {
        throw Exception("failed to read kinematic model file " + path.string());
    }
    try {
        common::v1::KinematicModel model;
        // the default options refuse a field the message does not have, which is how a typo in
        // a file gets caught
        const auto status = google::protobuf::util::JsonStringToMessage(json, &model);
        if (!status.ok()) {
            throw Exception("not the proto JSON of a kinematic model: " + status.ToString());
        }
        check_finite(model, "");
        validate(model);
        load_meshes(&model, path.parent_path());
        return model;
    } catch (const Exception& xcp) {
        throw Exception("kinematic model file " + path.string() + ": " + xcp.what());
    }
}

}  // namespace impl

KinematicModel KinematicModel::from_file(const std::filesystem::path& path) {
    const auto message = impl::read_kinematic_model_file_message(path);
    try {
        return from_proto(message);
    } catch (const Exception& xcp) {
        // the conversion reports a joint with no type or a geometry with no shape as not
        // supported, which a gripper server would pass on as having no model at all, so we
        // rethrow it as a plain error that names the file
        throw Exception("kinematic model file " + path.string() + ": " + xcp.what());
    }
}

}  // namespace sdk
}  // namespace viam
