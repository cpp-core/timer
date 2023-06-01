[![Build](https://github.com/cpp-core/timer/actions/workflows/build.yaml/badge.svg)](https://github.com/cpp-core/timer/actions/workflows/build.yaml)

`core::timer::Timer` is a simple timer for `C++`.

## Example

```c++
#include <iomanip>
#include <iostream>
#include "core/timer/timer.h"

using namespace core;

int main(int argc, const char *argv[]) {

    unsigned int output{};
    auto ns = timer::Timer().run(1'000'000, [&]() {
	output += 1;
	timer::doNotOptimizeAway(output);
    }).elapsed_per_iteration();
    
    std::cout << std::setprecision(2) << ns << " nanoseconds per operation" << std::endl;
    // 0.31 nanoseconds per operation
	
    return 0;
}
```

## Build

    git clone git@github.com:cpp-core/timer
    mkdir timer/build && cd timer/build
    CC=clang-15 CXX=clang++-15 cmake -DCMAKE_INSTALL_PREFIX=$HOME/opt ..
    make check
