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


# TODO: [required] fixup libviam_rust_utils install name after download
# TODO: Sanitizer build configs
# TODO: All warnings and warnings as errors
# TODO: Runtime dependency sets?
# TODO: Finding external projects vs building in (e.g grpc)
# TODO: Better clang-format integration (versions, dependencies, only changed, etc.)
# TODO: Version from git describe?
# TODO: Export macros and visibilty annotations
# TODO: CPack? CTest?


# This is a tricky decision. If we set this too old, we lose access to
# modern CMake facilities that we want to be able to use, like file
# sets. If we set it too new, then the system CMake can't be used to
# build the product. Here, we make an opinionated choice to require
# the very latest stable CMake as of this writing, arguing that for
# most uses of this product, it is expected to be cross-compiling for
# the target platform on a development system, meaning that we are not
# constrained by the version of CMake available on target systems.

if (WIN32)
  # Use a newer version of CMake on Windows, so we are NEW for
  # https://cmake.org/cmake/help/latest/policy/CMP0149.html
  cmake_minimum_required(VERSION 3.27 FATAL_ERROR)
else()
  cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
endif()

set(CMAKE_PROJECT_VERSION 0.8.0)

# Identify the project.
project(viam-cpp-sdk
  VERSION ${CMAKE_PROJECT_VERSION}
  DESCRIPTION "Viam Robotics C++ SDK"
  HOMEPAGE_URL https://github.com/viamrobotics/viam-cpp-sdk
  LANGUAGES CXX
)


# Configure cmake-level options:

# - `BUILD_SHARED_LIBS`
#
# Enabled by default so that we produce a
# modern SDK, this option can be set to `OFF` to build a static
# library.
# Note that the pkg-config files installed by the project do not work
# for a static build; please use Conan for more robust pkg-config support.
#
option(BUILD_SHARED_LIBS "If enabled, build shared libraries" ON)


# Configure project-global options:

# - `VIAMCPPSDK_ENFORCE_COMPILER_MINIMA`
#
# The user can elect to disable enforcement of compiler minima (at
# their peril).
#
option(VIAMCPPSDK_ENFORCE_COMPILER_MINIMA "Control whether we validate the selected compiler against known minima" ON)


# - `VIAMCPPSDK_OFFLINE_PROTO_GENERATION`
#
# Do not use buf.builds remote definitions or services and use the
# local proto compiler plugins to do generation.
#
option(VIAMCPPSDK_OFFLINE_PROTO_GENERATION "Use local tools for proto generation" OFF)


# - `VIAMCPPSDK_USE_DYNAMIC_PROTOS`
#
# The user can select whether to use the prebuilt proto files under
# the `gen` directory, or to dynamically regenerate them as part of
# the build. In the latter mode, additional targets are defined that
# will copy the newly generated protos back to the source directory so
# they can be committed:
#
# ```
# cmake ... -DVIAMCPPSDK_USE_DYNAMIC_PROTOS=ON
# [make|ninja|...] update-static-protos
# ```
# Remember to check-in the results after regenerating!
#
option(VIAMCPPSDK_USE_DYNAMIC_PROTOS "Regenerate protos as part of building (network access may be required)" ON)


# - `VIAMCPPSDK_USE_LOCAL_PROTOS`
#
# Builds against local proto definitions, as opposed to those in the
# buf registry. This flag assumes that there is a `buf.work.yaml` file
# in the parent directory of the C++ SDK source directory, and that
# the API repo is in a sibling directory to the C++ SDK source, with
# the directory name `api`. This flag requires
# `VIAMCPPSDK_USE_DYNAMIC_PROTOS=ON` to be set.
#
option(VIAMCPPSDK_USE_LOCAL_PROTOS "Generate protos against sibling `api` directory per parent directory `buf.workspace` file")


# - `VIAMCPPSDK_USE_WALL_WERROR`
#
# This causes the SDK's internal code to compile with `-Wall` and
# `-Werror` flags.
#
option(VIAMCPPSDK_USE_WALL_WERROR "Build with -Wall and -Werror flags" ON)


# - `VIAMCPPSDK_SANITIZED_BUILD`
#
# This causes the SDK to build with UBSan, helping in the detection of
# undefined behavior. Note that builds with this flag will be less
# performant.
#
option(VIAMCPPSDK_SANITIZED_BUILD "Build with address and UB sanitizers (less performant)" OFF)


