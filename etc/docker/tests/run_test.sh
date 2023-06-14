#!/bin/bash
mkdir build
cd build
cmake .. -G Ninja -DVIAMCPPSDK_USE_DYNAMIC_PROTOS=ON \
		-DVIAMCPPSDK_OFFLINE_PROTO_GENERATION=ON \
		-DVIAMCPPSDK_SANITIZED_BUILD=ON \
		-DVIAMCPPSDK_CLANG_TIDY=ON
ninja all
ninja install
cd src/viam/sdk/tests
UBSAN_OPTIONS="print_stacktrace=1" ctest --output-on-failure
