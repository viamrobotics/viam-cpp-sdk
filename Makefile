## TODO(RSDK-803): stop relying on a relative path here
LDFLAGS = -L/usr/local/lib `pkg-config --libs protobuf grpc++ re2 libcares`\
-ldl -L../rust-utils/target/debug -lviam_rust_utils

CXX = g++
CXXFLAGS += `pkg-config --cflags protobuf grpc`
CXXFLAGS += -std=c++11 -I src/gen/ -I src/gen/proto/

all: buf 

buf:	buf-clean
	@mkdir -p src/gen
	buf generate buf.build/viamrobotics/goutils --template buf.gen.yaml 
	buf generate buf.build/googleapis/googleapis --template buf.gen.yaml --path google/rpc --path google/api 
	buf generate buf.build/viamrobotics/api --template buf.gen.yaml
	#ls src/gen/robot/v1/*.grpc.pb.cc | while read l; do mockgen -source="$$l" -destination=src/gen/robot/mock_v1/mock_`basename "$$l"`; done
buf-clean:
	rm -rf src/gen/*
