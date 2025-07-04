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
  PRIVATE
    "$<BUILD_INTERFACE:$<TARGET_PROPERTY:viam-cpp-sdk::viamapi,INTERFACE_INCLUDE_DIRECTORIES>>"
)

# Configure the SDK version file
if (VIAMCPPSDK_USE_LOCAL_PROTOS)
  set(VIAMCPPSDK_API_PROTO_LABEL "unknown (local generation)")
else()
  file(READ "${CMAKE_CURRENT_SOURCE_DIR}/../api/api_proto_tag.lock" VIAMCPPSDK_API_PROTO_LABEL)
  string(REGEX REPLACE "\n$" "" VIAMCPPSDK_API_PROTO_LABEL "${VIAMCPPSDK_API_PROTO_LABEL}")
endif()
message(WARNING "api proto label is ${VIAMCPPSDK_API_PROTO_LABEL}")
configure_file(common/private/version_metadata.hpp.in common/private/version_metadata.hpp @ONLY)

# Configure the grpc client forward declarations file
configure_file(common/grpc_fwd.hpp.in common/grpc_fwd.hpp)

# Set compile and link options based on arguments
if (VIAMCPPSDK_USE_WALL_WERROR)
  if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(viamsdk PRIVATE -Wall -Werror)
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
   target_compile_options(viamsdk PRIVATE -Wall -Werror)
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(viamsdk PRIVATE -Wall -Werror)
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Note: Currently, the generated protos throw W4244 and W4267, so turn that back to a warning.
    target_compile_options(viamsdk PRIVATE /W4 /WX /wd4244 /wd4267)
  else()
    message(FATAL_ERROR "VIAMCPPSDK_USE_WALL_ERROR is set, but not known how to enable for compiler ID ${CMAKE_CXX_COMPILER_ID}")
  endif()
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
    common/instance.cpp
    common/linear_algebra.cpp
    common/pose.cpp
    common/proto_value.cpp
    common/utils.cpp
    common/version_metadata.cpp
    common/world_state.cpp
    common/private/service_helper.cpp
    components/arm.cpp
    components/base.cpp
    components/board.cpp
    components/button.cpp
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
    components/private/button_client.cpp
    components/private/button_server.cpp
    components/private/camera_server.cpp
    components/private/encoder.cpp
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
    components/private/switch_client.cpp
    components/private/switch_server.cpp
    components/sensor.cpp
    components/servo.cpp
    components/switch.cpp
    config/resource.cpp
    log/logging.cpp
    log/private/log_backend.cpp
    module/handler_map.cpp
    module/module.cpp
    module/service.cpp
    referenceframe/frame.cpp
    registry/registry.cpp
    resource/reconfigurable.cpp
    resource/resource.cpp
    resource/resource_api.cpp
    resource/resource_manager.cpp
    resource/resource_server_base.cpp
    resource/stoppable.cpp
    robot/client.cpp
    rpc/dial.cpp
    rpc/server.cpp
    rpc/private/viam_grpc_channel.cpp
    services/discovery.cpp
    services/generic.cpp
    services/mlmodel.cpp
    services/motion.cpp
    services/navigation.cpp
    services/private/discovery_client.cpp
    services/private/discovery_server.cpp
    services/private/generic_client.cpp
    services/private/generic_server.cpp
    services/private/mlmodel.cpp
    services/private/mlmodel_client.cpp
    services/private/mlmodel_server.cpp
    services/private/motion_client.cpp
    services/private/motion_server.cpp
    services/private/navigation_client.cpp
    services/private/navigation_server.cpp
    services/service.cpp
    spatialmath/geometry.cpp
    spatialmath/orientation.cpp
    spatialmath/orientation_types.cpp
  PUBLIC FILE_SET viamsdk_public_includes TYPE HEADERS
    BASE_DIRS
      ../..
      ${CMAKE_CURRENT_BINARY_DIR}/../..
    FILES
      ../../viam/sdk/common/client_helper.hpp
      ../../viam/sdk/common/exception.hpp
      ../../viam/sdk/common/instance.hpp
      ../../viam/sdk/common/linear_algebra.hpp
      ../../viam/sdk/common/pose.hpp
      ../../viam/sdk/common/proto_convert.hpp
      ../../viam/sdk/common/proto_value.hpp
      ../../viam/sdk/common/utils.hpp
      ../../viam/sdk/common/version_metadata.hpp
      ../../viam/sdk/common/world_state.hpp
      ../../viam/sdk/components/arm.hpp
      ../../viam/sdk/components/base.hpp
      ../../viam/sdk/components/board.hpp
      ../../viam/sdk/components/button.hpp
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
      ../../viam/sdk/components/switch.hpp
      ../../viam/sdk/config/resource.hpp
      ../../viam/sdk/log/logging.hpp
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
      ../../viam/sdk/rpc/dial.hpp
      ../../viam/sdk/rpc/message_sizes.hpp
      ../../viam/sdk/rpc/server.hpp
      ../../viam/sdk/services/discovery.hpp
      ../../viam/sdk/services/generic.hpp
      ../../viam/sdk/services/mlmodel.hpp
      ../../viam/sdk/services/motion.hpp
      ../../viam/sdk/services/navigation.hpp
      ../../viam/sdk/services/service.hpp
      ../../viam/sdk/spatialmath/geometry.hpp
      ../../viam/sdk/spatialmath/orientation.hpp
      ../../viam/sdk/spatialmath/orientation_types.hpp
      ${CMAKE_CURRENT_BINARY_DIR}/../../viam/sdk/common/grpc_fwd.hpp
)

