# Viam C++ SDK

## (In)stability Notice

> **Warning**
> This is a beta release of the Viam C++ SDK. Stability is not guaranteed. Breaking changes are likely to occur, and occur often.

## Repository Layout
- `src` The implementation of the C++ sdk library
- `src/gen` All the google and viam api autogen files
- `examples` A list of examples

## Getting Started
First you may want to build the SDK library, to do so navigate to the root of the repository and run `make build` this should pull all the dependencies and hopefully build the library!
Lastly you can run one of the examples by navigation to the examples/{dial|echo} folder and run `make example_{dial|echo} && ./example_{dial|echo}`

### Acquiring the Rust-Utils Binary
In order to connect over webRTC, you will need the [rust-utils](https://github.com/viamrobotics/rust-utils) binary. Detailed install instructions to come.

## Usage
Instructions for running examples follows. More robust instructions for setting up a project to come.

### Echo Streaming Example
The echo example communicates with the goutils sample server. It demonstrates individual, streamed, and bidirectional communication. To test, navigate to your goutils clone and run

``` shell
go run rpc/examples/echo/server/cmd/main.go
```

Then, from the `examples/echo` directory, run 

``` shell
make example_echo && ./example_echo
```

You can run `make clean` when done testing to clean up.

### Dial Example

If you are connecting to a robot with authentication you will need to add credentials. Update path code :

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
make example_dial && ./example_dial
```

If you want to connect to a robot without credentials then just simply pass a null pointer for the credentials.

Note also that this will attempt to connect over webRTC by default. To override and only use direct gRPC calls, you will need to use a `.local` address for your robot's uri, e.g. `name.xxxx.local.viam.cloud:8080` instead of `name.xxxx.viam.cloud`.

## A Note on Connectivity and webRTC Functionality

The rust webRTC implementation (which C++ relies on via rust's foreign function interface) is still new, and liable to have bugs. At a minimum, we expect that calls to `ShellService::shell()` have a high likelihood of strange behavior. If you encounter any issues with streaming requests over webRTC, direct dial (by disabling webrtc as above) should resolve them. And please file a bug report! We will endeavor to be as responsive as possible, and resolve issues as quickly as possible.

## License 
Copyright 2022 Viam Inc.

Apache 2.0 - See [LICENSE](https://github.com/viamrobotics/viam-cpp-sdk/blob/main/LICENSE) file
