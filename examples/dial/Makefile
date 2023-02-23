## TODO(RSDK-803): stop relying on a relative path here
LDFLAGS = -L/usr/local/lib `pkg-config --libs protobuf grpc++ re2 libcares` \
-ldl -lviam_rust_utils \
-ldl -lboost_log

GEN = ../../src/gen
CXX = g++
CXXFLAGS1 += -Wall -Wextra
CXXFLAGS += `pkg-config --cflags protobuf grpc`
CXXFLAGS += -std=c++11 -I $(GEN)/ -I $(GEN)/proto/ -I ../../src/robot/ -I ../../src/rpc/ -I ../../src/

all: example_dial 

example_dial: $(GEN)/component/arm/v1/arm.pb.o $(GEN)/component/arm/v1/arm.grpc.pb.o $(GEN)/tagger/v1/tagger.pb.o $(GEN)/module/v1/module.grpc.pb.o $(GEN)/module/v1/module.pb.o $(GEN)/app/v1/app.pb.o $(GEN)/app/v1/robot.pb.o $(GEN)/google/api/http.pb.o $(GEN)/google/api/annotations.pb.o $(GEN)/robot/v1/robot.pb.o $(GEN)/robot/v1/robot.grpc.pb.o $(GEN)/common/v1/common.pb.o $(GEN)/common/v1/common.grpc.pb.o ../../src/robot/client.o ../../src/rpc/dial.o ../../src/components/resource_manager.o ../../src/components/component_base.o ../../src/common/utils.o ../../src/registry/registry.o ../../src/common/proto_type.o ../../src/services/service_base.o ../../src/resource/resource_base.o ../../src/resource/resource.o ../../src/module/manager.o ../../src/module/module.o ../../src/module/client.o ../../src/module/handler_map.o ../../src/subtype/subtype.o ../../src/config/resource.o ../../src/referenceframe/frame.o ../../src/spatialmath/geometry.o ../../src/spatialmath/orientation.o ../../src/spatialmath/orientation_types.o ../../src/spatialmath/pose.o example_dial.o ../../src/components/generic.o
	$(CXX) $(CXXFLAGS1) $^ $(CXXFLAGS) $(LDFLAGS) -o $@ 

clean:
	rm -f example_dial *.o
