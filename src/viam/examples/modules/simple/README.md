# VIAM Simple Module Example
This example goes through how to create custom modular resources using Viam's C++ SDK, and how to connect it to a Robot.

This is a limited document. For a more in-depth understanding of modules, see the [documentation](https://docs.viam.com/program/extend/modular-resources/).

## Purpose
Modular resources allow you to define custom components and services and add them to your robot. Viam ships with many component types, but you're not limited to only using those types -- you can create your own using modules.

For more information, see the [documentation](https://docs.viam.com/program/extend/modular-resources/). For a more complex example, take a look at the [complex module example](https://github.com/viamrobotics/viam-cpp-sdk/tree/main/src/viam/examples/modules/complex), which contains multiple new APIs and custom resource models.

## Project structure
The `main.cpp` file contains the definition of a new generic model and code to register it.

It also has the optional validator function and implements reconfigure. The validator function can throw errors that are triggered due to errors in the configuration. It also returns a vector of strings representing the implicit dependencies of the resource. The reconfiguration method reconfigures the resource based on the new configuration passed in. The validator function is defined upon resource registration, and the reconfigure method is implemented on the resource class.

When simple_module is called, the main function creates and starts the module. Read further to learn how to connect this module to your robot.

## Configuring and using the module
These steps assume that you have a robot available at [app.viam.com](app.viam.com).

The `simple_module` binary generated after building is the entrypoint for this module. To connect this module with your robot, you must add this module's entrypoint to the robot's config. For example, this could be `/home/viam-cpp-sdk/build/install/bin/simple_module`. See the [documentation](https://docs.viam.com/program/extend/modular-resources/#use-a-modular-resource-with-your-robot) for more details.

Once the module has been added to your robot, you will then need to add a component that uses the `acme:demo:printer` model. See the [documentation](https://docs.viam.com/program/extend/modular-resources/#configure-a-component-instance-for-a-modular-resource) for more details. `acme:demo:printer` components depend on a generic component `component1` by default; you will need to include a generic component with that name, as well.

An example configuration for a printer that depends on a generic component could look like this:
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
			"model": "acme:demo:printer"
		},
    {
			"namespace": "rdk",
			"type": "generic",
			"name": "component1",
			"model": "rdk:builtin:fake"
		}
	]
}
```

