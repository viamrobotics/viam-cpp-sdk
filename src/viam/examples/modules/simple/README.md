# VIAM Simple Module Example
This example goes through how to create custom modular resources using Viam's C++ SDK, and how to connect it to a machine.

This is a limited document. For a more in-depth understanding of modules, see the [documentation](https://docs.viam.com/registry/).

## Purpose
Modular resources allow you to define custom components and services and add them to your machine. Viam ships with many component types, but you're not limited to only using those types -- you can create your own using modules.

For more information, see the [documentation](https://docs.viam.com/registry/). For a more complex example, take a look at the [complex module example](https://github.com/viamrobotics/viam-cpp-sdk/tree/main/src/viam/examples/modules/complex), which contains multiple new APIs and custom resource models.

For a list of example modules in different Viam SDKs, take a look [here](https://github.com/viamrobotics/upload-module/#example-repos).

## Project structure
The `main.cpp` file contains the definition of a new sensor component and code to register it. It also has the optional validator function and implements reconfigure. The validator function is defined upon resource registration, and the reconfigure method is implemented on the resource class.

The validator function can throw errors that are triggered due to errors in the configuration. It also returns a vector of strings representing the implicit dependencies of the resource. Note that this sensor has no implicit dependencies; see the [complex module example](https://github.com/viamrobotics/viam-cpp-sdk/tree/main/src/viam/examples/modules/complex) for examples of modular resources with implicit dependencies.

The reconfiguration method reconfigures the resource based on the new configuration passed in.

When simple_module is run, the main function creates and starts the module. Read further to learn how to connect this module to your machine.

## Configuring and using the module
The `simple_module` binary generated after building is the entrypoint for this module. To connect this module with your machine, you must add this module's entrypoint to the machine's config. For example, this could be `/home/viam-cpp-sdk/build/install/bin/simple_module`. See the [documentation](https://docs.viam.com/registry/configure/#add-a-local-module) for more details.

Once the module has been added to your machine, you will then need to add a component that uses the `viam:sensor:mysensor` model. See the [documentation](https://docs.viam.com/registry/configure/#add-a-local-modular-resource) for more details.

An example configuration for our sensor could look like this:
```json{
  "components": [
    {
      "name": "mysensor",
      "api": "rdk:component:sensor",
      "model": "viam:sensor:mysensor",
      "attributes": {
        "multiplier": 2
      }
    }
  ],
  "modules": [
    {
      "type": "local",
      "name": "my-module",
      "executable_path": "/home/viam-cpp-sdk/build/install/bin/simple_module"
    }
  ]
}
```

Note in particular that our sensor has a `multiplier` attribute whose presence is checked in the `validate` and `reconfigure` methods, defaulting to 1.0 if not present.
