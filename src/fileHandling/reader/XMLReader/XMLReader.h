//
// Created by Ashutosh Solanki on 22.05.24.
//

#pragma once

#include "particleRepresentation/container/ParticleContainer.h"
#include "fileHandling/reader/XMLHandling/ConfigurationFile.h"
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include "utils/enumsStructs.h"

class XMLReader {

public:
    /**
     * @brief Read xml files containing parameters for simulation
     *
     * @param simulationSettings Struct to load all configurations in that have been read out of the input file.
     * @param filename Path to the file to be read.
     *
     */
    static int readFile(std::string &filename, enumsStructs::SimulationSettings& simulationSettings);
};
