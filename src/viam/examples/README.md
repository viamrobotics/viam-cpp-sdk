# Component & Service Examples
Examples: 
 - camera
 - motor
 
These examples demonstrate basic functionality of Viam components and services while connecting to a local Viam server instance. 
If you wish to build & run these examples:

Download and build the RDK by following the instructions [here](https://github.com/viamrobotics/rdk#building-and-using).

Then run:
``` shell
[viam-server-command] --config viam-cpp-sdk/src/viam/examples/[component name]/example_config.json
```
This will setup a server running on `localhost:8080` that has a mock setup of the example component. Leave this running. Now we can use the C++ sdk to connect to that component and perform some action.

Download and build the C++ SDK by following the instructions [here](https://github.com/viamrobotics/viam-cpp-sdk#getting-started).

Then run: 
``` shell
viam-cpp-sdk/build/viam/examples/[component name]/example_[component name]
```



# Generic Examples

## Project example
This example shows how to setup a simple CMake-based project that uses Viam's C++ SDK. 

## Module example
This example goes through how to create custom modular resources using Viam's C++ SDK, and how to connect it to a Robot.

## Dial Example

If you are connecting to a robot with authentication you will need to
add credentials. Update path code :

``` c++
void *ptr = init_rust_runtime();
char *path = dial("<your robot uri here>", "<your authentication type>", "<your authentication payload>", false, ptr);
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

If you want to run this locally, you can download and build the C++ SDK by following the instructions [here](https://github.com/viamrobotics/viam-cpp-sdk#getting-started).

Then run:

``` shell
./build/src/viam/examples/dial/example_dial
```

If you want to connect to a robot without credentials then just simply
pass a null pointer for the credentials.

Note also that this will attempt to connect over webRTC by default. To
override and only use direct gRPC calls, you will need to use a
`.local` address for your robot's uri,
e.g. `name.xxxx.local.viam.cloud:8080` instead of
`name.xxxx.viam.cloud`.
