# syntax=docker/dockerfile:1
#
# One Dockerfile, parameterized over distro, version, and strategy.
# Stages:
#   base   - distro + universal devkit + LLVM + cmake. Internal only, not published.
#   system - base + apt -dev packages (grpc-from-source fallback on old distros). Published.
#
# Strategy is selected by bake `target = "system"`, not a build arg.
# Distro-conditional logic lives in scripts/, not in RUN steps here.

# Required parameters
ARG DISTRO
ARG VERSION

# Build options
# DEBUG=1 turns on `set -x` tracing in every install script.
ARG DEBUG=

FROM ${DISTRO}:${VERSION} AS base

# DEBIAN_FRONTEND set here once, not per-call in scripts.
ENV DEBIAN_FRONTEND=noninteractive \
    HOME=/root

ENV DEBUG=${DEBUG}

COPY scripts/ /opt/scripts/
RUN chmod -R +x /opt/scripts

RUN /opt/scripts/install-base.sh
RUN /opt/scripts/install-llvm.sh
RUN /opt/scripts/install-cmake.sh


FROM base AS system

# Optional override for the grpc version built from source (numeric, e.g. 1.65.0).
# When set, forces a source build at that version on every distro, even where the
# apt grpc would otherwise be accepted. Empty -> default behavior.
ARG GRPC_SOURCE_VERSION_OVERRIDE=
ENV GRPC_SOURCE_VERSION_OVERRIDE=${GRPC_SOURCE_VERSION_OVERRIDE}

RUN /opt/scripts/install-system-deps.sh
