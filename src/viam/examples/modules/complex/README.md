# VIAM Complex Module Example
This example goes through how to create custom modular resources using Viam's C++ SDK and how to connect them to a Robot.

This is a limited document. For a more in-depth understanding of modules, see the [documentation](https://docs.viam.com/registry/).

## Purpose
Modular resources allow you to define custom components and services, and add them to your robot. Viam ships with many component types, but you're not limited to only using those types -- you can create your own using modules.

For more information, see the [documentation](https://docs.viam.com/registry/). For a simpler example, take a look at the [simple module example](https://github.com/viamrobotics/viam-cpp-sdk/tree/main/src/viam/examples/modules/simple), which only contains one custom resource model in one file.

For a fully fleshed-out example of a C++ module that uses Github CI to upload to the Viam Registry, take a look at [module-example-cpp](https://github.com/viamrobotics/module-example-cpp). For a list of example modules in different Viam SDKs, take a look [here](https://github.com/viamrobotics/upload-module/#example-repos).

## Project structure
The complex module example defines three new resources: a Gizmo component, a Summation service, and a custom Base component.

The `proto` directory contains the `gizmo.proto` and `summation.proto` definitions of all the message types and calls that can be made to the Gizmo component and Summation service. These proto files are compiled automatically with `buf` through our cmake build system generation. See the `CMakeLists.txt` file in this directory for more information.

The `config` directory contains two buf.gen.yaml files that are used for compiling the files in `proto` to C++. `config/buf.gen.local.yaml.in` is used when offline generation is enabled and `config/buf.gen.remote.plugin.yaml.in` is used when offline proto generation is disabled. Offline proto generation is enabled by default and can be set with `VIAMCPPSDK_OFFLINE_PROTO_GENERATION`.

The `gizmo` directory contains all the necessary definitions for creating a custom `Gizmo` component type. `api.cpp` and `api.hpp` define what a `Gizmo` can do (mirroring the `proto` definition), implement the gRPC `GizmoServer` for receiving calls, and implement the gRPC `GizmoClient` for making calls. See the [API docs](https://docs.viam.com/build/program/apis/) for more info. `impl.cpp` and `impl.hpp` contain the unique implementation of a `Gizmo`. This is defined as a specific `Model`. See the [Model docs](https://docs.viam.com/registry/create/#valid-apis-to-implement-in-your-model) for more info.

Similarly, the `summation` directory contains the analogous definitions for the `Summation` service type. The files in this directory mirror the files in the `gizmo` directory.

The `base` directory contains all the necessary definitions for creating a custom modular `Base` component type. Since it is inheriting an already existing component supported by the Viam SDK, there is no need for `api` files.

In the main directory, there is also a `main.cpp` file, which creates a module, registers the above resources, and starts the module. It also handles SIGTERM and SIGINT OS signals using the `SignalManager` class. Read further to learn how to connect this module to your robot.

Finally, the `client.cpp` file can be used to test the module once you have connected to your robot and configured it. You will have to update the credentials and robot address in that file before building. After building, the `build/install/bin/complex_module_client` generated binary can be called to run the client.

`test_complex_module.cpp` is used by the Viam team to ensure the complex module example works as expected.

## Configuring and using the module

The `complex_module` binary generated after building is the entrypoint for this module. To connect this module with your robot, you must add this module's entrypoint to the robot's config. For example, the entrypoint file may be at `/home/viam-cpp-sdk/build/install/bin/complex_module` and you must add this file path to your configuration. See the [documentation](https://docs.viam.com/registry/configure/#add-a-local-module) for more details.

Once the module has been added to your robot, add a `gizmo` component that uses the `viam:gizmo:mygizmo` model. See the [documentation](https://docs.viam.com/registry/configure/#add-a-local-modular-resource) for more details. You can also add a `summation` service that uses the `viam:summation:mysummation` model and a `base` component that uses the `viam:base:mybase` model in a similar manner.

An example configuration for a gizmo component, a summation service, and a base component could look like this:
```json
{
  "components": [
    {
      "name": "gizmo1",
      "type": "gizmo",
      "namespace": "viam",
      "model": "viam:gizmo:mygizmo",
      "attributes": {
        "arg1": "arg1",
        "motor": "motor1"
      }
    },
    {
      "name": "base1",
      "type": "base",
      "model": "viam:base:mybase",
      "attributes": {
        "left": "motor1",
        "right": "motor2"
      }
    },
    {
      "name": "motor1",
      "type": "motor",
      "model": "fake"
    },
    {
      "name": "motor2",
      "type": "motor",
      "model": "fake"
    }
  ],
  "services": [
    {
      "name": "mysum1",
      "type": "summation",
      "namespace": "viam",
      "model": "viam:summation:mysummation",
      "attributes": {
        "subtract": false
      }
    }
  ],
  "modules": [
    {
      "name": "MyModule",
      "executable_path": "/home/viam-cpp-sdk/build/install/bin/complex_module"
    }
  ]
}
```
