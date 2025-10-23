# VIAM Simple Module Example
This example goes through how to create a custom audio input resource using Viam's C++ SDK, and how to connect it to a Robot.

This is a limited document. For a more in-depth understanding of modules, see the [documentation](https://docs.viam.com/registry/).

Refer to main.cpp and the comments throughout for more information. For other C++ module examples, refer to the [simple module example](https://github.com/viamrobotics/viam-cpp-sdk/tree/main/src/viam/examples/modules/simple) or [complex module example](https://github.com/viamrobotics/viam-cpp-sdk/tree/main/src/viam/examples/modules/complex).

For a fully fleshed-out example of a C++ module that uses Github CI to upload to the Viam Registry, take a look at [module-example-cpp](https://github.com/viamrobotics/module-example-cpp). For a list of example modules in different Viam SDKs, take a look [here](https://github.com/viamrobotics/upload-module/#example-repos).

This is a limited document. For a more in-depth understanding of modules generally, see the [documentation](https://docs.viam.com/program/extend/modular-resources/).


Example Configuration:
```json{
  "components": [
    {
      "name": "sinewave-audio",
      "api": "rdk:component:audio_in",
      "model": "viam:audio_in:sinewave",
      "attributes": {
        "frequency": 440
      }
    }
  ],
  "modules": [
    {
      "type": "local",
      "name": "my-module",
      "executable_path": "/home/viam-cpp-sdk/build/src/viam/examples/modules/audioin/audioin_module"
    }
  ]
}
```

