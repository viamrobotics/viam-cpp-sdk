#!/bin/bash
set -e

INSTALL_DIR="$(pwd)/install"
BUILD_SHARED=$(grep 'BUILD_SHARED_LIBS' CMakeCache.txt | cut -d '=' -f 2)

pushd src/viam/sdk/tests
UBSAN_OPTIONS="print_stacktrace=1" ctest --output-on-failure
popd
pushd src/viam/examples/modules/complex
UBSAN_OPTIONS="print_stacktrace=1" ctest --output-on-failure
popd

# Test that example_module builds and runs with the SDK install from above.
# Check with both CMake and make/pkg-config that we can build the example
# and have it start listening.

run_module() {
	./example_module fake-socket-path > module-out-temp.txt &
	MODULE_PID=$!
	sleep 2
	kill ${MODULE_PID}
	grep 'Module listening' module-out-temp.txt
	return $?
}

cd ../src/viam/examples/project
pushd cmake
cmake . -G Ninja # Just do an in-source build to save path fiddling
ninja all
run_module
popd

if [ ${BUILD_SHARED} = "ON" ]
then
	pushd pkg-config
	PKG_CONFIG_PATH=${INSTALL_DIR}/lib/pkgconfig CXXFLAGS="-DBOOST_LOG_DYN_LINK" make all
	run_module
	popd
fi