# - `VIAMCPPSDK_CLANG_TIDY`
#
# This causes the SDK to run clang-tidy with the options specified in
# the .clang-tidy file.
#
option(VIAMCPPSDK_CLANG_TIDY "Run the clang-tidy linter" OFF)

# The following options are only defined if this project is not being included as a subproject
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  option(VIAMCPPSDK_BUILD_EXAMPLES "Build the example executables" ON)
  option(VIAMCPPSDK_BUILD_TESTS "Build the example executables" ON)
endif()


# Enforce known toolchains and toolchain minima unless asked not to.
if (VIAMCPPSDK_ENFORCE_COMPILER_MINIMA)
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.4)
      message(FATAL_ERROR "Insufficient GCC version: 9.4+ required")
    endif()
  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 10.0)
      message(FATAL_ERROR "Insufficient clang version: 10.0+ required")
    endif()
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 10.0)
      message(FATAL_ERROR "Insufficient Apple clang version: XCode 10.0+ required")
    endif()
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 19.29)
      message(FATAL_ERROR "Insufficient MSVC version: Visual Studio 2019 16.10 (MSVC 19.29) or later is required")
    endif()
  else()
    message(FATAL_ERROR "Unknown / untested compiler ${CMAKE_CXX_COMPILER_ID}")
  endif()
endif()


# If no build type is selected, build optimized but retain debug
# info. This is probably the right build type for packaging and
# release.
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()


# Enforce the C++ standard, and disable extensions
if (NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED True)
if(MSVC)
  # https://discourse.cmake.org/t/set-cmake-cxx-standard-should-set-zc-cplusplus-for-msvc/1876
  string(APPEND CMAKE_CXX_FLAGS " /Zc:__cplusplus")
endif()
set(CMAKE_CXX_EXTENSIONS OFF)


# Produce a compilation database when generating for a build
# system that is able to make one.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Produce GNU-style variables for installation, and default the
# installation directory to be local to the build. If you intend to
# install elsewhere, pass an explicit argument to CMAKE_INSTALL_PREFIX
# on the command line:
#
# cmake ... -DCMAKE_INSTALL_PREFIX=$HOME/opt
#
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "..." FORCE)
endif()
include(GNUInstallDirs)

# Set the Cmake module path to include our cmake helper functions
set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${PROJECT_SOURCE_DIR}/src/viam/cmake
)

include(viamcppsdk_utils)



# Configure how the SDK will manage runtime paths. This is important
# even for BUILD_SHARED_LIBS=OFF due to the always-shared
# `libviam_rust_utils` shared library.
set(CMAKE_BUILD_WITH_INSTALL_RPATH false)
set(CMAKE_BUILD_RPATH_USE_ORIGIN true)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH true)
set(CMAKE_SKIP_BUILD_RPATH false)

list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}" isSystemDir)
if("${isSystemDir}" STREQUAL "-1")
  # TODO: Is there a way to do this with generator expressions?
  file(RELATIVE_PATH BINTOPREFIXRELPATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR} ${CMAKE_INSTALL_PREFIX})
  file(RELATIVE_PATH PREFIXTOLIBRELPATH ${CMAKE_INSTALL_PREFIX} ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})

  if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    list(PREPEND CMAKE_INSTALL_RPATH "@loader_path/${BINTOPREFIXRELPATH}/${PREFIXTOLIBRELPATH}")
  else()
    list(PREPEND CMAKE_INSTALL_RPATH "$ORIGIN/${BINTOPREFIXRELPATH}/${PREFIXTOLIBRELPATH}")
  endif()
endif()

# Everything needs threads, and prefer -pthread if available
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)


# See if the user provided a libviam_rust_utils in the root of the
# tree. If not, try to download the latest.
#
# TODO: Fixup runtime paths at install time.
# TODO: This should be pushed down into a tool
#
# TODO: Eventually, `rust_utils` should be its own package that can be
# pulled in via `find_package`, and this whole thing can be removed.
#
# TODO: When this is removed, also remove the
# `target_link_directories` call down in src/CMakeLists.txt, as it
# will no longer be needed.
set(viam_rust_utils_file ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}viam_rust_utils${CMAKE_STATIC_LIBRARY_SUFFIX})

file(GLOB viam_rust_utils_files ${PROJECT_SOURCE_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}viam_rust_utils*${CMAKE_STATIC_LIBRARY_SUFFIX})

