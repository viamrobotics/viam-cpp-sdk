#!/bin/bash
set -e

mkdir build
cd build
cmake .. -G Ninja -DVIAMCPPSDK_USE_DYNAMIC_PROTOS=ON \
		-DVIAMCPPSDK_OFFLINE_PROTO_GENERATION=ON \
		-DVIAMCPPSDK_SANITIZED_BUILD=ON \
		-DVIAMCPPSDK_CLANG_TIDY=ON
ninja all
ninja install
INSTALL_DIR="$(pwd)/install"

# pushd src/viam/sdk/tests
# UBSAN_OPTIONS="print_stacktrace=1" ctest --output-on-failure
# popd
# pushd src/viam/examples/modules/complex
# UBSAN_OPTIONS="print_stacktrace=1" ctest --output-on-failure
# popd

# In the examples below, we check that example_module exits with a status code of 1, 
# the expected error for not providing a socket address on the command line.

cd ../src/viam/examples/project
pushd cmake
cmake . -G Ninja # Just do an in-source build to save path fiddling
ninja all
(set +e; ./example_module; [ $? -eq 1 ])
popd
pushd pkg-config
PKG_CONFIG_PATH=${INSTALL_DIR}/lib/pkgconfig make all
(set +e; ./example_module; [ $? -eq 1 ])
popd
