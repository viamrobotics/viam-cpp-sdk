#pragma once

#include "../component_base.hpp"
#include "../../robot/client.hpp"
#include "common/v1/common.pb.h"
#include "../../registry/registry.hpp"

class Servo: public ComponentBase {
    public:
        static ComponentType type;
        void move(int angle);
        int get_position();
        // void stop() = 0;
        bool is_moving();
    private:
        static ComponentRegistration register_();
        static ComponentRegistration registration_;
};
