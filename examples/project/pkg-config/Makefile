CXX = c++

CXXFLAGS = -std=c++14
CXXFLAGS += `pkg-config --cflags viam-cpp-sdk-libviamcpp`
LINKFLAGS = `pkg-config --libs viam-cpp-sdk-libviamcpp`

all: example_dial

example_dial.o: ../../dial/example_dial.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $^

example_dial: example_dial.o
	$(CXX) $(LINKFLAGS) -o $@ $^
