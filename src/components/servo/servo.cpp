#include "servo.hpp"
#include "../component_base.hpp"
#include "../../registry/registry.hpp"
#include "client.hpp"

ComponentType Servo::type = ComponentType("servo");

ComponentRegistration Servo::register_() {
    ComponentRegistration registration = ComponentRegistration(ComponentType("servo"), "servo", ServoClient::create);
    Registry::register_component(registration);
    return registration;
}

ComponentRegistration Servo::registration_ = Servo::register_();
