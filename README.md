# Viam C++ SDK

## Resources
* [Documentation](https://cpp.viam.dev)
* [Examples](https://github.com/viamrobotics/viam-cpp-sdk/tree/main/src/viam/examples)

## Repository Layout
- `src/viam/sdk` The implementation of the C++ sdk library
- `src/viam/api` All the google and viam api autogen files
- `src/viam/examples` A list of examples

## Getting Started

Please see [BUILDING.md](doc/BUILDING.md) for instructions on how to use CMake-based build system for this project.

## Building Documentation Locally for Testing
The C++ sdk uses [Doxygen](https://www.doxygen.nl/) to generate documentation.
An automated workflow will generate and update our documentation on each merge,
and publish it to [cpp.viam.dev](https://cpp.viam.dev).

Generating documentation locally to observe changes while developing with the
C++ SDK is simple.
First, make sure doxygen is installed, e.g.,
```
(on mac) brew install doxygen
(on linux) apt-get install -y doxygen
```

Next, generate the documentation. From the SDK root, run:
```
./bin/build-docs.sh
```

That will run `doxygen Doxyfile` and create the `etc/docs/api/current` folder. Then
simply open the `index.html` file in your preferred browser to view the documentation.

## A Note on Connectivity and webRTC Functionality

The rust webRTC implementation (which C++ relies on via rust's foreign
function interface) is still new, and liable to have bugs. At a
minimum, we expect that calls to `ShellService::shell()` have a high
likelihood of strange behavior. If you encounter any issues with
streaming requests over webRTC, direct dial (by disabling webrtc as
above) should resolve them. And please file a bug report! We will
endeavor to be as responsive as possible, and resolve issues as
quickly as possible.

## A note on logging

Users should only interact with logging via the macros, classes, and functions in
[`viam/sdk/log/logging.hpp`](src/viam/sdk/log/logging.hpp). Logging is
implemented using Boost.Log, but this is an implementation detail subject
to change without warning. In particular, using Boost.Log macros such as
`BOOST_LOG_TRIVIAL` or `BOOST_LOG_SEV` is undefined behavior which will likely
fail to output log messages.

## Running Tests
Tests for the SDK are located in `src/viam/sdk/tests`. The CMakeLists.txt file in that directory defines how to build them. When the SDK is built, the test executables are placed in the test folder within your specified build directory (e.g., `build`, if you followed the instructions in [`BUILDING.md`](https://github.com/viamrobotics/viam-cpp-sdk/blob/main/BUILDING.md)). The test executable files can be run individually. To run the entire test suite at once, navigate to the `tests` folder in your build directory and run:
```
ctest
```
Or to avoid navigating all the way to the folder you can specify the test directory, for example:
```
ctest --test-dir build/src/viam/sdk/tests/
```
Additionally, for more useful ctest options explore:
```
ctest --help
```

## License
Copyright 2022 Viam Inc.

Apache 2.0 - See [LICENSE](https://github.com/viamrobotics/viam-cpp-sdk/blob/main/LICENSE) file
