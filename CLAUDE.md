# Viam C++ SDK

Public C++ SDK for the Viam robotics platform (`viam-cpp-sdk`). Built with CMake. Tests use Boost.Test.

## Codebase Structure

```
src/viam/sdk/
  components/<name>/                — Hardware component types (arm, base, camera, etc.)
  services/<name>/                  — Service types (vision, motion, slam, etc.)
  robot/                            — Robot client and connection
  resource/                         — Resource registry, base interfaces
  rpc/                              — Channel, connection, and gRPC plumbing
  config/                           — Config parsing
  module/                           — Modular component support
  spatialmath/                      — Geometries, poses, transforms
  referenceframe/                   — Frame system
  registry/                         — Type registration
  log/                              — Logging
  common/                           — Shared utilities
  app/                              — App/cloud API clients
  tests/                            — Boost.Test unit tests
src/viam/api/                       — Auto-generated protobuf code (NEVER EDIT, regenerated from viamrobotics/api)
examples/                           — Example modules and clients
```

All component clients follow the same pattern: header in `<name>.hpp`, gRPC client wrapper in `client.hpp`/`client.cpp`, server-side handler in `server.hpp`/`server.cpp`.

## Conventions

- **Language**: C++ (see `CMakeLists.txt` for the exact standard).
- **Build**: CMake. Out-of-tree builds in a `build/` directory.
- **Method signatures** for component client methods take an `AttributeMap extra` parameter for the optional extra arguments and return typed C++ values converted from proto responses.
- **Testing**: Boost.Test. Test files are named `test_<component>.cpp` under `src/viam/sdk/tests/`.

## Protected Paths

Enforced via `.claude/settings.ci.json`:

- `src/viam/api/**` — generated protobuf code.
- `.github/**` — CI workflows.
- `Makefile` — build glue.

## Verification Commands

- Configure build: `cmake -B build -S .`
- Build: `cmake --build build`
- Run tests: `ctest --test-dir build`
- Full clean rebuild: `rm -rf build && cmake -B build -S . && cmake --build build`
