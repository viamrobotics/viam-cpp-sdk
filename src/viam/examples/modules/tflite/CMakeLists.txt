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

option(VIAMCPPSDK_BUILD_TFLITE_EXAMPLE_MODULE "Select whether to build the tflite module (builds tflite too!)" OFF)

if (VIAMCPPSDK_BUILD_TFLITE_EXAMPLE_MODULE)

  # In theory, this should make the change of `BUILD_SHARED_LIBS`
  # scoped just to the tensorflow build. We always want to build
  # tensorflow statically. If it is built dynamically, then we need to
  # install it. However, that requires building with
  # `TFLITE_ENABLE_INSTALL` in order to ensure that the dynamic
  # libraries get installed to the instalation root, which seems to
  # just not really work. Yes, this feels bad.
  function(fetchcontent_tflite)
    set(BUILD_SHARED_LIBS OFF)
    set(TFLITE_C_BUILD_SHARED_LIBS OFF)
    FetchContent_Declare(
      tensorflow
      GIT_REPOSITORY https://github.com/tensorflow/tensorflow.git
      GIT_PROGRESS TRUE
      GIT_TAG v2.13.0
      SOURCE_SUBDIR tensorflow/lite/c
    )
    FetchContent_MakeAvailable(tensorflow)
  endfunction()
  fetchcontent_tflite()

  add_executable(tflite_module
    main.cpp
  )

  target_link_libraries(tflite_module
    PRIVATE Threads::Threads
    PRIVATE viam-cpp-sdk::viamsdk
    PRIVATE tensorflowlite_c
  )

  install(
    TARGETS tflite_module
    COMPONENT examples
  )

endif()
