[![Build](https://github.com/cpp-core/timer/actions/workflows/build.yaml/badge.svg)](https://github.com/cpp-core/timer/actions/workflows/build.yaml)

## Build

    git clone git@github.com:cpp-core/timer
    mkdir timer/build && cd timer/build
    CC=clang-15 CXX=clang++-15 cmake -DCMAKE_INSTALL_PREFIX=$HOME/opt ..
    make check
