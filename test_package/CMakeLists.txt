cmake_minimum_required(VERSION 3.25 FATAL_ERROR)


project(viam-cpp-sdk-example-conan-project
  DESCRIPTION "Viam Robotics C++ SDK - CMake + Conan Example Project"
  HOMEPAGE_URL https://github.com/viamrobotics/viam-cpp-sdk
  LANGUAGES CXX
)


# Use of the C++ SDK requires C++14.
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Everything needs threads, and prefer -pthread if available
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)


# In practice, you should set a minimum required version
# when loading the SDK:
#
# find_package(viam-cpp-sdk 1.0.2 CONFIG REQUIRED COMPONENTS viamsdk)
#
find_package(viam-cpp-sdk CONFIG REQUIRED COMPONENTS viamsdk)


# Here we compile the `examle_dial.cpp` and `example_module.cpp` source
# files into executables, but as parts of separate projects rather than
# being built-in to the SDK build.
add_executable(example_dial
  ${PROJECT_SOURCE_DIR}/../src/viam/examples/dial/example_dial.cpp
)

add_executable(example_module
  ${PROJECT_SOURCE_DIR}/../src/viam/examples/modules/simple/main.cpp)

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
  viam-cpp-sdk::viamsdk
)

target_link_libraries(example_module
  viam-cpp-sdk::viamsdk
)
