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

include(CheckIncludeFile)

add_executable(example_module
  example_module.cpp
)

target_link_libraries(example_module
  PRIVATE Threads::Threads
  viam-cpp-sdk::viamsdk
)

install(
  TARGETS example_module
  COMPONENT examples
)


add_executable(example_mlmodel
  example_mlmodel.cpp
)

target_link_libraries(example_mlmodel
  PRIVATE Threads::Threads
  viam-cpp-sdk::viamsdk
)

install(
  TARGETS example_mlmodel
  COMPONENT examples
)

find_path(TFLITE_INCLUDE_PATH "tensorflow/lite/c/c_api.h")
find_library(TFLITE_LIBRARY "tensorflowlite_c")
if (EXISTS ${TFLITE_INCLUDE_PATH} AND EXISTS ${TFLITE_LIBRARY})
  add_executable(mlmodelservice_tflite
    mlmodelservice_tflite.cpp
  )

  target_link_libraries(mlmodelservice_tflite
    PRIVATE Threads::Threads
    PRIVATE viam-cpp-sdk::viamsdk
    PRIVATE tensorflowlite_c
  )

  install(
    TARGETS mlmodelservice_tflite
    COMPONENT examples
  )
else()
  message(WARNING "Failed to find tflite C headers/library: tflite example module will not be built")
  message(WARNING "If you have tflite in a non-standard location, add -DCMAKE_PREFIX_PATH=/path/to/tflite to your CMake invocation")
endif()
