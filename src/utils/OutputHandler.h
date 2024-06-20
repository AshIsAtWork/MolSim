//
// Created by Ashutosh Solanki on 22.05.24.
//

#pragma once

#include <string>

/**
 * @brief Sets the file format which is specified in the string.
 *
 * @param OutputFormat String containing the file format
 * @return FileHandler::outputFormat::vtk, if the file format is vtk,
 * FileHandler::outputFormat::xyz, if the file format is xyz,
 * FileHandler::outputFormat::invalid, if the file format is none of the above.
 */
inline FileHandler::outputFormat setOutputFormat(const std::string &OutputFormat) {
    static const std::unordered_map<std::string, FileHandler::outputFormat> formatMap = {
            {"vtk", FileHandler::outputFormat::vtk},
            {"xyz", FileHandler::outputFormat::xyz}
    };

    auto it = formatMap.find(OutputFormat);
    return (it != formatMap.end()) ? it->second : FileHandler::outputFormat::invalid;
}
