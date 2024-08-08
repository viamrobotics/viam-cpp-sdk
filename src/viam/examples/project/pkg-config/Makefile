CXX = c++

# Technically the SDK supports building with c++14, but note that more recent
# versions of abseil will force c++17, so this may be needed to make things
# work out of the box.
CXX_STANDARD ?= c++17

CXXFLAGS += -std=$(CXX_STANDARD)
CXXFLAGS += `pkg-config --cflags viam-cpp-sdk-libviamsdk`
LDFLAGS += `pkg-config --libs viam-cpp-sdk-libviamsdk`

all: example_dial example_module

example_dial.o: ../../dial/example_dial.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^

example_dial: example_dial.o
<<<<<<< HEAD
	$(CXX) $(LDFLAGS) -o $@ $^
=======
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^
>>>>>>> 5c72e224a1cec7e22a2e5ec003f7c13718bfef77

example_module.o: ../../modules/simple/main.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^

example_module: example_module.o
<<<<<<< HEAD
	$(CXX) $(LDFLAGS) -o $@ $^
=======
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^
>>>>>>> 5c72e224a1cec7e22a2e5ec003f7c13718bfef77

clean:
	rm -f *.o example_dial example_module
