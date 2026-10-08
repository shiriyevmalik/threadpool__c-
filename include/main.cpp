#include <iostream>
#include <vector>
#include <numeric>
#include <chrono>
#include "ThreadPool.hpp"

long long sum_chunk(const std::vector<int>& data, size_t start, size_t end) {
    long long t = 0;
    for (size_t i=start; i<end; ++i) {
        t+=data[i];
    }
    return t;
}

int main() {
    const size_t TOTAL = 10'000'000;
    std::cout << TOTAL << " integers exists\n";

    std::vector<int> numbers(TOTAL_ELEMENTS);
    for (size_t i=0; i<TOTAL; i++) numbers[i]=(i%100)+1;
    auto start_time = std::chrono::high_resolution_clock::now();
    long long sequential_sum = sum_chunk(numbers, 0, TOTAL);

    auto end_time=std::chrono::high_resolution_clock::now();
    auto seq_duration=std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time).count();

    std::cout << "\n[Single Thread] Sum: " << sequential_sum 
              << " | Time: " << seq_duration << " ms\n";

    const size_t num_workers=4;
    ThreadPool pool(num_workers);

    start_time=std::chrono::high_resolution_clock::now();

    size_t chunk_size=TOTAL
    std::vector<std::future<long long>> futures;

    for (size_t i=0; i<num_workers; i++) {
        size_t start_idx=i*chunk_size;
        size_t end_idx=(i==num_workers-1) ? TOTAL : start_idx+chunk_size;

        futures.push_back(
            pool.enqueue(sum_chunk, std::cref(numbers), start_idx, end_idx)
        );
    }

    long long parallel_sum=0;
    for (auto& fut : futures) parallel_sum+=fut.get();
    end_time=std::chrono::high_resolution_clock::now();
    auto par_duration=std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time).count();
    std::cout << "[ThreadPool (" << num_workers << " workers)] Sum: " << parallel_sum 
              << " | Time: " << par_duration << " ms\n";

    if (sequential_sum==parallel_sum) std::cout << "results mach";
    else std::cerr << "error";

    return 0;
}
