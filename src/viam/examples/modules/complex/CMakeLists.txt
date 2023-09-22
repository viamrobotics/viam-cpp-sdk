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

add_executable(complex_module)
target_sources(complex_module
  PRIVATE
    main.cpp
    base/impl.cpp
    base/impl.hpp
    gizmo/impl.cpp
    gizmo/impl.hpp
    gizmo/api.cpp
    gizmo/api.hpp
    summation/api.cpp
    summation/api.hpp
    summation/impl.cpp
    summation/impl.hpp

    proto/gizmo.grpc.pb.cc
    proto/gizmo.grpc.pb.h
    proto/gizmo.pb.cc
    proto/gizmo.pb.h
    proto/summation.grpc.pb.cc
    proto/summation.grpc.pb.h
    proto/summation.pb.cc
    proto/summation.pb.h
)

add_executable(complex_module_client)
target_sources(complex_module_client
  PRIVATE
    client.cpp
    gizmo/api.hpp
    gizmo/api.cpp
    summation/api.hpp
    summation/api.cpp

    proto/gizmo.grpc.pb.cc
    proto/gizmo.grpc.pb.h
    proto/gizmo.pb.cc
    proto/gizmo.pb.h
    proto/summation.grpc.pb.cc
    proto/summation.grpc.pb.h
    proto/summation.pb.cc
    proto/summation.pb.h
)

target_link_libraries(complex_module
  PRIVATE Threads::Threads
  viam-cpp-sdk::viamsdk
)

target_link_libraries(complex_module_client
  viam-cpp-sdk::viamsdk
)

install(
  TARGETS complex_module
  COMPONENT examples
)

install(
  TARGETS complex_module_client
  COMPONENT examples
)
