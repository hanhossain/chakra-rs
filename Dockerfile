#FROM --platform=linux/amd64 mcr.microsoft.com/devcontainers/rust:latest
FROM --platform=linux/amd64 ubuntu:24.04

RUN DEBIAN_FRONTEND=noninteractive apt-get update  \
    && apt-get -y install \
    build-essential \
    gcc-14 g++-14 \
    curl git \
    cmake ninja-build \
    gdb \
    clang-20 lldb-20 lld-20 libc++-20-dev libc++abi-20-dev

RUN ln -s -f /bin/clang++-20 /bin/clang++
RUN ln -s -f /bin/clang-20 /bin/clang

RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y

ENV CARGO_BUILD_TARGET=x86_64-unknown-linux-gnu
