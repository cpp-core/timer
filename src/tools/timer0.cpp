// Copyright (C) 2023 by Mark Melton
//

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
