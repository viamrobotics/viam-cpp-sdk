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


# Set compile and link options based on arguments
if (VIAMCPPSDK_USE_WALL_WERROR)
  target_compile_options(viamsdk PRIVATE -Wall -Werror)
endif()
if (VIAMCPPSDK_SANITIZED_BUILD)
  target_link_options(viamsdk PRIVATE -fsanitize=undefined -fno-omit-frame-pointer -fno-sanitize-recover)
  target_compile_options(viamsdk PRIVATE -fsanitize=undefined -fno-omit-frame-pointer -fno-sanitize-recover)
endif()


target_sources(viamsdk
  PRIVATE
    # TODO(RSDK-1742): we have to put `registry` up top here because we need the
    # registry types to be defined first, before anything tries to init them.
    # this obviously isn't great. it breaks up stylistic ordering and is brittle
    # when we need to add updates. we should refactor to make this unnecessary.
    # consider making all necessary runtime values a single `context` that has to
    # be initialized within main before anything else happens?
    registry/registry.cpp
    common/proto_type.cpp
    common/utils.cpp
    components/camera/camera.cpp
    components/camera/client.cpp
    components/camera/server.cpp
    components/component_base.cpp
    components/generic/client.cpp
    components/generic/generic.cpp
    components/generic/server.cpp
    components/motor/motor.cpp
    components/motor/client.cpp
    components/motor/server.cpp
    components/service_base.cpp
    config/resource.cpp
    module/handler_map.cpp
    module/module.cpp
    module/service.cpp
    referenceframe/frame.cpp
    resource/resource.cpp
    resource/resource_base.cpp
    resource/resource_server_base.cpp
    resource/resource_type.cpp
    robot/client.cpp
    robot/service.cpp
    rpc/dial.cpp
    rpc/server.cpp
    services/service_base.cpp
    spatialmath/geometry.cpp
    spatialmath/orientation_types.cpp
    spatialmath/orientation.cpp
    subtype/subtype.cpp
    tests/test_utils.cpp
    tests/mocks/camera_mocks.cpp
    tests/mocks/generic_mocks.cpp
    tests/mocks/mock_motor.cpp
  PUBLIC FILE_SET viamsdk_public_includes TYPE HEADERS
    BASE_DIRS
      ../..
    FILES
      ../../viam/sdk/common/proto_type.hpp
      ../../viam/sdk/common/utils.hpp
      ../../viam/sdk/components/camera/camera.hpp
      ../../viam/sdk/components/camera/client.hpp
      ../../viam/sdk/components/camera/server.hpp
      ../../viam/sdk/components/component_base.hpp
      ../../viam/sdk/components/generic/client.hpp
      ../../viam/sdk/components/generic/generic.hpp
      ../../viam/sdk/components/generic/server.hpp
      ../../viam/sdk/components/motor/motor.hpp
      ../../viam/sdk/components/motor/client.hpp
      ../../viam/sdk/components/motor/server.hpp
      ../../viam/sdk/config/resource.hpp
      ../../viam/sdk/module/handler_map.hpp
      ../../viam/sdk/module/module.hpp
      ../../viam/sdk/module/service.hpp
      ../../viam/sdk/referenceframe/frame.hpp
      ../../viam/sdk/registry/registry.hpp
      ../../viam/sdk/resource/resource.hpp
      ../../viam/sdk/resource/resource_base.hpp
      ../../viam/sdk/resource/resource_server_base.hpp
      ../../viam/sdk/resource/resource_type.hpp
      ../../viam/sdk/robot/client.hpp
      ../../viam/sdk/robot/service.hpp
      ../../viam/sdk/rpc/dial.hpp
      ../../viam/sdk/rpc/server.hpp
      ../../viam/sdk/services/service_base.hpp
      ../../viam/sdk/spatialmath/geometry.hpp
      ../../viam/sdk/spatialmath/orientation_types.hpp
      ../../viam/sdk/spatialmath/orientation.hpp
      ../../viam/sdk/subtype/subtype.hpp
      ../../viam/sdk/tests/test_utils.hpp
      ../../viam/sdk/tests/mocks/camera_mocks.hpp
      ../../viam/sdk/tests/mocks/generic_mocks.hpp
      ../../viam/sdk/tests/mocks/mock_motor.hpp
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
  PRIVATE ${VIAMCPPSDK_GRPCXX_REFLECTION_LIBRARIES}
  PUBLIC Boost::headers
  PRIVATE Boost::log
  PRIVATE viam_rust_utils
  PRIVATE Threads::Threads
)


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
