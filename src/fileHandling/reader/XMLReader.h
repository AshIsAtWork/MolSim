//
// Created by Ashutosh Solanki on 22.05.24.
//

#pragma once

#include "particleRepresentation/container/ParticleContainer.h"
#include "fileHandling/reader/XMLHandling/Collision.h"
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include "XMLHandling/Collision.h"
#include "utils/ParameterPassing.h"

class XMLReader {

public:
    /**
     * @brief Read xml files containing particle data.
     *
     * @param particles Particle Container to store the particles in.
     * @param filename Path to the file to be read.
     *
     * The format of a xml file must be specified in the first line
     * that is not a comment. Look at the files in the input file for more information. Any other file format will
     * be rejected. If the specified file format is violated, the data may not be read correctly.
     */
    static int readFile(ParticleContainer &particles, std::string &filename);
};
