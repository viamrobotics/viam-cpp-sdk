#include <viam/sdk/referenceframe/kinematics_model_table.hpp>

#include <cmath>
#include <map>
#include <set>
#include <sstream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#if defined(__has_include) && (__has_include(<xtensor/generators/xbuilder.hpp>))
#include <xtensor/generators/xbuilder.hpp>
#else
#include <xtensor/xbuilder.hpp>
#endif
#if defined(__has_include) && (__has_include(<xtensor/containers/xarray.hpp>))
#include <xtensor/containers/xarray.hpp>
#else
#include <xtensor/xarray.hpp>
#endif

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/referenceframe/private/kinematics_model_table_internals.hpp>

namespace viam {
namespace sdk {
namespace impl {

namespace {

Vector3 parse_triple(const std::string& s) {
    std::istringstream iss(s);
    double x = 0;
    double y = 0;
    double z = 0;
    if (!(iss >> x >> y >> z)) {
        throw Exception(ErrorCondition::k_general,
                        "URDFToModelTable: failed to parse space-delimited triple: '" + s + "'");
    }
    return Vector3{x, y, z};
}

double magnitude(const Vector3& v) {
    return std::sqrt(v.x() * v.x() + v.y() * v.y() + v.z() * v.z());
}

}  // namespace

std::vector<ParsedJoint> parse_urdf(const KinematicsDataURDF& urdf) {
    namespace pt = boost::property_tree;
    pt::ptree tree;
    const std::string text(reinterpret_cast<const char*>(urdf.bytes.data()), urdf.bytes.size());
    std::istringstream iss(text);
    try {
        pt::read_xml(iss, tree);
    } catch (const pt::xml_parser_error& e) {
        throw Exception(ErrorCondition::k_general,
                        std::string("URDFToModelTable: failed to parse URDF XML: ") + e.what());
    }

    const auto robot_it = tree.find("robot");
    if (robot_it == tree.not_found()) {
        throw Exception(ErrorCondition::k_general,
                        "URDFToModelTable: no <robot> root element in URDF");
    }
    const pt::ptree& robot = robot_it->second;

    std::vector<ParsedJoint> joints;
    for (const auto& child : robot) {
        if (child.first != "joint") {
            continue;
        }
        const pt::ptree& jnode = child.second;

        ParsedJoint j;
        j.name = jnode.get<std::string>("<xmlattr>.name", "");
        j.type_str = jnode.get<std::string>("<xmlattr>.type", "");
        j.parent_link = jnode.get<std::string>("parent.<xmlattr>.link", "");
        j.child_link = jnode.get<std::string>("child.<xmlattr>.link", "");

        if (j.type_str.empty() || j.parent_link.empty() || j.child_link.empty()) {
            throw Exception(ErrorCondition::k_general,
                            "URDFToModelTable: joint '" + j.name +
                                "' missing required attribute (type/parent/child)");
        }

        const auto origin_opt = jnode.get_child_optional("origin");
        if (origin_opt) {
            j.xyz = parse_triple(origin_opt->get<std::string>("<xmlattr>.xyz", "0 0 0"));
            j.rpy = parse_triple(origin_opt->get<std::string>("<xmlattr>.rpy", "0 0 0"));
        }
        const auto axis_opt_node = jnode.get_child_optional("axis");
        if (axis_opt_node) {
            j.axis_opt = parse_triple(axis_opt_node->get<std::string>("<xmlattr>.xyz", "1 0 0"));
        }
        joints.push_back(std::move(j));
    }
    return joints;
}

std::vector<ParsedJoint> walk_urdf_chain(const std::vector<ParsedJoint>& joints) {
    std::map<std::string, std::vector<const ParsedJoint*>> by_parent;
    std::set<std::string> all_parents;
    std::set<std::string> all_children;
    for (const auto& j : joints) {
        by_parent[j.parent_link].push_back(&j);
        all_parents.insert(j.parent_link);
        all_children.insert(j.child_link);
    }

    std::vector<std::string> roots;
    for (const auto& p : all_parents) {
        if (all_children.find(p) == all_children.end()) {
            roots.push_back(p);
        }
    }
    if (roots.size() != 1) {
        std::string list;
        for (const auto& r : roots) {
            list += "'" + r + "' ";
        }
        throw Exception(ErrorCondition::k_general,
                        "URDFToModelTable: expected exactly one root link, found " +
                            std::to_string(roots.size()) + ": " + list);
    }

    std::vector<ParsedJoint> ordered;
    ordered.reserve(joints.size());
    std::set<std::string> visited;
    std::string current = roots[0];
    while (true) {
        if (!visited.insert(current).second) {
            throw Exception(ErrorCondition::k_general,
                            "URDFToModelTable: cycle in URDF chain at link '" + current + "'");
        }
        auto it = by_parent.find(current);
        if (it == by_parent.end() || it->second.empty()) {
            break;
        }
        if (it->second.size() > 1) {
            throw Exception(ErrorCondition::k_general,
                            "URDFToModelTable: branching topology at link '" + current + "' (" +
                                std::to_string(it->second.size()) + " outgoing joints)");
        }
        const ParsedJoint* j = it->second.front();
        ordered.push_back(*j);
        current = j->child_link;
    }
    if (ordered.size() != joints.size()) {
        throw Exception(ErrorCondition::k_general,
                        "URDFToModelTable: chain walk visited " + std::to_string(ordered.size()) +
                            " of " + std::to_string(joints.size()) + " joints");
    }
    return ordered;
}

JointRow to_row(const ParsedJoint& parsed) {
    JointRow row;
    row.name = parsed.name;
    row.xyz = parsed.xyz;
    row.rpy = parsed.rpy;

    if (parsed.type_str == "revolute") {
        row.type = JointType::k_revolute;
    } else if (parsed.type_str == "continuous") {
        row.type = JointType::k_continuous;
    } else if (parsed.type_str == "prismatic") {
        row.type = JointType::k_prismatic;
    } else if (parsed.type_str == "fixed") {
        row.type = JointType::k_fixed;
    } else {
        throw Exception(ErrorCondition::k_not_supported,
                        "URDFToModelTable: joint '" + parsed.name + "' has unsupported type '" +
                            parsed.type_str +
                            "' (supported: revolute, continuous, prismatic, fixed)");
    }

    if (row.type == JointType::k_fixed) {
        row.axis = Vector3{0, 0, 0};
    } else if (parsed.axis_opt) {
        if (magnitude(*parsed.axis_opt) < 1e-12) {
            throw Exception(
                ErrorCondition::k_general,
                "URDFToModelTable: joint '" + parsed.name + "' has zero-magnitude axis");
        }
        row.axis = *parsed.axis_opt;
    } else {
        row.axis = Vector3{1, 0, 0};  // URDF default
    }
    return row;
}

}  // namespace impl

ModelTable kinematics_to_model_table(const KinematicsDataURDF& urdf) {
    using namespace impl;
    auto chain = walk_urdf_chain(parse_urdf(urdf));
    ModelTable table;
    table.reserve(chain.size());
    for (const auto& j : chain) {
        table.push_back(to_row(j));
    }
    return table;
}

xt::xarray<double> model_table_to_tensor(const ModelTable& table) {
    if (table.empty()) {
        throw Exception(ErrorCondition::k_general, "model_table_to_tensor: empty model table");
    }
    xt::xarray<double> out = xt::zeros<double>({table.size(), std::size_t{10}});
    for (std::size_t i = 0; i < table.size(); ++i) {
        const auto& r = table[i];
        out(i, 0) = r.xyz.x();
        out(i, 1) = r.xyz.y();
        out(i, 2) = r.xyz.z();
        out(i, 3) = r.rpy.x();
        out(i, 4) = r.rpy.y();
        out(i, 5) = r.rpy.z();
        out(i, 6) = r.axis.x();
        out(i, 7) = r.axis.y();
        out(i, 8) = r.axis.z();
        out(i, 9) = static_cast<double>(static_cast<int>(r.type));
    }
    return out;
}

ModelTable tensor_to_model_table(const xt::xarray<double>& tensor) {
    if (tensor.dimension() != 2) {
        throw Exception(ErrorCondition::k_general,
                        "tensor_to_model_table: expected 2D tensor, got " +
                            std::to_string(tensor.dimension()) + "D");
    }
    if (tensor.shape()[1] != 10) {
        throw Exception(ErrorCondition::k_general,
                        "tensor_to_model_table: expected shape (n, 10), got (n, " +
                            std::to_string(tensor.shape()[1]) + ")");
    }
    if (tensor.shape()[0] == 0) {
        throw Exception(ErrorCondition::k_general, "tensor_to_model_table: empty tensor");
    }
    ModelTable table;
    table.reserve(tensor.shape()[0]);
    for (std::size_t i = 0; i < tensor.shape()[0]; ++i) {
        const double type_v = tensor(i, 9);
        const int type_i = static_cast<int>(type_v);
        if (static_cast<double>(type_i) != type_v) {
            throw Exception(ErrorCondition::k_general,
                            "tensor_to_model_table: row " + std::to_string(i) +
                                " joint type value " + std::to_string(type_v) +
                                " is not an integer");
        }
        JointType joint_type;
        switch (static_cast<JointType>(type_i)) {
            case JointType::k_revolute:
            case JointType::k_continuous:
            case JointType::k_prismatic:
            case JointType::k_fixed:
                joint_type = static_cast<JointType>(type_i);
                break;
            default:
                throw Exception(ErrorCondition::k_general,
                                "tensor_to_model_table: row " + std::to_string(i) +
                                    " joint type value " + std::to_string(type_i) +
                                    " does not match any JointType");
        }
        JointRow row;
        row.xyz = Vector3{tensor(i, 0), tensor(i, 1), tensor(i, 2)};
        row.rpy = Vector3{tensor(i, 3), tensor(i, 4), tensor(i, 5)};
        row.axis = Vector3{tensor(i, 6), tensor(i, 7), tensor(i, 8)};
        row.type = joint_type;
        table.push_back(std::move(row));
    }
    return table;
}

}  // namespace sdk
}  // namespace viam
