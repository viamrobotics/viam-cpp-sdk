mkdir build
cd build
pwd
cmake .. -G Ninja -DVIAMCPPSDK_USE_DYNAMIC_PROTOS=ON -DVIAMCPPSDK_OFFLINE_PROTO_GENERATION=ON
ninja all
ninja install
cd src/tests
ctest
