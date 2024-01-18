# Viam C++ SDK - Build Instructions

## (In)stability Notice

> **Warning** This is a beta release of the Viam C++ SDK. Stability
> is not guaranteed. Breaking changes are likely to occur, and occur
> often.

## Overview

The `viam-cpp-sdk` has a newly introduced CMake build system to
replace the existing Makefile infrastructure. This document explains
how to use the new infrastructure.

If you experience problems while following this guide, please see the
`Limitations, Known Issues, and Troubleshooting` section at the bottom
to see if your issue (and hopefully a workaround) is covered there.

PLEASE NOTE: It is very likely that you will need to rebuild the
generated sources as part of building the SDK. Please see the
documentation on `VIAMCPPSDK_USE_DYNAMIC_PROTOS` below, and the
additional comments about mismatched protobufs in the troubleshooting
guide, before continuing further.

## Software Prerequisites

The project depends on [CMake](https://cmake.org/) >= 3.25,
[Boost](https://www.boost.org/) >= 1.74, [gRPC](https://grpc.io/) >=
1.30.2, [protobuf](https://protobuf.dev/overview/) >= 3.12.4,
[xtensor](https://github.com/xtensor-stack/xtensor) >= 0.24.3,
[abseil](https://abseil.io/) and on the transitive dependencies of
those projects.

You will need to install these required dependencies before building
the Viam C++ SDK, preferably by way of your system package manager:

- Debian: `apt-get install cmake build-essential libabsl-dev libboost-all-dev libgrpc++-dev libprotobuf-dev libxtensor-dev`
- MacOS with Homebrew: `brew install abseil cmake boost grpc protobuf xtensor`

There are also several optional or conditionally required dependencies:

- Debian: `apt-get install pkg-config ninja-build protobuf-compiler-grpc`
- MacOS with Homebrew: `brew install pkg-config ninja buf`

The `pkg-config` dependency is needed if your local system does not
provide CMake `find_package` support for one or more required
dependencies like `gRPC`.

The `ninja[-build]` dependency is only required if you want to use
Ninja as the target build system, so is entirely optional.

You will need `protobuf-compiler-grpc` if you intend to do local
dynamic protobuf generation (see the relevant SDK options below) or if
your local gRPC version is too old to be used with buf.build
(effectively, older than gRPC 1.41).

Installing the `buf` homebrew package will save you from needing to
download it as part of the build, which costs a little time on every
build invocation.

Note that Debian Bullseye does not ship a recent enough version of
CMake by default, but it can be obtained from the Bullseye Backports
repository:

``` shell
apt-get install software-properties-common
apt-add-repository 'deb http://deb.debian.org/debian bullseye-backports main'
apt-get update
apt-get install -t bullseye-backports cmake
```

You can of course build and install any or all of these requirements
manually rather than via your package manager, though this is likely
to make it more difficult to correctly configure the build of the Viam
C++ SDK.

## Obtaining the `viam-cpp-sdk` Source

Just clone https://github.com/viamrobotics/viam-cpp-sdk into a
directory of your choice and checkout the branch or tag of interest.

## Invoking CMake to Generate a Build System

This document doesn't intend to provide a complete guide for how CMake
is used, as that is a complex topic. Please consult the [CMake
documentation](https://cmake.org/cmake/help/latest/index.html) for
specific details on how CMake works and how to invoke the tool. The
guidance here is intended to cover the common case builds of the
`viam-cpp-sdk`.

CMake organizes out-of-tree builds. The first step is to create a
scratch directory where you want to keep the working state of your
build. A convention is to name this directory `build` and to create it
under the root of the project. The `viam-cpp-sdk` top-level
`.gitignore` file already ignores such a directory, so we recommend
starting there:

``` shell
git clone https://github.com/viamrobotics/viam-cpp-sdk
cd viam-cpp-sdk
mkdir build
cd build
```

CMake isn't itself a build system, it is a build system generator. Our
next step is to generate the build system. In these examples we use
Ninja, but you can also use Makefiles or [many other build
systems](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html).

When invoking CMake, you pass the path to the root `CMakeLists.txt`
file for the project you wish to build. If you have opted to use an
in-tree `build` directory per the above instructions, this is as
simple as:

``` shell
cmake .. -G Ninja
```

If you encounter an error about an inability to link an OpenSSL target on MacOS,
try setting your [PKG_CONFIG_PATH](#setting-pkg_config_path) to point to your
Homebrew installation.

If all went well, you should now have a file named `build.ninja` in
the `build` directory created above. Note that `git status` will not
show this, since the `build` directory is ignored.

If you wish to use a different generator than Ninja, change the
argument to `-G` above to the build system you wish to generate for.

## Building and Installing the SDK

To compile all targets in the SDK:

``` shell
ninja all
```

To install the SDK:

``` shell
ninja install
```

By default, the build is configured to install the SDK to the
`install` subdirectory of the working directory, so if you have been
following the instructions above and the build worked, you should see
an installation tree under `build/install`.

If the build did not succeed, please see the next section.


## Options to Configure or Customize the Build

### Options for Package Search

A frequent cause of difficulty or failure is an inability for the
build to find a third-party dependency like `gRPC`. If the build
fails, first check if you have installed the right dependencies per
the lists above. In some cases, you may find that a necessary direct
or transitive dependency has been installed, but to a location that
CMake does not search by default.

#### Setting `PKG_CONFIG_PATH`

On Homebrew, OpenSSL does not get installed in the default search
paths because it is "keg-only". We can inform CMake of the actual
install location by customizing `PKG_CONFIG_PATH` in the shell
environment before invoking CMake:

``` shell
PKG_CONFIG_PATH="/opt/homebrew/opt/openssl@3/lib/pkgconfig" cmake ...
```

#### Setting `CMAKE_PREFIX_PATH`

CMake also has its own intrinsic package search mechanism. If you have
dependencies installed to locations that are not part of CMake's
default search set, you can add them by configuring
`CMAKE_PREFIX_PATH` as an argument to CMake. For instance, if you have
a personal tree of installed software in `$HOME/opt`, you can inform
CMake that this path should be searched for packages by passing it as
a command line "define" to CMake:

``` shell
cmake -DCMAKE_PREFIX_PATH=$HOME/opt ...

```

Please see
[CMAKE_PREFIX_PATH](https://cmake.org/cmake/help/latest/variable/CMAKE_PREFIX_PATH.html)
for more details on `CMAKE_PREFIX_PATH`.

### Options for Building the SDK

### `CMAKE_INSTALL_PREFIX`

By default, the SDK will install into a subdirectory of the working
build directory. This is not appropriate for making the SDK available
for other projects. To install the SDK to a location of your choice
you should customize `CMAKE_INSTALL_PREFIX` while invoking `CMake` and
then re-run the `install` task. For instance, to install the SDK to
`/usr/local`:


``` shell
cmake -DCMAKE_INSTALL_PREFIX=/usr/local ...
ninja install
```

#### `VIAMCPPSDK_ENFORCE_COMPILER_MINIMA`

By default, the SDK build will enforce compiler minima. If you must
bypass this check for some reason, you may do so by disabling this
default-on option:

``` shell
cmake -DVIAMCPPSDK_ENFORCE_COMPILER_MINIMA=OFF ...
```

#### `VIAMCPPSDK_USE_DYNAMIC_PROTOS`

Because the checked-in copies of generated sources only work with a
narrow range of gRPC versions, the SDK by default will dynamically
regenerate proto code. If you wish to test against the checked-in
copies or cannot regenerate for any reason, you can do so by disabling
this flag:

``` shell
cmake -DVIAMCPPSDK_USE_DYNAMIC_PROTOS=OFF ...
```

#### `VIAMCPPSDK_OFFLINE_PROTO_GENERATION`

By default, the SDK will use local buf services and definitions to
generate code. It is possible to instead rely on buf.build services to
do code generation (note that this requies online access):

```shell
cmake -DVIAMCPPSDK_OFFLINE_PROTO_GENERATION=OFF ...
```

Note that this option only has an effect when
`VIAMCPPSDK_USE_DYNAMIC_PROTOS=ON`.

## Building Against the SDK

### Using CMake

The Viam C++ SDK installs support files for CMake's `find_package`
subsystem.

``` cmake
find_package(viam-cpp-sdk CONFIG REQUIRED COMPONENTS Lib)
target_link_libraries(mytarget viam-cpp-sdk::viamcpp)

```

Note that you will need to ensure that `CMAKE_PREFIX_PATH` is set
appropriately in your project to include the path to the installed
Viam C++ SDK.

An example project using this technique is available under
`examples/project/cmake` and it can be built with CMake in the regular
way. If you have allowed the SDK to install to the default
`build/install` prefix, the example should build against your SDK
installation automatically. If you have customized the SDK
installation prefix by building it with a specific
`CMAKE_INSTALL_PREFIX` that is not among CMake's default package
search list (e.g. `$HOME/opt`), you will need to pass that information
along when building the example:

``` shell
cd examples/project/cmake
mkdir build
cd build
cmake .. -DCMAKE_PREFIX_PATH=<path/to/which/the/sdk/was/installed> ...
```

Consuming the SDK via CMake is the preferred mechanism and likely to
give the best results.

### Using `pkg-config`

The Viam C++ SDK also installs `.pc` files to introspect the SDK with
`pkg-config`. An example project using `make` and `pkg-config` is
available under `examples/project/pkg-config`. Much like with the need
to customize the CMake prefix path, you may need to customize
`PKG_CONFIG_PATH` in order to find a Viam C++ SDK installation that is
not part of `pkg-config`'s default search list:

``` shell
cd examples/project/pkg-config
PKG_CONFIG_PATH=<path/to/which/the/sdk/was/installed>/lib/pkgconfig make ...
```

### Manually

It is of course possible to build against the Viam C++ SDK without
using either CMake support or `pkg-config` by explicitly setting
toolchain include paths, library paths, and passing `-lviamcpp` or
similar on the link line.

## Limitations, Known Issues, and Troubleshooting

### Build Issue: Missing Build Dependency Edge when Using `VIAMDPPSDK_USE_DYNAMIC_PROTOS`

Sometimes when running `ninja all` or `ninja install` in a project
using `-DVIAMCPPSDK_USE_DYNAMIC_PROTOS=ON` the build will fail because
generated sources have not yet been produced. If that happens you can
explicitly run the `ninja generate-dynamic-protos` command to ensure
that they are pre-populated.

``` shell
cmake -DVIAMCPPSDK_USE_DYNAMIC_PROTOS=ON ...
ninja all   # FAILS!
ninja generate-dynamic-protos
nina all    # OK!
```

### Build Issue: Proto Mismatch

If compilation fails with a wall of compiler-generated errors that
includes text like:

``` text
error: incompatible with your Protocol Buffer headers
```

then there is likely a mismatch between your local protobuf/gRPC
version and the version used to generate the checked-in files. Switch
to using dynamic proto generation:

``` shell
cmake ...
ninja all    # FAILS, wall of compiler text with above message
cmake -DVIAMCPPSDK_USE_DYNAMIC_PROTOS=ON ...
ninja all    # OK!
```


### Runtime Issue: The `viam_rust_utils` Shared Library Not Found

For some platforms (mostly macOS) the runtime and install name
properties are not correctly configured for the `viam_rust_utils`
library. This can lead to a runtime error where the `viam_rust_utils`
shared library is not found at program startup. If this happens, set
`[DY]LD_LIBRARY_PATH` to point into the library directory of the
installation when running commands that require it:

``` shell
cmake ...
ninja install
./build/install/bin/example_echo    # FAILS: libviam_rust_utils not found
export DYLD_LIBRRAY_PATH=./build/install/lib
./build/install/bin/example_echo    # OK!
```

## Getting Help

If you are having trouble getting the SDK installed or your project
built against it and running, please take advantage of our [community
resources](https://www.viam.com/resources/community).

## License
Copyright 2023 Viam Inc.

Apache 2.0 - See [LICENSE](https://github.com/viamrobotics/viam-cpp-sdk/blob/main/LICENSE) file
