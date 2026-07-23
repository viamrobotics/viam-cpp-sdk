# Docker for Viam C++ SDK Development

We provide docker infrastructure for C++ SDK development, either for developing
on the SDK itself, or for module development. Everything happens in a
[Docker bake file](etc/docker/docker-bake.hcl) which builds various parts of a
[Dockerfile](etc/docker/Dockerfile). The Dockerfile uses a layered approach, with
respective `system` and `conan` images built on top of a lightweight base layer.

The `system` image installs all the dependencies needed to do a build of the SDK
against _system_ dependencies, meaning those installed mostly from `apt`.

The `conan` image is set up to allow creating and building the `viam-cpp-sdk` conan package. See [conan.md](doc/conan.md) for more on this.

Let's consider the `system` image case. From the root of this repository, run

```shell
docker buildx bake -f etc/docker/docker-bake.hcl --set '*.platform=linux/arm64' --load system-debian-bookworm
```

This sets the target platform to `linux/arm64`, creating a `debian-bookworm` version
of the `system` image. Running

```shell
docker run --rm -it ghcr.io/viamrobotics/cpp-sdk-system-debian:bookworm bash
```

will put you in a bookworm container which is all set to clone and build the SDK as per the
[build instructions](doc/BUILDING.md#invoking-cmake-to-generate-a-build-system).

If you are writing a module with the Viam C++ SDK, you can use the `system` Docker image as a base
container to build the SDK and your module on top of it, or to set up CI pipelines. For example, see the
Linux jobs in the [pull request CI](.github/workflows/test.yml).

Many Viam C++ SDK modules use Conan for dependency management. For similar patterns built on the `conan` image,
see the [conan CI](.github/workflows/conan.yml).
