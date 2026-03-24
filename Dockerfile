#FROM --platform=linux/amd64 mcr.microsoft.com/devcontainers/rust:latest
FROM --platform=linux/amd64 ubuntu:24.04

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install \
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
