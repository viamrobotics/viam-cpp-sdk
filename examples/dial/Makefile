## TODO(RSDK-803): stop relying on a relative path here
LDFLAGS = -L/usr/local/lib `pkg-config --libs protobuf grpc++ re2 libcares`\
-ldl -L../../../rust-utils/target/debug -lviam_rust_utils

GEN = ../../src/gen
CXX = g++
CXXFLAGS += `pkg-config --cflags protobuf grpc`
CXXFLAGS += -std=c++11 -I $(GEN)/ -I $(GEN)/proto/

all: example_dial 

example_dial: $(GEN)/google/api/http.pb.o $(GEN)/google/api/annotations.pb.o $(GEN)/robot/v1/robot.pb.o $(GEN)/robot/v1/robot.grpc.pb.o $(GEN)/common/v1/common.pb.o $(GEN)/common/v1/common.grpc.pb.o example_dial.o
	$(CXX) $^ $(CXXFLAGS) $(LDFLAGS) -o $@

clean:
	rm -f example_dial *.o
