//
// Created by Ashutosh Solanki on 22.05.24.
//

#pragma once
#include <string>

/**
* @brief Sets the file format which is specified in the string.
*
* @param InputFormat String containing the file format
* @return FileHandler::inputFormat::txt, if the file format is txt,
 * FileHandler::inputFormat::xml, if the file format is xml,
 * FileHandler::inputFormat::invalid, if the file format is none of the above.
*/
inline FileHandler::inputFormat setInputFormat(const std::string &InputFormat) {
    static const std::unordered_map<std::string, FileHandler::inputFormat> formatMap = {
            {"txt", FileHandler::inputFormat::txt},
            {"xml", FileHandler::inputFormat::xml}
    };

    auto it = formatMap.find(InputFormat);
    return (it != formatMap.end()) ? it->second : FileHandler::inputFormat::invalid;
}

