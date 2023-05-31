// Copyright (C) 2022, 2023 by Mark Melton
//

#undef NDEBUG
#include <cassert>

#include <iostream>
#include <random>
#include <vector>
#include <fmt/format.h>
#include "core/sort/record_iterator.h"

using namespace core::sort;
using std::cout, std::endl;

void output_records(const std::vector<int>& data, int nrows, int ncols) {
    for (auto i = 0; i < nrows; ++i) {
	const int *row = &data[i * ncols];
	for (auto j = 0; j < ncols; ++j)
	    cout << fmt::format("{:2d}", row[j]) << " ";
	cout << endl;
    }
}

int main(int argc, const char *argv[]) {
    // Generate 10k records each with 7 integer numbered sequentially.
    int nrows = 9, ncols = 5;
    std::vector<int> data(nrows * ncols);
    std::generate(data.begin(), data.end(), [n=0]() mutable { return n++; });
    cout << endl << "sequential: " << endl;
    output_records(data, nrows, ncols);

    // Shuffle the records
    std::shuffle(begin_record(data, ncols), end_record(data, ncols), std::mt19937_64{});
    cout << endl << "shuffled: " << endl;
    output_records(data, nrows, ncols);
    
    // Sort the records
    RecordIterator<int, false, true> begin(data.data(), ncols);
    RecordIterator<int, false, true> end(begin + nrows);
    std::sort(begin, end,
	      [](const int *a, const int *b) {
		  return a[0] < b[0];
	      });
    cout << endl << "sorted: " << endl;
    output_records(data, nrows, ncols);
    
    return 0;
}