//
// Created by Ashutosh Solanki on 23.05.24.
//

#include "particleRepresentation/container/ParticleContainer.h"
#include "fileHandling/reader/XMLHandling/Collision.h"
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include "../reader/XMLHandling/Collision.h"

class XMLWriter {

    /**
     * @brief Write the particle data to a xml file.
     *
     * @param particles Particle Container to read the particles from.
     * @param filename Path to the file to be written.
     *
     * Writes the particles to a xml file.
     */
    static void writeToFile(ParticleContainer &particles, std::string &filename);

};
