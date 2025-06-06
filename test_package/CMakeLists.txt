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


# Everything needs threads, and prefer -pthread if available
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)


# In practice, you should set a minimum required version
# when loading the SDK:
#
# find_package(viam-cpp-sdk 1.0.2 CONFIG REQUIRED COMPONENTS viamsdk)
#
find_package(viam-cpp-sdk CONFIG REQUIRED viamsdk)

add_executable(example_module main.cpp)

target_link_libraries(example_module
  viam-cpp-sdk::viamsdk
)
