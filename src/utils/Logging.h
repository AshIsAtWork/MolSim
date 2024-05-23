//
// Created by daniel on 05.05.24.
//

#pragma once
#include <string>
#include "spdlog/spdlog.h"

/**
 * @brief Sets the log level which is specified in the string.
 *
 * @param logLevel String containing the log level
 * @return true, of the log level is valid, false otherwise.
 */
inline bool setLogLevel(const std::string& logLevel) {
    static const std::unordered_map<std::string, spdlog::level::level_enum> levelMap = {
            {"off", spdlog::level::off},
            {"critical", spdlog::level::critical},
            {"error", spdlog::level::err},
            {"warn", spdlog::level::warn},
            {"info", spdlog::level::info},
            {"debug", spdlog::level::debug},
            {"trace", spdlog::level::trace}
    };

    auto it = levelMap.find(logLevel);
    if (it != levelMap.end()) {
        spdlog::set_level(it->second);
        return true;
    }
    return false;
}

