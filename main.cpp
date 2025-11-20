// Lock-free HFT backtester with Monte Carlo VaR simulation
// Author: Azita Dadresan | CQF, Fidelity anomaly detection background

#include <iostream>
#include <vector>
#include <atomic>
#include <thread>
#include <random>
#include <algorithm>
#include <chrono>
#include <cmath>

constexpr int NUM_THREADS = 4;
constexpr int PATHS_PER_THREAD = 250000;
constexpr double S0 = 100.0;
constexpr double MU = 0.05;
constexpr double SIGMA = 0.2;
constexpr double T = 1.0/252;
constexpr int STEPS = 10;
constexpr double DT = T / STEPS;

std::atomic<double> global_pnl_sum{0.0};
std::vector<double> pnl_results;

void monte_carlo_worker(int thread_id, int num_paths) {
    std::mt19937 gen(thread_id * 12345);
    std::normal_distribution<> dist(0.0, 1.0);
    
    std::vector<double> local_pnls;
    local_pnls.reserve(num_paths);
    
    for (int path = 0; path < num_paths; ++path) {
        double S = S0;
        for (int step = 0; step < STEPS; ++step) {
            double Z = dist(gen);
            S *= std::exp((MU - 0.5 * SIGMA * SIGMA) * DT + SIGMA * std::sqrt(DT) * Z);
        }
        double pnl = S - S0;
        local_pnls.push_back(pnl);
    }
    
    double local_sum = std::accumulate(local_pnls.begin(), local_pnls.end(), 0.0);
    double expected = global_pnl_sum.load();
    while (!global_pnl_sum.compare_exchange_weak(expected, expected + local_sum));
    
    pnl_results.insert(pnl_results.end(), local_pnls.begin(), local_pnls.end());
}

void compute_risk_metrics(const std::vector<double>& pnls) {
    std::vector<double> sorted_pnls = pnls;
    std::sort(sorted_pnls.begin(), sorted_pnls.end());
    
    int var_idx = static_cast<int>(0.05 * sorted_pnls.size());
    double var_95 = sorted_pnls[var_idx];
    
    double cvar_sum = 0.0;
    for (int i = 0; i <= var_idx; ++i) {
        cvar_sum += sorted_pnls[i];
    }
    double cvar_95 = cvar_sum / (var_idx + 1);
    
    std::cout << "\n=== Risk Metrics (1M Paths, 4 Threads) ===\n";
    std::cout << "Mean PnL: $" << global_pnl_sum.load() / pnls.size() << "\n";
    std::cout << "95% VaR: $" << var_95 << "\n";
    std::cout << "95% CVaR: $" << cvar_95 << "\n";
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    
    std::vector<std::thread> threads;
    pnl_results.reserve(NUM_THREADS * PATHS_PER_THREAD);
    
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back(monte_carlo_worker, i, PATHS_PER_THREAD);
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Execution Time: " << duration.count() / 1e6 << " seconds\n";
    std::cout << "Latency per path: " << duration.count() / (NUM_THREADS * PATHS_PER_THREAD) << " microseconds\n";
    
    compute_risk_metrics(pnl_results);
    
    return 0;
}
