[![Build](https://github.com/cpp-core/timer/actions/workflows/build.yaml/badge.svg)](https://github.com/cpp-core/timer/actions/workflows/build.yaml)

`core::timer::Timer` is a header-only, micro-timer for modern
`C++`. The timer is designed to faciliate minimal-overhedad, ad-hoc
timing of `C++` code including micro-timing down to a single machine
instructions. `core::timer::Timer` is not designed for benchmarking
([benchmark](https://github.com/google/benchmark) or
[nanobench](https://github.com/martinus/nanobench) are excellent tools
for this purpose) nor for detailed performance profiling.

The timer has two basic modes:
- Inovking `run` on isolated code for in vivo measurements.
- Using `start` and `stop` timer calls for in vitro measurements.

## In Vivo Example

The `run` method is designed to repeatedly execute isoldated code in a
loop in order to measure the average cost per iteration. Modern
compilers, taking advantge of the `C++` standard's "as if" rule, will
often eliminate code that produces no visible side-effects making this
type of measurement difficult.

The timer library provides the `core::timer::doNotOptimizeAway`
function which forces the compiler to materialize the given expression
either in memory or in a register. This still allows the compiler to
fully optimize the expression given that constraint.

The following example demonstrates using the `run` method coupled with
the `doNotOptimizeAway` function to measure the cost of performing an
integer addition (i.e. `output += 1`). Without the `doNotOptimizeAway`
call, a typical outcome is for the compiler to eliminate all of the
computation since there are no visible side-effects.

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