if (viam_rust_utils_files)
  list(LENGTH viam_rust_utils_files num_viam_rust_utils_files)
  if (num_viam_rust_utils_files GREATER 1)
    message(FATAL_ERROR "Found multiple viam_rust_utils libraries in source tree root, cannot proceed")
  endif()
  FILE(COPY_FILE
    ${viam_rust_utils_files}
    ${viam_rust_utils_file}
    ONLY_IF_DIFFERENT
  )
elseif(NOT WIN32) # TODO(RSDK-10366): Currently, rust_utils is not published for windows, so don't even try downloading
  set(lvru_system_name ${CMAKE_SYSTEM_NAME})
  if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(lvru_system_name "macosx")
  endif()
  file(
    DOWNLOAD
    https://github.com/viamrobotics/rust-utils/releases/latest/download/${CMAKE_SHARED_LIBRARY_PREFIX}viam_rust_utils-${lvru_system_name}_${CMAKE_SYSTEM_PROCESSOR}${CMAKE_STATIC_LIBRARY_SUFFIX}
    ${viam_rust_utils_file}
    STATUS lvru_status
  )
  list(GET lvru_status 0 lvru_status_code)
  list(GET lvru_status 1 lvru_status_string)

  if(NOT lvru_status_code EQUAL 0)
    message(FATAL_ERROR "No local viam_rust_utils found and failed to download: ${lvru_status_string}")
  endif()

endif()

# TODO(RSDK-10366): Currently, rust_utils is not published for windows, so don't even declare the library
if (NOT WIN32)
  add_library(viam_rust_utils SHARED IMPORTED)

  target_link_directories(viam_rust_utils
    INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_LIBDIR}>"
  )

  set_property(TARGET viam_rust_utils PROPERTY IMPORTED_LOCATION ${viam_rust_utils_file})

  install(
    IMPORTED_RUNTIME_ARTIFACTS viam_rust_utils
    LIBRARY COMPONENT viam-cpp-sdk_runtime
  )
endif()

# Install the license file
install(FILES
  LICENSE
  DESTINATION ${CMAKE_INSTALL_DATADIR}/${CMAKE_PROJECT_NAME}
  COMPONENT viam-cpp-sdk_runtime
)


