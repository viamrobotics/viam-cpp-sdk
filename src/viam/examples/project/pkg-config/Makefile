CXX = c++

# The SDK requires C++20.
CXX_STANDARD ?= c++20

CXXFLAGS += -std=$(CXX_STANDARD)
CXXFLAGS += `pkg-config --cflags viam-cpp-sdk-libviamsdk`
LDLIBS += `pkg-config --libs-only-l viam-cpp-sdk-libviamsdk`
LDFLAGS += `pkg-config --libs-only-L --libs-only-other viam-cpp-sdk-libviamsdk`

all: example_dial example_module

example_dial.o: ../../dial/example_dial.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^

example_dial: example_dial.o
	$(CXX) $(CXXFLAGS)  -o $@ $^ $(LDFLAGS) $(LDLIBS)

example_module.o: ../../modules/simple/main.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^

example_module: example_module.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

clean:
	rm -f *.o example_dial example_module
