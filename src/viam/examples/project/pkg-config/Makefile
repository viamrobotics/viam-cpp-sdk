CXX = c++

CXXFLAGS = -std=c++14
CXXFLAGS += `pkg-config --cflags viam-cpp-sdk-libviamsdk`
LINKFLAGS = `pkg-config --libs viam-cpp-sdk-libviamsdk`

all: example_dial example_module

example_dial.o: ../../dial/example_dial.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^

example_dial: example_dial.o
	$(CXX) $(LINKFLAGS) -o $@ $^

example_module.o: ../../modules/example_module.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^

example_module: example_module.o
	$(CXX) $(LINKFLAGS) -o $@ $^
