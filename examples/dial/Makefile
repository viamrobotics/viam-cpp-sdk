## TODO(RSDK-803): stop relying on a relative path here
LDFLAGS = -L/usr/local/lib `pkg-config --libs protobuf grpc++ re2 libcares` \
-ldl -L../../../rust-utils/target/debug -lviam_rust_utils \
-ldl -L/usr/local/boost_1_80_0/stage/lib/ -lboost_log

GEN = ../../src/gen
CXX = g++
CXXFLAGS1 += -Wall -Wextra
CXXFLAGS += `pkg-config --cflags protobuf grpc`
CXXFLAGS += -std=c++11 -I $(GEN)/ -I $(GEN)/proto/ -I ../../src/robot/ -I ../../src/rpc/ -I ../../include/

all: example_dial 

example_dial: $(GEN)/google/api/http.pb.o $(GEN)/google/api/annotations.pb.o $(GEN)/robot/v1/robot.pb.o $(GEN)/robot/v1/robot.grpc.pb.o $(GEN)/common/v1/common.pb.o $(GEN)/common/v1/common.grpc.pb.o ../../src/robot/client.o ../../src/rpc/dial.o ../../src/components/resource_manager.o ../../src/components/component_base.o ../../src/common/utils.o ../../src/registry/registry.o ../../src/common/proto_type.o example_dial.o 
	$(CXX) $(CXXFLAGS1) $^ $(CXXFLAGS) $(LDFLAGS) -o $@ 

clean:
	rm -f example_dial *.o
