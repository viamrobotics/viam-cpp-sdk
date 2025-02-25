#!/bin/sh

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

# Check if clang-format is installed
if ! command -v clang-format >/dev/null 2>&1; then
    echo "Error: clang-format is not installed"
    echo "Please install it using: brew install clang-format"
    exit 1
fi

find ./src -not -path "./src/viam/api" -type f \( -name \*.cpp -o -name \*.hpp \) | xargs clang-format -style=file -i -fallback-style=none "$@"
