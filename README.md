# Viam C++ SDK

## (In)stability Notice

> **Warning** This is a alpha release of the Viam C++ SDK. Stability
> is not guaranteed. Breaking changes are likely to occur, and occur
> often.

## Resources
* [Documentation](https://cpp.viam.dev)
* [Examples](https://github.com/viamrobotics/viam-cpp-sdk/tree/main/src/viam/examples)

## Repository Layout
- `src/viam/sdk` The implementation of the C++ sdk library
- `src/viam/api` All the google and viam api autogen files
- `src/viam/examples` A list of examples

## Getting Started

NOTE: Please see `BUILDING.md` for instructions on how to use
CMake-based build system for this project.

## Using Docker for Development

A Docker description is included which will install the required
packages.
1. Create a new docker image from one of the provided docker
   files. From the root directory of this project (where this README
   is located), run `docker build -t cpp . -f
   etc/docker/Dockerfile.debian.bullseye` (other files are provided
   for Ubuntu Focal and Ubuntu Jammy as well as Debian Sid). Run the
   new image in a container that has your local project directory
   mounted to the filesystem. This will ensure that you can continue
   to develop locally on your machine, and all files will be
   automatically synced into the docker environment. To create the
   environment and open a shell to it, run the command `docker run
   --rm -it -v "$PWD":/usr/src/viam-cpp-sdk -w /usr/src/viam-cpp-sdk
   cpp /bin/bash`
1. Start developing! You can develop as you normally would, using the
   files on your local filesystem. When it comes time to
   testing/building/running the program, do so inside the docker
   environment you opened in the previous step.

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

Second, make sure the documentation directory exists. From the SDK root, run:
```
mkdir -p etc/docs/api/current
```

Finally, generate the documentation. From the SDK root, run:
```
doxygen Doxyfile
```

That will generate all documentation in the `etc/docs/api/current` folder. Then 
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

## License
Copyright 2022 Viam Inc.

Apache 2.0 - See [LICENSE](https://github.com/viamrobotics/viam-cpp-sdk/blob/main/LICENSE) file
