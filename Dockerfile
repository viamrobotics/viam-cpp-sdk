FROM ubuntu:focal

ENV HOME /root
ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get -y --no-install-recommends install \
    build-essential \
    clang \
    cmake \
    gdb \
    ca-certificates \
    wget \
    protobuf-compiler \
    libprotobuf-dev \
    autoconf \ 
    automake \
    libtool \
    curl \
    make \
    g++ \
    unzip \
    pkg-config \
    git \
    libssl-dev

RUN cd ${HOME} && \
    wget --quiet \
        https://boostorg.jfrog.io/artifactory/main/release/1.81.0/source/boost_1_81_0.tar.gz && \
        tar xzf ./boost_1_81_0.tar.gz && \
        cd ./boost_1_81_0 && \
        ./bootstrap.sh && \
        ./b2 install && \
        cd .. && \
        rm -rf ./boost_1_81_0*

RUN BIN="/usr/local/bin" && \
    VERSION="1.11.0" && \
    cd ${HOME}  && \
    wget --quiet -O ${BIN}/buf "https://github.com/bufbuild/buf/releases/download/v${VERSION}/buf-$(uname -s)-$(uname -m)" && \
    chmod +x "${BIN}/buf"

RUN cd ${HOME} && \
    wget --quiet https://github.com/protocolbuffers/protobuf/releases/download/v21.12/protobuf-cpp-3.21.12.tar.gz && \
    tar xzf ./protobuf-cpp-3.21.12.tar.gz && \
    cd ./protobuf-3.21.12 && \
    ./configure && \
    make -j$(nproc) && \
    # make check && \
    make install && \
    ldconfig && \
    cd .. && \
    rm -rf ./protobuf*

RUN cd ${HOME} && \
    git clone https://github.com/google/re2 && \
    cd re2 && \ 
    make && \
    make test && \
    make install && \
    make testinstall && \
    ldconfig && \
    cd ${HOME} && \
    rm -rf ./re2

RUN MY_INSTALL_DIR=$HOME/.local && \
    mkdir -p $MY_INSTALL_DIR && \
    export PATH="$MY_INSTALL_DIR/bin:$PATH" && \
    cd ${HOME} && \
    git clone --recurse-submodules -b v1.50.0 --depth 1 --shallow-submodules https://github.com/grpc/grpc && \
    cd grpc && \
    mkdir -p cmake/build && \
    cd cmake/build && \
    cmake -DgRPC_INSTALL=ON -DgRPC_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=$MY_INSTALL_DIR ../.. && \
    make -j 4 && \
    make install && \
    cd ${HOME} && \
    rm -rf ./grpc

RUN curl -sL -o /usr/local/lib/libviam_rust_utils.so \
    https://github.com/viamrobotics/rust-utils/releases/latest/download/libviam_rust_utils-linux_$(uname -m).so && \
    ldconfig

ENV PKG_CONFIG_PATH=$HOME/.local/lib/pkgconfig/
ENV CPLUS_INCLUDE_PATH=$HOME/.local/lib/:$HOME/.local/include/