// Copyright (C) 2023 by Mark Melton
//

#pragma once
#include <atomic>
#include <chrono>

namespace core::timer {

template <typename T>
inline void doNotOptimizeAway(const T& val) {
    asm volatile("" : : "r,m"(val) : "memory");
}

#ifdef __clang__
template<class T>
inline void doNotOptimizeAway(T& value) {
    asm volatile("" : "+r,m"(value) : : "memory");
}
#else
template<class T>
inline void doNotOptimizeAway(T& value) {
    asm volatile("" : "+m,r"(value) : : "memory");
}
#endif

inline void doNotReorderBarrier() {
    std::atomic_signal_fence(std::memory_order_acq_rel);
}

template<class Duration = std::chrono::nanoseconds,
	 class Clock = std::chrono::high_resolution_clock>
class Timer {
public:
    using TimePoint = typename Clock::time_point;

    template<class Code>
    Timer& run(size_t n, Code&& code) {
	start();
	for (auto i = 0ul; i < n; ++i) {
	    code();
	}
	stop(n);
	return *this;
    }

    void start() {
	start_ = Clock::now();
    }

    auto stop(size_t n = 1) {
	auto end = Clock::now();
	iterations_ += n;
	elapsed_ += std::chrono::duration_cast<Duration>(end - start_);
	return elapsed_;
    }

    auto elapsed_per_iteration() const {
	return iterations_ > 0 ? (double)elapsed_.count() / iterations_ : 0.0;
    }

    auto elapsed() const {
	return elapsed_;
    }

private:
    TimePoint start_{};
    Duration elapsed_{};
    size_t iterations_{};
};

}; // core::timer
