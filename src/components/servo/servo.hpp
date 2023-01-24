#pragma once

#include "../component_base.hpp"
#include "../../robot/client.hpp"
#include "common/v1/common.pb.h"
#include "../../registry/registry.hpp"

class Servo: public ComponentBase {
    public:
        static ComponentType type;
        static std::shared_ptr<Servo> from_robot(std::shared_ptr<RobotClient> robot, std::string name);
        void move(int angle);
        int get_position();
        // void stop() = 0;
        bool is_moving();
};
