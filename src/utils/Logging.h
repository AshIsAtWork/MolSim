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
    if(logLevel == "off") {
        spdlog::set_level(spdlog::level::off);
        return true;
    }
    if(logLevel == "critical") {
        spdlog::set_level(spdlog::level::critical);
        return true;
    }
    if(logLevel == "error") {
        spdlog::set_level(spdlog::level::err);
        return true;
    }
    if(logLevel == "warn") {
        spdlog::set_level(spdlog::level::warn);
        return true;
    }
    if(logLevel == "info") {
        spdlog::set_level(spdlog::level::info);
        return true;
    }
    if(logLevel == "debug") {
        spdlog::set_level(spdlog::level::debug);
        return true;
    }
    if(logLevel == "trace") {
        spdlog::set_level(spdlog::level::trace);
        return true;
    }
    return false;
}


