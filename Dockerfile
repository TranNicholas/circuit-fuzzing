FROM aflplusplus/aflplusplus:latest

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install --no-install-recommends -y \
        autoconf \
        ccache \
        help2man \
        libfl2 \
        libfl-dev \
        libjemalloc-dev \
        numactl \
        perl \
        perl-doc \
        zlib1g \
        zlib1g-dev \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

ARG REPO=https://github.com/verilator/verilator
ARG SOURCE_COMMIT=master

WORKDIR /tmp

RUN git clone "${REPO}" verilator && \
    cd verilator && \
    git checkout "${SOURCE_COMMIT}" && \
    autoconf && \
    CC=clang CXX=clang++ ./configure && \
    make -j"$(nproc)" && \
    make install && \
    cd .. && \
    rm -r verilator && \
    ccache -C && \
    verilator --version && \
    command -v afl-fuzz && \
    command -v afl-clang-fast++

WORKDIR /work
