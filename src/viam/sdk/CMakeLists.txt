# Declare the viamcpp library, and attach sources,
# headers for installation
add_library(viamsdk)

# TODO: This is analogized from
# https://gitlab.kitware.com/cmake/cmake/-/tree/master/Help/guide/importing-exporting/MathFunctionsComponents
# but it doesn't really seem to do anything. Everything works fine if
# it is removed, and naming the alias something other than `viamcpp`
# doesn't make it available under that alias anyway, as one might
# hope. What is it supposed to do?
add_library(viam-cpp-sdk::viamsdk ALIAS viamsdk)

# This bit of magic is required so that we get the public build-time
# include path of viamapi in *ahead* of the path anchored to `src` by
# `BASE_DIRS` below, because otherwise we will pull in the static
# versions of the proto files even if we are generating dynamically.
target_include_directories(viamsdk
  PUBLIC
    "$<BUILD_INTERFACE:$<TARGET_PROPERTY:viam-cpp-sdk::viamapi,INTERFACE_INCLUDE_DIRECTORIES>>"
)

# Configure the SDK version file
set(VIAMCPPSDK_API_PROTO_LABEL "v0.1.325")
set(VIAMCPPSDK_SDK_VERSION ${CMAKE_PROJECT_VERSION})
configure_file(common/version.cpp.in common/version.cpp @ONLY)


# Set compile and link options based on arguments
if (VIAMCPPSDK_USE_WALL_WERROR)
  target_compile_options(viamsdk PRIVATE -Wall -Werror)
endif()
if (VIAMCPPSDK_SANITIZED_BUILD)
  target_link_options(viamsdk PRIVATE -fsanitize=undefined -fno-omit-frame-pointer -fno-sanitize-recover)
  target_compile_options(viamsdk PRIVATE -fsanitize=undefined -fno-omit-frame-pointer -fno-sanitize-recover)
