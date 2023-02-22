cmake_minimum_required(VERSION 3.25 FATAL_ERROR)


project(viam-cpp-sdk-example-project
  DESCRIPTION "Viam Robotics C++ SDK - CMake Example Project"
  HOMEPAGE_URL https://github.com/viamrobotics/viam-cpp-sdk
  LANGUAGES CXX
)


# Use of the C++ SDK requires C++14.
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)


# NOTE: You do not need to / should not do this part in your project!
#
# This is a convenience to connect this example to the default in-tree
# installation directory for the viam-cpp-sdk. If you have the SDK
# installed to another location, the below `find_package` is likely to
# fail. You can fix this by re-running cmake for this example as
# follows:
#
#   cmake -DCMAKE_PREFIX_PATH=/path/to/prefix/where/viam-cpp-sdk/was/installed ...
#
# For instance, if you have installed the viam-cpp-sdk to `HOME/opt`, then:
#
#   cmake -DCMAKE_PREFIX_PATH=$HOME/opt ...
#
if (NOT CMAKE_PREFIX_PATH)
  set(CMAKE_PREFIX_PATH '${PROJECT_SOURCE_DIR}/../../../build/install')
endif()


# In practice, you should set a minimum required version
# when loading the SDK:
#
# find_package(viam-cpp-sdk 1.0.2 CONFIG_REQUIRED COMPONENTS Lib)
#
find_package(viam-cpp-sdk CONFIG REQUIRED COMPONENTS Lib)


# Here we compile the `examle_dial.cpp` and `example_module.cpp` source
# files into an executable, but as part of a separate project rather than
# being built-in to the SDK build.
add_executable(example_dial
  ../../dial/example_dial.cpp
)

add_executable(example_module
  ../../modules/example_module.cpp)

# Declaring the link dependency on the viam-cpp-sdk::viamcpp library
# will automatically wire up the required include paths and transitive
# library dependencies.
#
# NOTE: Except for gRPC dependencies if the SDK's gRPC dependency
# detection fell back on using pkg-config. Hopefully that isn't the
# case but, if it is, you may need to explicitly include
# `find_package` calls for it and add the necessary dependencies
# yourself.
#
target_link_libraries(example_dial
  viam-cpp-sdk::viamcpp
)

target_link_libraries(example_module
  viam-cpp-sdk::viamcpp
)
