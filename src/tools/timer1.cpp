// Copyright (C) 2023 by Mark Melton
//

#include <iostream>
#include "core/timer/timer.h"

using namespace core;

int main(int argc, const char *argv[]) {
    
    timer::Timer timer;
    timer.start();
    unsigned int output{};
    for (auto i = 0; i < 10'000; ++i)
	output ^= i;
    timer.stop();

    auto ns = timer.elapsed().count();
    std::cout << ns << " nanoseconds" << std::endl;
    std::cout << output << std::endl;
    // 3150 nanoseconds
	
    return 0;
}