if(WIN32)
  target_sources(viamsdk
    PRIVATE
      module/signal_manager_windows.cpp
  )
else()
  target_sources(viamsdk
    PRIVATE
      module/signal_manager_posix.cpp
  )
endif()

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
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../..>"
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

viamcppsdk_link_viam_api(viamsdk PRIVATE)

target_link_libraries(viamsdk
  PUBLIC Boost::headers
  PUBLIC Boost::log
  PUBLIC Boost::log_setup
  PUBLIC xtensor
  PRIVATE ${VIAMCPPSDK_GRPCXX_REFLECTION_LIBRARIES}
  PRIVATE ${VIAMCPPSDK_GRPCXX_LIBRARIES}
  PRIVATE ${VIAMCPPSDK_GRPC_LIBRARIES}
  PRIVATE ${VIAMCPPSDK_PROTOBUF_LIBRARIES}
  PRIVATE Threads::Threads
)

# if the `viam_rust_utils` target exists then we should use it. If not then
# we're probably on a non-x86_64 windows build or some other platform without
# automated `rust-utils` builds, so we should use the stubs instead.
if (TARGET viam_rust_utils)
  target_link_libraries(viamsdk
    PRIVATE viam_rust_utils
  )
else()
  target_sources(viamsdk PRIVATE rpc/private/viam_rust_utils_stubs.cpp)
endif()


# TODO several of these dependencies should properly be attached to `viam_rust_utils`,
# not `viamsdk`. However, we currently are unable to do so while maintaining compilation
if (APPLE)
  target_link_libraries(viamsdk PUBLIC "-framework Security")
elseif (NOT WIN32)
  target_link_libraries(viamsdk PRIVATE dl)
  target_link_libraries(viamsdk PRIVATE rt)
else()
  target_link_libraries(viamsdk INTERFACE Ncrypt.lib)
  target_link_libraries(viamsdk INTERFACE Secur32.lib)
  target_link_libraries(viamsdk INTERFACE Ntdll.lib)
  target_link_libraries(viamsdk INTERFACE Userenv.lib)
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

if (VIAMCPPSDK_BUILD_TESTS)
  add_subdirectory(tests)
endif()
