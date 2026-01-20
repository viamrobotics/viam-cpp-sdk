# Using the Conan to build or consume the C++ SDK

The Viam C++ SDK provides first class support for the `conan` C++
package manager. This section will go into more detail
without assuming any familiarity with `conan`, but please consult the
[`conan` docs](https://docs.conan.io/2/introduction.html) for more info.

To use `conan`, you'll need to have `python` and `pip` on your system if
you don't already. For example,

- Debian: `apt-get install python3 python3-pip`
- MacOS with Homebrew: `brew install python`
- Windows: `choco install python conan`

and then, optionally in a venv if you prefer,

```shell
pip install conan # Not necessary on Windows since choco installed it above
conan profile detect
```

The second line detects a default [`conan`
profile](https://docs.conan.io/2/reference/config_files/profiles.html)
encapsulating your build environment, which you can modify as needed.

## The Viam Conan Server and C++ Module Development

Viam has a Conan server which we use to host artifacts and recipes of the Viam C++ SDK.
It can be accessed as follows:

```shell
conan remote add viamconan https://viam.jfrog.io/artifactory/api/conan/viamconan
```

If you want to build a C++ module with Conan support, adding the `viamconan` remote above
is the prerequisite to being able to do `conan create . [...]` to build a module with
`viam-cpp-sdk/0.x.y` as a dependency. This includes various C++ modules in the
[viam-modules](https://github.com/viam-modules) organization.

Each new release of the C++ SDK has artifacts published to the `viamconan` remote for several
common binary targets, and if there is not an artifact for your platform, Conan will automatically grab
the recipe to build it locally.

The information above should be enough to get started on C++ module development with Conan support.
For other use cases, read on!

## Building and Managing Dependencies

Next note that there are two possible approaches here.
Namely, it is possible to

1. use `conan` to build and package the SDK; OR
2. use `conan` to obtain the [software
   prerequisites](BUILDING.md#software-prerequisites) for the SDK.

Option 1. makes more sense for building against the C++ SDK, as you would in module development.
As compared to the approach above using tagged releases from the Viam Conan Server, this would
only be necessary if you need to build your module against changes to the C++ SDK which are not yet
in a tagged release. In this scenario we use Conan to get the SDK's dependencies, and then to
build it and install the package in the local Conan cache.

Option 2. makes more sense for doing locally development *on* the SDK
while using `conan` to get dependencies instead of your system package
manager. Note that Option 1 implies a superset of Option 2.

In either case, `conan` will use the [`conanfile.py`](../conanfile.py).
Note that we build with `offline_proto_generation=True` by default.

### Option 1. Creating and consuming the SDK `conan` package.

Here we use `conan` to package and install the SDK so that we can build
against it by declaring it as a dependency in a
`conanfile.txt` or a `conanfile.py`.

To do this, call `conan create . --build=missing` from the project root. This will build
and test the `viam-cpp-sdk` recipe, adding it into your local cache. See
the [conan docs](https://docs.conan.io/2/reference/commands/create.html)
for more info on `conan create`, as we will omit any details about using
profiles, options, or settings to customize the build.

Once this is done, the `viam-cpp-sdk` package is ready to be consumed.
The example projects show a [minimal
`conanfile.txt`](../src/viam/examples/project/cmake/conanfile.txt). With
this `conanfile.txt` in the same directory as your project's
`CMakeLists.txt`, you can then do, for example,

```shell
conan install . --output-folder=build-conan --build=missing
cmake . --preset=conan-release
cmake --build --preset=conan-release -j 8
```

Note that this can be done with the same `CMakeLists.txt` from the
[example project](../src/viam/examples/project/cmake/CMakeLists.txt): it is
agnostic of the use of `conan` to package the SDK as opposed to the SDK
having been built and installed manually.

It is also possible to build using a `conanfile.py` rather than a
`conanfile.txt`, see again the [conan
docs](https://docs.conan.io/2/tutorial/consuming_packages/the_flexibility_of_conanfile_py.html#consuming-packages-flexibility-of-conanfile-py),
or look at the [`test_package/conanfile.py`](../test_package/conanfile.py)
which is the test package recipe.

### Option 2. Using `conan` to manage the SDK dependencies

Here we use `conan` to grab the SDK dependencies, setting ourselves up
for local SDK development.

From the root of this repo, you can do
``` shell
# optionally specify --output-folder=build-conan to not use the build/ dir
conan install . --build=missing
```
to install dependencies with `conan`. And then
``` shell
cmake . --preset conan-release
cmake --build --preset=conan-release -j
```
The `conan install` sets up some
[cmake-presets](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html)
which are used to resolve `--preset conan-release` above. Among other
things, this tells CMake about the `conan_toolchain.cmake` generated by
`conan`, and runs CMake with certain environment variables set.

If you do not want to use cmake-presets, you can do
``` shell
cd build-conan
source conanbuild.sh
cmake .. -G Ninja -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake
```
and later call `source deactivate_conanbuild.sh`. The `conanbuild.sh`
script does the same environment variable setting as the cmake-presets,
but it may be preferable if you would rather invoke your build system
directly.

PLEASE NOTE: Whether by cmake-presets or `conanbuild.sh`, you MUST, one
way or another, make the `PATH` entries set by `conan` available to
CMake, because `buf` requires that `protoc` is available on `PATH`. If
you do not do this then `buf generate` will fail outright, or, if you
have a different version of `protoc` available in your `PATH`, it will
silently fail and later cause compilation failures due to protobuf
version mismatches.

In the instructions above, we separated using `conan install` to grab the
dependencies, and then using `cmake` to configure and invoke the build system. Conan also
provides `conan build` which performs all these in one fell swoop, via
```shell
conan build .
```
Note however that this uses the `build()` method from the `conanfile.py`, which skips building
the tests and examples. You are free to reenable them by editing the relevant settings in the `CMakeCache.txt`.
