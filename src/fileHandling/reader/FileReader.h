//
// Created by daniel on 05.05.24.
//

#pragma once

#include "particleRepresentation/ParticleContainer.h"
#include <cstdlib>
#include <fstream>
#include <sstream>
#include "moleculeSimulator/particleGeneration/ParticleGenerator.h"


/**
 * @brief Class for reading particles from txt files. At the moment file formats Cuboid and Particle are supported.
 */

class FileReader {
private:
    /**
     * @brief Helper method to read files containing cuboids.
     *
     * @param particles Particle Container to store the particles in.
     * @param input_file Input file stream being currently at the position of that line, in which the number of cuboids is specified.
     *
     * Only parses files correctly which adhere to the particle format. If the file pretends to contain
     * valid cuboid data, but violates the input format for some reason, this method may provide erroneous data.
     * If an error is detected, this function returns -1. The key word Cuboid in the first non comment
     * line of a file indicates the cuboid file format. Look at 2D-cuboid.collision.txt for reference.
     *
     *@return 0, if the operation was successful, -1 otherwise
     *
     */
    static int readCuboid(ParticleContainer &particles, std::ifstream &input_file);

    /**
     * @brief Helper method to read files containing particles.
     *
     * @param particles Particle Container to store the particles in.
     * @param input_file Input file stream being currently at the position of that line, in which the number of particles is specified.
     *
     * Only parses files correctly which adhere to the particle format. If the file pretends to contain
     * valid particle data, but violates the input format for some reason, this method may provide erroneous data.
     * If an error is detected, this function returns -1. The key word Particle in the first non comment
     * line of a file indicates the particle file format. Look at eingabe-sonne.txt for reference.
     *
     * @return 0, if the operation was successful, -1 otherwise
     */
    static int readParticle(ParticleContainer &particles, std::ifstream &input_file);

public:
    /**
     * @brief Read txt files containing cuboid or particle data.
     *
     * @param particles Particle Container to store the particles in.
     * @param filename Path to the file to be read.
     *
     * Valid file formats are Cuboid and Particle. The format of a txt file must be specified in the first line
     * that is not a comment. Look at the files in the input file for more information. Any other file format will
     * be rejected. If the specified file format is violated, the data may not be read correctly.
     *
     * @return 0, if the operation was successful, -1 if the file format is violated, -2 if the file could not be opened and -3 if the
     * the file format is unknown
     */
    static int readFile(ParticleContainer &particles, const std::string &filename);
};
