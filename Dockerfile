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
    clang-19 lld-19 \
    gdb \
    libc++-19-dev libc++abi-19-dev

RUN update-alternatives --install /usr/bin/clang clang /usr/bin/clang-19 10
RUN update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-19 10

RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y

ENV CARGO_BUILD_TARGET=x86_64-unknown-linux-gnu
ENV IN_DOCKER=true

ENV PATH="/root/.cargo/bin:${PATH}"

RUN curl -L --proto '=https' --tlsv1.2 -sSf https://raw.githubusercontent.com/cargo-bins/cargo-binstall/main/install-from-binstall-release.sh | bash
RUN cargo binstall cargo-nextest --secure
