# Copyright 2023 Viam Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

enable_testing()

add_library(viamsdk_test)

target_sources(viamsdk_test
  PRIVATE
    mocks/camera_mocks.cpp
    mocks/generic_mocks.cpp
    mocks/mlmodel_mocks.cpp
    mocks/mock_arm.cpp
    mocks/mock_base.cpp
    mocks/mock_board.cpp
    mocks/mock_encoder.cpp
    mocks/mock_gantry.cpp
    mocks/mock_gripper.cpp
    mocks/mock_motor.cpp
    mocks/mock_motion.cpp
    mocks/mock_movement_sensor.cpp
    mocks/mock_navigation.cpp
    mocks/mock_pose_tracker.cpp
    mocks/mock_power_sensor.cpp
    mocks/mock_sensor.cpp
    mocks/mock_servo.cpp
    mocks/mock_robot.cpp
    test_utils.cpp
)

target_link_libraries(viamsdk_test
  PUBLIC viam-cpp-sdk::viamsdk
)

viamcppsdk_add_boost_test(test_arm.cpp)
viamcppsdk_add_boost_test(test_base.cpp)
viamcppsdk_add_boost_test(test_board.cpp)
viamcppsdk_add_boost_test(test_camera.cpp)
viamcppsdk_add_boost_test(test_common.cpp)
viamcppsdk_add_boost_test(test_encoder.cpp)
viamcppsdk_add_boost_test(test_gantry.cpp)
viamcppsdk_add_boost_test(test_gripper.cpp)
viamcppsdk_add_boost_test(test_generics.cpp)
viamcppsdk_add_boost_test(test_mlmodel.cpp)
viamcppsdk_add_boost_test(test_motor.cpp)
viamcppsdk_add_boost_test(test_motion.cpp)
viamcppsdk_add_boost_test(test_movement_sensor.cpp)
viamcppsdk_add_boost_test(test_navigation.cpp)
viamcppsdk_add_boost_test(test_pose_tracker.cpp)
viamcppsdk_add_boost_test(test_power_sensor.cpp)
viamcppsdk_add_boost_test(test_proto_value.cpp)
viamcppsdk_add_boost_test(test_proto_value_visit.cpp)
viamcppsdk_add_boost_test(test_resource.cpp)
viamcppsdk_add_boost_test(test_sensor.cpp)
viamcppsdk_add_boost_test(test_servo.cpp)
viamcppsdk_add_boost_test(test_robot.cpp)
