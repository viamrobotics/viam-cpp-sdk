## TODO(RSDK-803): stop relying on a relative path here
LDFLAGS = -L/usr/local/lib `pkg-config --libs protobuf grpc++ re2 libcares`\
-ldl -L../rust-utils/target/debug -lviam_rust_utils

CXX = g++
CXXFLAGS += `pkg-config --cflags protobuf grpc`
CXXFLAGS += -std=c++11 -I src/gen/ -I src/gen/proto/

all: buf

buf: buf-clean
	@mkdir -p src/gen
	buf generate buf.build/viamrobotics/goutils --template buf.gen.yaml
	buf generate buf.build/googleapis/googleapis --template buf.gen.yaml --path google/rpc --path google/api
	buf generate buf.build/viamrobotics/api --template buf.gen.yaml --path common,component,robot,service,module

buf-clean:
	rm -rf src/gen/*

clean:
	find ./src -name '*.o' | xargs rm -rf

format:
	find ./src -not -path "*/gen/*" -type f \( -name \*.cpp -o -name \*.hpp \) | xargs clang-format -style=file -i -fallback-style=none
