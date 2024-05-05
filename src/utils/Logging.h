//
// Created by daniel on 05.05.24.
//

#pragma once
#include <string>

#include "spdlog/spdlog-inl.h"

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


