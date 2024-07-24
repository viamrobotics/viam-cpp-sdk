CXX = c++

VIAM_PREFIX ?= ../../../../../build/install
PKG_CONFIG_PATH ?= $(VIAM_PREFIX)/lib/pkgconfig


CXXFLAGS = -std=c++17
CXXFLAGS += `PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --cflags viam-cpp-sdk-libviamsdk`
LINKFLAGS = `PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --libs viam-cpp-sdk-libviamsdk`
LDFLAGS ="-Wl,-rpath,$(VIAM_PREFIX)/"

all: example_dial example_module

example_dial.o: ../../dial/example_dial.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^

example_dial: example_dial.o
	$(CXX) $(LINKFLAGS) $(LDFLAGS) -o $@ $^

example_module.o: ../../modules/simple/main.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^

example_module: example_module.o
	$(CXX) $(LINKFLAGS) $(LDFLAGS) -o $@ $^
