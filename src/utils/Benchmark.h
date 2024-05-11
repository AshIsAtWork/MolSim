//
// Created by daniel on 07.05.24.
//
#pragma once

#include <iostream>
#include "utils/Logging.h"

/**
 * @brief Measure the execution time of the simulation
 *
 * @param simulator Simulator on which the simulation will run
 *
 * During the time measurement all output (logs and file output) is disabled
 */
inline void performBenchmark(Simulator &simulator) {
    spdlog::set_level(spdlog::level::off);
    auto tStart = std::chrono::steady_clock::now();
    simulator.run(true);
    auto tEnd = std::chrono::steady_clock::now();
    std::chrono::nanoseconds duration_ns{tEnd - tStart};
    double duration_s = static_cast<double>(duration_ns.count()) / 1e9;
    long duration_min = duration_ns.count() / 60000000000;
    long duration_min_sec = static_cast<long>(std::ceil(duration_s)) - duration_min * 60;
    std::cout << "Execution time: " << duration_min << " min " << duration_min_sec << " sec | "
            << duration_s << " sec | " << duration_ns.count() << " ns.\n";
}
