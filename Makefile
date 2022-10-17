LDFLAGS = -L/usr/local/lib `pkg-config --libs protobuf grpc++ re2 libcares`\
-ldl -L../../../../target/debug -lviam_rust_utils

CXX = g++
CXXFLAGS += `pkg-config --cflags protobuf grpc`
CXXFLAGS += -std=c++11 -I gen/ -I gen/proto/

all: buf 

buf:	buf-clean
	@mkdir -p gen
	buf generate buf.build/viamrobotics/goutils --template buf.gen.yaml
	buf generate buf.build/googleapis/googleapis --template buf.gen.yaml --path google/rpc --path google/api 
	buf generate buf.build/viamrobotics/api --template buf.gen.yaml
buf-clean:
	rm -rf src/gen/*
