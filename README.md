# Viam C++ SDK

## Resources
* [Documentation](https://cpp.viam.dev)
* [Examples](https://github.com/viamrobotics/viam-cpp-sdk/tree/main/src/viam/examples)

## Repository Layout
- `src/viam/sdk` The implementation of the C++ sdk library
- `src/viam/api` All the google and viam api autogen files
- `src/viam/examples` A list of examples

## Getting Started

Please see [BUILDING.md](https://github.com/viamrobotics/viam-cpp-sdk/blob/main/BUILDING.md) for instructions on how to use CMake-based build system for this project.

NOTE: Windows is not supported. If you are using Windows, build the project in WSL.

## Using Docker for Development

Some Dockerfiles are provided for C++ SDK development, either for developing
on the SDK itself, or for module development. The docker images are split up
into base images, found in [etc/docker/base-images](etc/docker/base-images), and
an [SDK build image](etc/docker/Dockerfile.sdk-build). The base images install
apt packages required to build and develop the SDK, whereas the SDK build image
is meant to be built on top of a base image using multi-stage Docker building.

From the root of this repository, run
```shell
docker build -t base/bullseye -f etc/docker/base-images/Dockerfile.debian.bullseye .
```
This will create a Debian Bullseye base image. Note the use of the `-t base/bullseye`
arg to assign a tag to the image, which is important for the next step. You can then
use `Dockerfile.sdk-build` in a couple different ways.

```shell
docker build --build-arg BASE_TAG=base/bullseye --build-arg GIT_TAG=[...] -f etc/docker/Dockerfile.sdk-build .
```

This will use `base/bullseye` as a base to build the SDK version provided in `GIT_TAG`,
which should be a tagged release version. The SDK will be cloned from
https://github.com/viamrobotics/viam-cpp-sdk/. This is the recommended approach for
C++ module development, which should generally be done against a tagged release.

You can also do
```shell
docker build --build-arg BASE_TAG=base/bullseye --build-arg REPO_SETUP=copy -f etc/docker/Dockerfile.sdk-build .
```

Note the use of the build argument `REPO_SETUP=copy`, which adds a Docker instruction
to copy the SDK repo from the current working directory, rather than cloning from
GitHub. This approach may make more sense for developing on the SDK itself, or if
your C++ SDK development relies on a localversion of the SDK.

The examples above illustrated the use of several `--build-arg` arguments, namely
`BASE_TAG`, `GIT_TAG`, and `REPO_SETUP`. Please see
[Dockerfile.sdk-build](etc/docker/Dockerfile.sdk-build) for a complete account of
all build arguments and their defaults.

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

## License
Copyright 2022 Viam Inc.

Apache 2.0 - See [LICENSE](https://github.com/viamrobotics/viam-cpp-sdk/blob/main/LICENSE) file
