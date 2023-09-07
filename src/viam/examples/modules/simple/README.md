# VIAM Simple Module Example
This example goes through how to create custom modular resources using Viam's C++ SDK, and how to connect it to a Robot.

This is a limited document. For a more in-depth understanding of modules, see the [documentation](https://docs.viam.com/program/extend/modular-resources/).

## Purpose
Modular resources allow you to define custom components and services and add them to your robot. Viam ships with many component types, but you're not limited to only using those types -- you can create your own using modules.

For more information, see the [documentation](https://docs.viam.com/program/extend/modular-resources/). For a more complex example, take a look at the [complex module example](https://github.com/viamrobotics/viam-cpp-sdk/tree/main/src/viam/examples/modules/complex), which contains multiple new APIs and custom resource models.

## Project structure
The `main.cpp` file contains the definition of a new generic model and code to register it. It also has the optional validator function and implements reconfigure. The validator function is defined upon resource registration, and the reconfigure method is implemented on the resource class.

The validator function can throw errors that are triggered due to errors in the configuration. It also returns a vector of strings representing the implicit dependencies of the resource. Note that printers have no implicit dependencies; see the [complex module example](https://github.com/viamrobotics/viam-cpp-sdk/tree/main/src/viam/examples/modules/complex) for examples of modular resources with implicit dependencies.

The reconfiguration method reconfigures the resource based on the new configuration passed in.

When simple_module is run, the main function creates and starts the module. Read further to learn how to connect this module to your robot.

## Configuring and using the module
The `simple_module` binary generated after building is the entrypoint for this module. To connect this module with your robot, you must add this module's entrypoint to the robot's config. For example, this could be `/home/viam-cpp-sdk/build/install/bin/simple_module`. See the [documentation](https://docs.viam.com/program/extend/modular-resources/#use-a-modular-resource-with-your-robot) for more details.

Once the module has been added to your robot, you will then need to add a component that uses the `acme:demo:printer` model. See the [documentation](https://docs.viam.com/program/extend/modular-resources/#configure-a-component-instance-for-a-modular-resource) for more details. 

An example configuration for a printer could look like this:
```json
{
	"modules": [
		{
			"name": "MyModule",
			"executable_path": "/home/viam-cpp-sdk/build/install/bin/example_module"
		}
	],
	"components": [
		{
			"namespace": "rdk",
			"type": "generic",
			"name": "printer1",
			"model": "acme:demo:printer",
      "attributes": {
        "to_print": "foo"
      }
		}
	]
}
```

