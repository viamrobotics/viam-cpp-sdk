# syntax=docker/dockerfile:1
#
# One Dockerfile, parameterized over distro, version, and strategy.
# Stages:
#   base   - distro + universal devkit + LLVM + cmake. Internal only, not published.
#   system - base + apt -dev packages (grpc-from-source fallback on old distros). Published.
#
# Strategy is selected by bake `target = "system"`, not a build arg.
# Distro-conditional logic lives in scripts/, not in RUN steps here.

ARG DISTRO=debian
ARG VERSION=bookworm

FROM ${DISTRO}:${VERSION} AS base

# DEBIAN_FRONTEND set here once, not per-call in scripts.
ENV DEBIAN_FRONTEND=noninteractive \
    HOME=/root

# Versions and toggles ride in on build args; distro identity comes from /etc/os-release.
ARG LLVM_VERSION=19
ARG CMAKE_MIN_VERSION=3.25
# DEBUG=1 turns on `set -x` tracing in every install script.
ARG DEBUG=
ENV LLVM_VERSION=${LLVM_VERSION} \
    CMAKE_MIN_VERSION=${CMAKE_MIN_VERSION} \
    DEBUG=${DEBUG}

COPY scripts/ /opt/scripts/
RUN chmod -R +x /opt/scripts

RUN /opt/scripts/install-base.sh
RUN /opt/scripts/install-llvm.sh
RUN /opt/scripts/install-cmake.sh


FROM base AS system

ARG GRPC_VERSION=v1.62.1
ENV GRPC_VERSION=${GRPC_VERSION}

RUN /opt/scripts/install-system-deps.sh
