# Viam C++ SDK

## (In)stability Notice

> **Warning** This is a alpha release of the Viam C++ SDK. Stability
> is not guaranteed. Breaking changes are likely to occur, and occur
> often.

## Repository Layout
- `src` The implementation of the C++ sdk library
- `src/gen` All the google and viam api autogen files
- `examples` A list of examples

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

## Usage
Instructions for running examples follows. More robust instructions
for setting up a project to come.

### Echo Streaming Example
The echo example communicates with the goutils sample server. It
demonstrates individual, streamed, and bidirectional communication. To
test, navigate to your goutils clone and run

``` shell
go run rpc/examples/echo/server/cmd/main.go
```

Then, run the `example_echo` program (it will be in the `bin`
directory of your SDK installation)

``` shell
ninja install && ./build/install/bin/example_echo
```

### Dial Example

If you are connecting to a robot with authentication you will need to
add credentials. Update path code :

``` c++
void *ptr = init_rust_runtime();
char *path = dial("<your robot uri here>", "<your robot credentials here>", false, ptr);
```

Then to obtain a robot client do :

``` c++
std::string address("unix://");
address += path;
RobotServiceClient client(grpc::CreateChannel(address, grpc::InsecureChannelCredentials()));
```

Now if we want to get the metadata of the robots we just have to do :

``` c++
ResourceNamesRequest req;
ResourceNamesResponse resp;
ClientContext context;

Status status = stub_->ResourceNames(&context, req, &resp);
if (!status.ok()) {
  return;
}

for (auto i = 0; i < resp.resource_size(); i++) {
  std::cout << "Resource " << i << " " << resp.resources(i).type() << std::endl;
}
```

Then simply run

``` shell
ninja install && ./build/install/bin/example_dial
```

If you want to connect to a robot without credentials then just simply
pass a null pointer for the credentials.

Note also that this will attempt to connect over webRTC by default. To
override and only use direct gRPC calls, you will need to use a
`.local` address for your robot's uri,
e.g. `name.xxxx.local.viam.cloud:8080` instead of
`name.xxxx.viam.cloud`.

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
