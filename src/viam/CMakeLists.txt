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

add_subdirectory(api)
add_subdirectory(sdk)
add_subdirectory(examples EXCLUDE_FROM_ALL)

# Generate CMake configs to enable importing this project
# into others via `find_package`.
include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/config/viam-cpp-sdkConfig.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/config/viam-cpp-sdkConfig.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/viam-cpp-sdk
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/config/viam-cpp-sdkConfigVersion.cmake"
  VERSION "${version}"
  COMPATIBILITY SameMajorVersion
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/config/viam-cpp-sdkConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/config/viam-cpp-sdkConfigVersion.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/viam-cpp-sdk
  COMPONENT viam-cpp-sdk_dev
)
