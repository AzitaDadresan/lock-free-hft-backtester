# Lock-Free HFT Backtester

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![Build](https://img.shields.io/badge/build-passing-brightgreen.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)

**Sub-microsecond latency lock-free order book simulator with Monte Carlo path integration for HFT risk simulations.**

## Features
- **Lock-free order book**: Atomic operations for sub-µs latency
- **Thread pool**: 4 worker threads for parallel Monte Carlo simulation
- **1M paths**: Geometric Brownian Motion (GBM) price generation
- **VaR/CVaR**: 95% Value-at-Risk and Conditional VaR outputs
- **Fidelity-grade**: Anomaly-resistant design patterns

## Installation & Run

### Prerequisites
- CMake 3.15+
- C++17 compiler (GCC 9+, Clang 10+, MSVC 2019+)

### Build
```bash
mkdir build && cd build
cmake ..
make
./hft_backtester
```

### Run Tests
```bash
./test_latency
```

## Use Case
Ideal for quantitative analysts at prop trading firms (Citadel, Jane Street) evaluating execution algos under extreme market conditions. Ties to my Fidelity anomaly detection work (detecting order flow irregularities) and CQF Monte Carlo simulations.

## Author
Azita Dadresan | CQF, JHU TA (Multivariable Calculus, Itô's Lemma applications)

## License
MIT
