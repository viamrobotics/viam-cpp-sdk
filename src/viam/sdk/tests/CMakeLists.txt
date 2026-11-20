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
    mocks/mock_audio_in.cpp
    mocks/mock_audio_out.cpp
    mocks/mock_base.cpp
    mocks/mock_board.cpp
    mocks/mock_button.cpp
    mocks/mock_encoder.cpp
    mocks/mock_gantry.cpp
    mocks/mock_gripper.cpp
    mocks/mock_motor.cpp
    mocks/mock_motion.cpp
    mocks/mock_movement_sensor.cpp
    mocks/mock_discovery.cpp
    mocks/mock_pose_tracker.cpp
    mocks/mock_power_sensor.cpp
    mocks/mock_sensor.cpp
    mocks/mock_servo.cpp
    mocks/mock_switch.cpp
    mocks/mock_robot.cpp
    test_utils.cpp
)

target_include_directories(viamsdk_test
  PUBLIC
    "$<BUILD_INTERFACE:$<TARGET_PROPERTY:viam-cpp-sdk::viamapi,INTERFACE_INCLUDE_DIRECTORIES>>"
)

target_link_libraries(viamsdk_test
  PUBLIC viam-cpp-sdk::viamsdk
  # Tests compile Boost.Test in via boost/test/included/unit_test.hpp; also linking
  # the shared unit_test_framework duplicates its globals and double-frees at exit.
  # Sources here must therefore not reference Boost.Test (see test_utils.hpp).
  PUBLIC Boost::headers
)

viamcppsdk_link_viam_api(viamsdk_test PUBLIC)

viamcppsdk_add_boost_test(test_arm.cpp)
viamcppsdk_add_boost_test(test_audio_in.cpp)
viamcppsdk_add_boost_test(test_audio_out.cpp)
viamcppsdk_add_boost_test(test_base.cpp)
viamcppsdk_add_boost_test(test_board.cpp)
viamcppsdk_add_boost_test(test_button.cpp)
viamcppsdk_add_boost_test(test_camera.cpp)
viamcppsdk_add_boost_test(test_common.cpp)
viamcppsdk_add_boost_test(test_data_consumer.cpp)
viamcppsdk_add_boost_test(test_discovery.cpp)
viamcppsdk_add_boost_test(test_encoder.cpp)
viamcppsdk_add_boost_test(test_framesystem.cpp)
viamcppsdk_add_boost_test(test_gantry.cpp)
viamcppsdk_add_boost_test(test_gripper.cpp)
viamcppsdk_add_boost_test(test_generics.cpp)
viamcppsdk_add_boost_test(test_log.cpp)
viamcppsdk_add_boost_test(test_mlmodel.cpp)
# test_module runs a real ModuleService on unix sockets and stops it by sending SIGTERM to the
# serving thread with pthread_kill. Neither exists on Windows, where the module is stopped through
# console control events instead.
if(NOT WIN32)
  viamcppsdk_add_boost_test(test_module.cpp)
endif()
viamcppsdk_add_boost_test(test_motor.cpp)
viamcppsdk_add_boost_test(test_motion.cpp)
viamcppsdk_add_boost_test(test_movement_sensor.cpp)
viamcppsdk_add_boost_test(test_pose_tracker.cpp)
viamcppsdk_add_boost_test(test_power_sensor.cpp)
viamcppsdk_add_boost_test(test_proto_value.cpp)
viamcppsdk_add_boost_test(test_proto_value_visit.cpp)
viamcppsdk_add_boost_test(test_resource.cpp)
viamcppsdk_add_boost_test(test_sensor.cpp)
viamcppsdk_add_boost_test(test_servo.cpp)
viamcppsdk_add_boost_test(test_switch.cpp)
viamcppsdk_add_boost_test(test_robot.cpp)
viamcppsdk_add_boost_test(test_kinematic_model.cpp)
viamcppsdk_add_boost_test(test_kinematics_model_table.cpp)

target_compile_definitions(test_kinematics_model_table
  PRIVATE
    VIAMCPPSDK_TEST_TESTFILES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/testfiles"
)