# Add `format` and `check-format` targets to run clang-format or just
# check for errors. This is not a great way to go about it, but it is
# enough to get started and match what the existing Makefile based
# setup did. A better way (perhaps) would be to add `clang-format` as
# an `IMPORT`ed executable and make this target dependent on it (and,
# probably, any .clang-format files). Then use CMake to find the
# sources rather than a shell script, and add them as dependencies so
# that the `format` target is a no-op if nothing is out of date.
add_custom_target(
  format
  COMMAND ${PROJECT_SOURCE_DIR}/bin/run-clang-format.sh
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_custom_target(
  check-format
  COMMAND ${PROJECT_SOURCE_DIR}/bin/run-clang-format.sh --dry-run -Werror
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  USES_TERMINAL
)


set(VIAMCPPSDK_BOOST_VERSION_MINIMUM 1.71)
set(VIAMCPPSDK_GRPC_VERSION_MINIMUM 1.30.2)
set(VIAMCPPSDK_PROTOBUF_VERSION_MINIMUM 3.12.4)
set(VIAMCPPSDK_XTL_VERSION_MINIMUM 0.7.2)
set(VIAMCPPSDK_XTENSOR_VERSION_MINIMUM 0.24.3)

# Time to find `BOOST`.
if (VIAMCPPSDK_BUILD_TESTS)
  set(VIAMCPPSDK_BOOST_TEST "unit_test_framework")
endif()

find_package(Boost ${VIAMCPPSDK_BOOST_VERSION_MINIMUM} REQUIRED COMPONENTS headers log program_options ${VIAMCPPSDK_BOOST_TEST})

# Time to find `protobuf` and `gRPC[++]`. Normally this would just be
# something like `find_package(gRPC <version> CONFIG REQUIRED)`, and
# I'd push that call down only into the directories that actually need
# it. However, that is currently 1) everything, and 2) it doesn't work
# on Bullseye because the version of proto/gRPC there doesn't actually
# install the .cmake files required to support `find_package`, so we
# need a fallback path to pick it up from pkg-config files
# instead. That logic is complex and I don't want to duplicate it into
# every place we pick up the dependencies. Hopefully this can all go
# away once our system of record is based on Bookworm.

# Set our required version to the Bullseye proto version of 3.12.4 and
# gRPC version of 1.30.2, but don't `REQUIRE` since we want to have
# another try via pkgconfig.

find_package(gRPC ${VIAMCPPSDK_GRPC_VERSION_MINIMUM} CONFIG)

# Protobuf has an idiosyncratic system for checking version compatibility, so we specify
# no version here and check it manually below.
find_package(Protobuf CONFIG)

if (gRPC_FOUND)
  get_target_property(VIAMCPPSDK_GRPC_CPP_PLUGIN gRPC::grpc_cpp_plugin LOCATION)
  set(VIAMCPPSDK_PROTOBUF_VERSION ${Protobuf_VERSION})

  # We need a good value for VIAMCPPSDK_PROTOBUF_VERSION in order to
  # interpolate it into our `pkgconfig` requirements. Make sure we
  # derived value for it.
  if (NOT VIAMCPPSDK_PROTOBUF_VERSION)
    message(FATAL_ERROR "Found gRPC with `find_package` but could not determine the `protobuf` version")
  endif()

  # If we picked up `protobuf` indirectly, make sure it meets our
  # version requirements.
  if (NOT VIAMCPPSDK_PROTOBUF_VERSION VERSION_GREATER_EQUAL ${VIAMCPPSDK_PROTOBUF_VERSION_MINIMUM})
    message(FATAL_ERROR "The `protobuf` package found indirectly by `find_package(gRPC...) is too old (<${VIAMCPPSDK_PROTOBUF_VERSION_MINIMUM})")
  endif()

  set(VIAMCPPSDK_GRPCXX_VERSION ${gRPC_VERSION})
  set(VIAMCPPSDK_GRPCXX_LIBRARIES gRPC::grpc++)
  set(VIAMCPPSDK_GRPCXX_REFLECTION_LIBRARIES gRPC::grpc++_reflection)
else()
  message(WARNING "Failed to find gRPC with `find_package`; falling back to `pkg_check_modules`")

  include(FindPkgConfig)

  pkg_check_modules(PROTOBUF REQUIRED protobuf>=${VIAMCPPSDK_PROTOBUF_VERSION_MINIMUM})
  pkg_check_modules(GRPC REQUIRED grpc>=${VIAMCPPSDK_GRPC_VERSION_MINIMUM})
  pkg_check_modules(GRPCXX REQUIRED grpc++>=${VIAMCPPSDK_GRPC_VERSION_MINIMUM})

  set(VIAMCPPSDK_PROTOBUF_VERSION ${PROTOBUF_VERSION})
  set(VIAMCPPSDK_PROTOBUF_INCLUDE_DIRS ${PROTOBUF_INCLUDE_DIRS})
  set(VIAMCPPSDK_PROTOBUF_LIBRARY_DIRS ${PROTOBUF_LIBRARY_DIRS})
  set(VIAMCPPSDK_PROTOBUF_LIBRARIES ${PROTOBUF_LIBRARIES})

  set(VIAMCPPSDK_GRPC_VERSION ${GRPC_VERSION})
  set(VIAMCPPSDK_GRPC_INCLUDE_DIRS ${GRPC_INCLUDE_DIRS})
  set(VIAMCPPSDK_GRPC_LIBRARY_DIRS ${GRPC_LIBRARY_DIRS})
  set(VIAMCPPSDK_GRPC_LIBRARIES ${GRPC_LIBRARIES})
  # TODO: This may be wrong. Consider providing a customization point
  # for users to override on the command line.
  set(VIAMCPPSDK_GRPC_CPP_PLUGIN ${GRPCXX_PREFIX}/bin/grpc_cpp_plugin)

  set(VIAMCPPSDK_GRPCXX_VERSION ${GRPCXX_VERSION})
  set(VIAMCPPSDK_GRPCXX_INCLUDE_DIRS ${GRPCXX_INCLUDE_DIRS})
  set(VIAMCPPSDK_GRPCXX_LIBRARY_DIRS ${GRPCXX_LIBRARY_DIRS})
  set(VIAMCPPSDK_GRPCXX_LIBRARIES ${GRPCXX_LIBRARIES})

  # We need to set the reflection library, which unfortunately does not
  # have a `.pc` file. To do so, we duplicate the `VIAMCPPSDK_GRPCXX_LIBRARIES`
  # value, assume its first element is the base dependency for GRPC, and
  # prepend our new list with a new value, equal to that base dependency plus
  # the `::grpc++_reflection` suffix for finding the reflection library.
  list(GET VIAMCPPSDK_GRPCXX_LIBRARIES 0 VIAMCPPSDK_GRPCXX_ROOT)
  set(VIAMCPPSDK_GRPCXX_REFLECTION_LIBRARIES ${VIAMCPPSDK_GRPCXX_LIBRARIES})
  set(VIAMCPPSDK_GRPCXX_REFLECTION_LIB ${VIAMCPPSDK_GRPCXX_ROOT}_reflection)
  list(PREPEND VIAMCPPSDK_GRPCXX_REFLECTION_LIBRARIES ${VIAMCPPSDK_GRPCXX_REFLECTION_LIB})
endif()

# Do this here unconditionally in case find_package didn't set the MAJOR/MINOR variables
string(REGEX REPLACE "^([0-9]+)\.([0-9]+)\.([0-9]+)$" "\\1" VIAMCPPSDK_PROTOBUF_VERSION_MAJOR ${VIAMCPPSDK_PROTOBUF_VERSION})
string(REGEX REPLACE "^([0-9]+)\.([0-9]+)\.([0-9]+)$" "\\2" VIAMCPPSDK_PROTOBUF_VERSION_MINOR ${VIAMCPPSDK_PROTOBUF_VERSION})
string(REGEX REPLACE "^([0-9]+)\.([0-9]+)\.([0-9]+)$" "\\3" VIAMCPPSDK_PROTOBUF_VERSION_PATCH ${VIAMCPPSDK_PROTOBUF_VERSION})

# The check below expects a version that looks like 20-something.x.y, so if
# we got a major version that doesn't look like that, synthesize one from the version we did get.
if (VIAMCPPSDK_PROTOBUF_VERSION_MAJOR GREATER 20)
  set(VIAMCPPSDK_PROTOC_VERSION "${VIAMCPPSDK_PROTOBUF_VERSION_MAJOR}.${VIAMCPPSDK_PROTOBUF_VERSION_MINOR}")
else()
  set(VIAMCPPSDK_PROTOC_VERSION "${VIAMCPPSDK_PROTOBUF_VERSION_MINOR}.${VIAMCPPSDK_PROTOBUF_VERSION_PATCH}")
endif()

# These are the minimum versions of gRPC and protobuf for which remote plugins with buf are supported.
if ((VIAMCPPSDK_GRPCXX_VERSION VERSION_GREATER 1.51.1) AND (VIAMCPPSDK_PROTOC_VERSION VERSION_GREATER 21.7.0))
  set(VIAMCPPSDK_BUF_REMOTE_PLUGIN_SUPPORTED 1)
endif()

if (VIAMCPPSDK_GRPCXX_VERSION VERSION_LESS 1.32.0)
  set(VIAMCPPSDK_GRPCXX_LEGACY_FWD 1)
endif()

include(FetchContent)

FetchContent_Declare(
  xtl
  GIT_REPOSITORY https://github.com/xtensor-stack/xtl.git
  GIT_TAG        e0f00666d90086bb245ae73abb6123d0e2c1b30b # 0.7.2
  FIND_PACKAGE_ARGS ${VIAMCPPSDK_XTL_VERSION_MINIMUM} CONFIG
)

FetchContent_Declare(
  xtensor
  GIT_REPOSITORY https://github.com/xtensor-stack/xtensor
  GIT_TAG        9e8662ddad7fcd1a96969b96bf8955e59cf44978 # 0.24.3
  FIND_PACKAGE_ARGS ${VIAMCPPSDK_XTENSOR_VERSION_MINIMUM} CONFIG
)

FetchContent_MakeAvailable(xtl xtensor)

# Pull in our subdirectories
add_subdirectory(src/viam)


# Define some custom targets to install different subsets of files.
#
# TODO: Can we programatically get the list of components?
# TODO: The `all` dependency here is too broad. Can it be narrowed?
add_custom_target(
  install-runtime
  COMMAND ${CMAKE_COMMAND} -DCOMPONENT=viam-cpp-sdk_runtime -P ${CMAKE_BINARY_DIR}/cmake_install.cmake
  DEPENDS all
)

add_custom_target(
  install-dev
  COMMAND ${CMAKE_COMMAND} -DCOMPONENT=viam-cpp-sdk_dev -P ${CMAKE_BINARY_DIR}/cmake_install.cmake
  DEPENDS all
)

if (VIAMCPPSDK_BUILD_EXAMPLES)
  add_custom_target(
    install-examples
    COMMAND ${CMAKE_COMMAND} -DCOMPONENT=viam-cpp-sdk_examples -P ${CMAKE_BINARY_DIR}/cmake_install.cmake
    DEPENDS all
  )
endif()