endif()
if (VIAMCPPSDK_CLANG_TIDY)
  find_program(CLANG_TIDY_EXE NAMES "clang-tidy-15" "clang-tidy")
  set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "--config-file=../.clang-tidy")
  set_target_properties(viamsdk PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
endif()

target_sources(viamsdk
  PRIVATE
    common/client_helper.cpp
    common/exception.cpp
    common/linear_algebra.cpp
    common/pose.cpp
    common/proto_type.cpp
    common/service_helper.cpp
    common/utils.cpp
    common/version.cpp
    common/world_state.cpp
    components/arm.cpp
    components/base.cpp
    components/board.cpp
    components/camera.cpp
    components/component.cpp
    components/encoder.cpp
    components/gantry.cpp
    components/generic.cpp
    components/gripper.cpp
    components/motor.cpp
    components/movement_sensor.cpp
    components/pose_tracker.cpp
    components/power_sensor.cpp
    components/private/arm_client.cpp
    components/private/arm_server.cpp
    components/private/base_client.cpp
    components/private/base_server.cpp
    components/private/board_client.cpp
    components/private/board_server.cpp
    components/private/camera_client.cpp
    components/private/camera_server.cpp
    components/private/encoder_client.cpp
    components/private/encoder_server.cpp
    components/private/gantry_client.cpp
    components/private/gantry_server.cpp
    components/private/generic_client.cpp
    components/private/gripper_client.cpp
    components/private/gripper_server.cpp
    components/private/generic_server.cpp
    components/private/motor_client.cpp
    components/private/motor_server.cpp
    components/private/movement_sensor_client.cpp
    components/private/movement_sensor_server.cpp
    components/private/pose_tracker_client.cpp
    components/private/pose_tracker_server.cpp
    components/private/power_sensor_client.cpp
    components/private/power_sensor_server.cpp
    components/private/sensor_client.cpp
    components/private/sensor_server.cpp
    components/private/servo_client.cpp
    components/private/servo_server.cpp
    components/sensor.cpp
    components/servo.cpp
    config/resource.cpp
    module/handler_map.cpp
    module/module.cpp
    module/service.cpp
    module/signal_manager.cpp
    referenceframe/frame.cpp
    registry/registry.cpp
    resource/reconfigurable.cpp
    resource/resource.cpp
    resource/resource_api.cpp
    resource/resource_manager.cpp
    resource/resource_server_base.cpp
    resource/stoppable.cpp
    robot/client.cpp
    robot/service.cpp
    rpc/dial.cpp
    rpc/server.cpp
    rpc/private/viam_grpc_channel.cpp
    services/generic.cpp
    services/mlmodel.cpp
    services/motion.cpp
    services/private/generic_client.cpp
    services/private/generic_server.cpp
    services/private/mlmodel.cpp
    services/private/mlmodel_client.cpp
    services/private/mlmodel_server.cpp
    services/private/motion_client.cpp
    services/private/motion_server.cpp
    services/service.cpp
    spatialmath/geometry.cpp
    spatialmath/orientation.cpp
    spatialmath/orientation_types.cpp
  PUBLIC FILE_SET viamsdk_public_includes TYPE HEADERS
    BASE_DIRS
      ../..
    FILES
      ../../viam/sdk/common/client_helper.hpp
      ../../viam/sdk/common/exception.hpp
      ../../viam/sdk/common/linear_algebra.hpp
      ../../viam/sdk/common/pose.hpp
      ../../viam/sdk/common/proto_type.hpp
      ../../viam/sdk/common/service_helper.hpp
      ../../viam/sdk/common/utils.hpp
      ../../viam/sdk/common/version.hpp
      ../../viam/sdk/common/world_state.hpp
      ../../viam/sdk/components/arm.hpp
      ../../viam/sdk/components/base.hpp
      ../../viam/sdk/components/board.hpp
      ../../viam/sdk/components/camera.hpp
      ../../viam/sdk/components/component.hpp
      ../../viam/sdk/components/encoder.hpp
      ../../viam/sdk/components/gantry.hpp
      ../../viam/sdk/components/generic.hpp
      ../../viam/sdk/components/gripper.hpp
      ../../viam/sdk/components/motor.hpp
      ../../viam/sdk/components/movement_sensor.hpp
      ../../viam/sdk/components/pose_tracker.hpp
      ../../viam/sdk/components/power_sensor.hpp
      ../../viam/sdk/components/sensor.hpp
      ../../viam/sdk/components/servo.hpp
      ../../viam/sdk/config/resource.hpp
      ../../viam/sdk/module/handler_map.hpp
      ../../viam/sdk/module/module.hpp
      ../../viam/sdk/module/service.hpp
      ../../viam/sdk/module/signal_manager.hpp
      ../../viam/sdk/referenceframe/frame.hpp
      ../../viam/sdk/registry/registry.hpp
      ../../viam/sdk/resource/reconfigurable.hpp
      ../../viam/sdk/resource/resource.hpp
      ../../viam/sdk/resource/resource_api.hpp
      ../../viam/sdk/resource/resource_manager.hpp
      ../../viam/sdk/resource/resource_server_base.hpp
      ../../viam/sdk/resource/stoppable.hpp
      ../../viam/sdk/robot/client.hpp
      ../../viam/sdk/robot/service.hpp
      ../../viam/sdk/rpc/dial.hpp
      ../../viam/sdk/rpc/message_sizes.hpp
      ../../viam/sdk/rpc/server.hpp
      ../../viam/sdk/services/generic.hpp
      ../../viam/sdk/services/mlmodel.hpp
      ../../viam/sdk/services/motion.hpp
      ../../viam/sdk/services/service.hpp
      ../../viam/sdk/spatialmath/geometry.hpp
      ../../viam/sdk/spatialmath/orientation.hpp
      ../../viam/sdk/spatialmath/orientation_types.hpp
)

set_target_properties(
  viamsdk PROPERTIES
  SOVERSION noabi
  VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}
)

