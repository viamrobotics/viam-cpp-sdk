#include "impl.hpp"

#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>

#include <grpcpp/support/status.h>

#include <viam/sdk/components/base/base.hpp>
#include <viam/sdk/components/component.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/resource/resource.hpp>

using namespace viam::sdk;

std::string find_motor(ResourceConfig cfg, std::string motor_name) {
    auto base_name = cfg.name();
    auto motor = cfg.attributes()->find(motor_name);
    if (motor == cfg.attributes()->end()) {
        std::ostringstream buffer;
        buffer << base_name << ": Required parameter `" << motor_name
               << "` not found in configuration";
        throw std::invalid_argument(buffer.str());
    }
    const auto* const motor_string = motor->second->get<std::string>();
    if (!motor_string || motor_string->empty()) {
        std::ostringstream buffer;
        buffer << base_name << ": Required non-empty string parameter `" << motor_name
               << "` is either not a string "
                  "or is an empty string";
        throw std::invalid_argument(buffer.str());
    }
    return *motor_string;
}

void MyBase::reconfigure(Dependencies deps, ResourceConfig cfg) {
    // Downcast `left` and `right` dependencies to motors.
    auto left = find_motor(cfg, "left");
    auto right = find_motor(cfg, "right");
    for (const auto& kv : deps) {
        if (kv.first.short_name() == left) {
            left_ = std::dynamic_pointer_cast<Motor>(kv.second);
        }
        if (kv.first.short_name() == right) {
            right_ = std::dynamic_pointer_cast<Motor>(kv.second);
        }
    }
}

std::vector<std::string> MyBase::validate(ResourceConfig cfg) {
    // Custom validation can be done by specifying a validate function at the
    // time of resource registration (see complex/main.cpp) like this one.
    // Validate functions can `throw` exceptions that will be returned to the
    // parent through gRPC. Validate functions can also return a vector of
    // strings representing the implicit dependencies of the resource.
    //
    // Here, we return the names of the "left" and "right" motors as found in
    // the attributes as implicit dependencies of the base.
    return {find_motor(cfg, "left"), find_motor(cfg, "right")};
}

bool MyBase::is_moving() {
    return left_->is_moving() || right_->is_moving();
}

void MyBase::stop(const AttributeMap& extra) {
    std::exception* e;

    // make sure we try to stop both motors, even if the first fails.
    try {
        left_->stop(extra);
    } catch (const std::exception& err) {
        *e = err;
    }

    try {
        right_->stop(extra);
    } catch (const std::exception& err) {
        *e = err;
    }

    // if we received an err from either motor, throw it.
    if (e) {
        throw *e;
    }
}

void MyBase::set_power(const Vector3& linear, const Vector3& angular, const AttributeMap& extra) {
    // Stop the base if absolute value of linear and angular velocity is less
    // than 0.01.
    if (abs(linear.y()) < 0.01 && abs(angular.z()) < 0.01) {
        stop(extra);  // ignore returned status code from stop
        return;
    }

    // Use linear and angular velocity to calculate percentage of max power to
    // pass to set_power for left & right motors
    auto sum = abs(linear.y()) + abs(angular.z());
    left_->set_power(((linear.y() - angular.z()) / sum), extra);
    right_->set_power(((linear.y() + angular.z()) / sum), extra);
}

AttributeMap MyBase::do_command(const AttributeMap& command) {
    std::cout << "Received DoCommand request for MyBase " << Resource::name() << std::endl;
    return command;
}

std::vector<GeometryConfig> MyBase::get_geometries(const AttributeMap& extra) {
    auto left_geometries = left_->get_geometries(extra);
    auto right_geometries = right_->get_geometries(extra);
    std::vector<GeometryConfig> geometries(left_geometries);
    geometries.insert(geometries.end(), right_geometries.begin(), right_geometries.end());
    return geometries;
}

Base::properties MyBase::get_properties(const AttributeMap& extra) {
    // Return fake properties.
    return {2, 4, 8};
}
