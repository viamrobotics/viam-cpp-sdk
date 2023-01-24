#include "servo.hpp"
#include "../component_base.hpp"

ComponentType Servo::type = ComponentType("servo");

std::shared_ptr<Servo> Servo::from_robot(std::shared_ptr<RobotClient> robot, std::string name) {
    std::shared_ptr<ComponentBase> component = robot->get_component(Servo::get_resource_name(name));
    return std::dynamic_pointer_cast<Servo>(component);
};