target_include_directories(viamsdk
  PUBLIC
    # These expand to nothing unless we picked up grpc/proto via
    # pkgconfig rather than find_package
    "$<BUILD_INTERFACE:${VIAMCPPSDK_GRPCXX_INCLUDE_DIRS}>"
    "$<INSTALL_INTERFACE:${VIAMCPPSDK_GRPCXX_INCLUDE_DIRS}>"
    "$<BUILD_INTERFACE:${VIAMCPPSDK_GRPC_INCLUDE_DIRS}>"
    "$<INSTALL_INTERFACE:${VIAMCPPSDK_GRPC_INCLUDE_DIRS}>"
    "$<BUILD_INTERFACE:${VIAMCPPSDK_PROTOBUF_INCLUDE_DIRS}>"
    "$<INSTALL_INTERFACE:${VIAMCPPSDK_PROTOBUF_INCLUDE_DIRS}>"
)

target_link_directories(viamsdk
  PUBLIC
    # This shouldn't be necessary, but we need it so that the
    # installation path of the C++ SDK is added to the link line of
    # consumers to that `-lviam_rust_utils` can be meaningfully
    # resolved.
    #
    # TODO: Remove this when we can import `viam_rust_utils` as a real
    # imported target with its own properties.
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_LIBDIR}>"

    # These expand to nothing unless we picked up grpc/proto via
    # pkg-config rather than find_package
    "$<BUILD_INTERFACE:${VIAMCPPSDK_GRPCXX_LIBRARY_DIRS}>"
    "$<INSTALL_INTERFACE:${VIAMCPPSDK_GRPCXX_LIBRARY_DIRS}>"
    "$<BUILD_INTERFACE:${VIAMCPPSDK_GRPC_LIBRARY_DIRS}>"
    "$<INSTALL_INTERFACE:${VIAMCPPSDK_GRPC_LIBRARY_DIRS}>"
    "$<BUILD_INTERFACE:${VIAMCPPSDK_PROTOBUF_LIBRARY_DIRS}>"
    "$<INSTALL_INTERFACE:${VIAMCPPSDK_PROTOBUF_LIBRARY_DIRS}>"
)

target_link_libraries(viamsdk
  PUBLIC viam-cpp-sdk::viamapi
  PUBLIC ${VIAMCPPSDK_GRPCXX_LIBRARIES}
  PUBLIC ${VIAMCPPSDK_GRPC_LIBRARIES}
  PUBLIC ${VIAMCPPSDK_PROTOBUF_LIBRARIES}
  PUBLIC Boost::headers
  PUBLIC Boost::log
  PUBLIC xtensor
  PRIVATE ${VIAMCPPSDK_GRPCXX_REFLECTION_LIBRARIES}
  PRIVATE absl::strings
  PRIVATE viam_rust_utils
  PRIVATE Threads::Threads
)


if (APPLE)
  target_link_libraries(viamsdk PUBLIC "-framework Security")
endif()


install(
  TARGETS viamsdk
  EXPORT viamsdk
  RUNTIME COMPONENT viam-cpp-sdk_runtime
  LIBRARY COMPONENT viam-cpp-sdk_runtime
      NAMELINK_COMPONENT viam-cpp-sdk_dev
  ARCHIVE COMPONENT viam-cpp-sdk_dev
  FILE_SET viamsdk_public_includes COMPONENT viam-cpp-sdk_dev DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
  EXPORT viamsdk
  FILE viam-cpp-sdk-viamsdk-Targets.cmake
  NAMESPACE viam-cpp-sdk::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/viam-cpp-sdk
  COMPONENT viam-cpp-sdk_dev
)


# Generate a `pkg-config` file for the library component of the
# project. This is realy best effort as we can't necessarily capture
# the full set of requirements and dependencies.
#
# TODO: We should be pulling the paths and libaries out as properties
# and interpolating them in, but I can't yet figure out the right
# incantation to get CMake to expand the various generator expressions
# in the "INSTALL" context.

configure_file(
  config/viam-cpp-sdk-libviamsdk.pc.in
  config/viam-cpp-sdk-libviamsdk.pc
  @ONLY
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/config/viam-cpp-sdk-libviamsdk.pc
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
  COMPONENT viam-cpp-sdk_dev
)


add_subdirectory(tests)
