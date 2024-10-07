#include "impl.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <grpcpp/support/status.h>

#include <viam/sdk/components/component.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/resource/resource.hpp>

using namespace viam::sdk;

std::string find_arg1(ResourceConfig cfg) {
    auto gizmo_name = cfg.name();
    auto arg1 = cfg.attributes().find("arg1");
    if (arg1 == cfg.attributes().end()) {
        std::ostringstream buffer;
        buffer << gizmo_name << ": Required parameter `arg1` not found in configuration";
        throw std::invalid_argument(buffer.str());
    }

    const ProtoValue& arg1_val = arg1->second;
    if (arg1_val.is_a<std::string>() && !arg1_val.get_unchecked<std::string>().empty()) {
        return arg1_val.get_unchecked<std::string>();
    }
    std::ostringstream buffer;
    buffer << gizmo_name << ": Required non-empty string parameter `arg1`"
           << "` is either not a string or is an empty string";
    throw std::invalid_argument(buffer.str());
}

void MyGizmo::reconfigure(const Dependencies& deps, const ResourceConfig& cfg) {
    arg1_ = find_arg1(cfg);
}

std::vector<std::string> MyGizmo::validate(ResourceConfig cfg) {
    // Custom validation can be done by specifying a validate function at the
    // time of resource registration (see complex/main.cpp) like this one.
    // Validate functions can `throw` exceptions that will be returned to the
    // parent through gRPC. Validate functions can also return a vector of
    // strings representing the implicit dependencies of the resource.
    //
    // Here, we return no implicit dependencies ({}). find_arg1 will `throw` an
    // exception if the `arg1` attribute is missing, is not a string or is an
    // empty string.
    find_arg1(cfg);
    return {};
}

bool MyGizmo::do_one(std::string arg1) {
    return arg1_ == arg1;
}

bool MyGizmo::do_one_client_stream(std::vector<std::string> arg1) {
    if (arg1.empty()) {
        return false;
    }
    bool resp = true;
    for (const std::string& arg : arg1) {
        resp = resp && arg1_ == arg;
    }
    return resp;
}

std::vector<bool> MyGizmo::do_one_server_stream(std::string arg1) {
    return {arg1_ == arg1, false, true, false};
}

std::vector<bool> MyGizmo::do_one_bidi_stream(std::vector<std::string> arg1) {
    std::vector<bool> resp = {};
    for (const std::string& arg : arg1) {
        resp.push_back(arg1_ == arg);
    }
    return resp;
}

std::string MyGizmo::do_two(bool arg1) {
    std::string arg1_string = arg1 ? "true" : "false";
    return "arg1=" + arg1_string;
}
