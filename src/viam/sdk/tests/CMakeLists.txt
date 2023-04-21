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

enable_testing()
include(viamcppsdk_utils)
viamcppsdk_add_boost_test(test_generic_component.cpp)
viamcppsdk_add_boost_test(test_motor.cpp)
viamcppsdk_add_boost_test(test_camera.cpp)
viamcppsdk_add_boost_test(test_types.cpp)
