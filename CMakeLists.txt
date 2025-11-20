cmake_minimum_required(VERSION 3.15)
project(LockFreeHFT CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -pthread")

add_executable(hft_backtester main.cpp)
add_executable(test_latency test_latency.cpp)

target_link_libraries(hft_backtester pthread)
target_link_libraries(test_latency pthread)