# The kinematic model loader is held to rdk's fixtures, which every SDK's loader has to agree on.
# We fetch that one directory at a pinned commit rather than keep a copy of it here. A build with
# no network can point VIAMCPPSDK_KINEMATIC_MODEL_FIXTURES_DIR at the directory in an rdk checkout.
set(VIAMCPPSDK_RDK_FIXTURES_COMMIT cefbb70808035ab976bb4df5e8184da400865ff5)
set(VIAMCPPSDK_RDK_FIXTURES_PATH referenceframe/testfiles/kinematic_models)
set(VIAMCPPSDK_KINEMATIC_MODEL_FIXTURES_DIR "" CACHE PATH
  "Directory of rdk's kinematic model fixtures. Fetched from rdk when empty.")

if (VIAMCPPSDK_KINEMATIC_MODEL_FIXTURES_DIR)
  set(VIAMCPPSDK_KINEMATIC_MODELS_DIR "${VIAMCPPSDK_KINEMATIC_MODEL_FIXTURES_DIR}")
else()
  set(VIAMCPPSDK_RDK_FIXTURES_CHECKOUT
    "${CMAKE_CURRENT_BINARY_DIR}/rdk_fixtures/${VIAMCPPSDK_RDK_FIXTURES_COMMIT}")
  set(VIAMCPPSDK_KINEMATIC_MODELS_DIR
    "${VIAMCPPSDK_RDK_FIXTURES_CHECKOUT}/${VIAMCPPSDK_RDK_FIXTURES_PATH}")
  # the stamp is written last, so a fetch that was cut short is started over
  if (NOT EXISTS "${VIAMCPPSDK_RDK_FIXTURES_CHECKOUT}/fetched")
    find_package(Git QUIET)
    set(VIAMCPPSDK_GIT_RESULT "git was not found")
    file(REMOVE_RECURSE "${VIAMCPPSDK_RDK_FIXTURES_CHECKOUT}")
    file(MAKE_DIRECTORY "${VIAMCPPSDK_RDK_FIXTURES_CHECKOUT}")
    message(STATUS "Fetching kinematic model fixtures from rdk at ${VIAMCPPSDK_RDK_FIXTURES_COMMIT}")
    # rdk is large, so we fetch the one commit without its files and let the checkout of the
    # fixture directory pull only the files under it. Line ending conversion is off because the
    # expected files hold the mesh bytes exactly as they are in rdk.
    foreach(VIAMCPPSDK_GIT_ARGS IN ITEMS
        "init;-q"
        "remote;add;origin;https://github.com/viamrobotics/rdk.git"
        "-c;core.autocrlf=false;fetch;-q;--depth;1;--filter=blob:none;origin;${VIAMCPPSDK_RDK_FIXTURES_COMMIT}"
        "-c;core.autocrlf=false;checkout;-q;FETCH_HEAD;--;${VIAMCPPSDK_RDK_FIXTURES_PATH}")
      if (NOT GIT_FOUND)
        break()
      endif()
      execute_process(
        COMMAND "${GIT_EXECUTABLE}" ${VIAMCPPSDK_GIT_ARGS}
        WORKING_DIRECTORY "${VIAMCPPSDK_RDK_FIXTURES_CHECKOUT}"
        RESULT_VARIABLE VIAMCPPSDK_GIT_RESULT
      )
      if (NOT VIAMCPPSDK_GIT_RESULT EQUAL 0)
        break()
      endif()
    endforeach()
    if (VIAMCPPSDK_GIT_RESULT EQUAL 0)
      file(WRITE "${VIAMCPPSDK_RDK_FIXTURES_CHECKOUT}/fetched" "")
    else()
      # tests are on by default, so a failed fetch must not stop someone with no network from
      # building the SDK. The fixture test fails instead, and says why.
      message(WARNING
        "Failed to fetch the kinematic model fixtures from rdk, so test_kinematic_model will "
        "fail. Set VIAMCPPSDK_KINEMATIC_MODEL_FIXTURES_DIR to ${VIAMCPPSDK_RDK_FIXTURES_PATH} "
        "in an rdk checkout to run it without the network.")
    endif()
  endif()
endif()

target_compile_definitions(test_kinematic_model
  PRIVATE
    VIAMCPPSDK_TEST_KINEMATIC_MODELS_DIR="${VIAMCPPSDK_KINEMATIC_MODELS_DIR}"
)
