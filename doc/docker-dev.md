Some Dockerfiles are provided for C++ SDK development, either for developing
on the SDK itself, or for module development. The docker images are split up
into base images, found in [etc/docker/base-images](etc/docker/base-images), and
an [SDK build image](etc/docker/Dockerfile.sdk-build). The base images install
apt packages required to build and develop the SDK, whereas the SDK build image
is meant to be built on top of a base image using multi-stage Docker building.

From the root of this repository, run
```shell
docker build -t base/bullseye -f etc/docker/base-images/Dockerfile.debian.bullseye .
```
This will create a Debian Bullseye base image. Note the use of the `-t base/bullseye`
arg to assign a tag to the image, which is important for the next step. You can then
use `Dockerfile.sdk-build` in a couple different ways.

```shell
docker build --build-arg BASE_TAG=base/bullseye --build-arg GIT_TAG=[...] -f etc/docker/Dockerfile.sdk-build .
```

This will use `base/bullseye` as a base to build the SDK version provided in `GIT_TAG`,
which should be a tagged release version. The SDK will be cloned from
https://github.com/viamrobotics/viam-cpp-sdk/. This is the recommended approach for
C++ module development, which should generally be done against a tagged release.

You can also do
```shell
docker build --build-arg BASE_TAG=base/bullseye --build-arg REPO_SETUP=copy -f etc/docker/Dockerfile.sdk-build .
```

Note the use of the build argument `REPO_SETUP=copy`, which adds a Docker instruction
to copy the SDK repo from the current working directory, rather than cloning from
GitHub. This approach may make more sense for developing on the SDK itself, or if
your C++ SDK development relies on a localversion of the SDK.

The examples above illustrated the use of several `--build-arg` arguments, namely
`BASE_TAG`, `GIT_TAG`, and `REPO_SETUP`. Please see
[Dockerfile.sdk-build](etc/docker/Dockerfile.sdk-build) for a complete account of
all build arguments and their defaults.
