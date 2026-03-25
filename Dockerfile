#FROM --platform=linux/amd64 mcr.microsoft.com/devcontainers/rust:latest
FROM --platform=linux/amd64 ubuntu:24.04

RUN DEBIAN_FRONTEND=noninteractive apt-get update  \
    && apt-get -y install \
    build-essential \
    curl \
    git \
    cmake \
    ninja-build \
    clang-19 \
    lld-19 \
    gdb

RUN ln -s /bin/clang++-19 /bin/clang++
RUN ln -s /bin/clang-19 /bin/clang

RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y

ENV CARGO_BUILD_TARGET=x86_64-unknown-linux-gnu
