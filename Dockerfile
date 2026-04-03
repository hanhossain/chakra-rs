#FROM --platform=linux/amd64 mcr.microsoft.com/devcontainers/rust:latest
FROM --platform=linux/amd64 ubuntu:24.04

RUN DEBIAN_FRONTEND=noninteractive apt-get update  \
    && apt-get -y install \
    build-essential \
    gcc-14 g++-14 \
    curl \
    git \
    cmake \
    ninja-build \
    clang-19 \
    lld-19 \
    gdb

RUN ln -sf /usr/bin/gcc-14 /usr/bin/gcc
RUN ln -sf /usr/bin/g++-14 /usr/bin/g++

RUN ln -sf /usr/bin/clang++-19 /usr/bin/clang++
RUN ln -sf /usr/bin/clang-19 /usr/bin/clang

RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y

ENV CARGO_BUILD_TARGET=x86_64-unknown-linux-gnu
