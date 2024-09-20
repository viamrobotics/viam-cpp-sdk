These examples assume that the Viam C++ SDK has been built and
installed. If you have not yet done so, please visit the root of the
tree and read the build instructions, then install the SDK.

- For an example project showing how to consume an installed
  `viam-cpp-sdk` with CMake's `find_package`, see
  [`cmake/CMakeLists.txt`](cmake/CMakeLists.txt). Note that
  this sample `CMakeLists.txt` can be used with no modifications
  to build against the SDK as installed directly from CMake,
  or to build against the `viam-cpp-sdk` `conan` package. For
  instructions building with `conan`, see [here](/BUILDING.md#creating-and-consuming-the-sdk-conan-package).

- For an example project showing how to consume an installed
  `viam-cpp-sdk` via its `pkg-config` setup, see
  [`pkg-config/Makefile`](pkg-config/Makefile).
