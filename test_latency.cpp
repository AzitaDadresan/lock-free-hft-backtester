// Unit tests for lock-free HFT backtester latency

#include <iostream>
#include <chrono>
#include <cassert>
#include <thread>
#include <atomic>

constexpr int TEST_PATHS = 10000;
std::atomic<int> counter{0};

void dummy_worker() {
    for (int i = 0; i < TEST_PATHS; ++i) {
        counter.fetch_add(1, std::memory_order_relaxed);
    }
}

void test_atomic_latency() {
    auto start = std::chrono::high_resolution_clock::now();
    
    std::thread t1(dummy_worker);
    std::thread t2(dummy_worker);
    t1.join();
    t2.join();
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    
    double latency_per_op = duration.count() / (2.0 * TEST_PATHS);
    std::cout << "Atomic operation latency: " << latency_per_op << " ns\n";
    
    assert(counter.load() == 2 * TEST_PATHS);
    assert(latency_per_op < 1000);
    std::cout << "✓ Latency test passed\n";
}

int main() {
    test_atomic_latency();
    return 0;
}
