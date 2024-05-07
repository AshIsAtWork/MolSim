//
// Created by daniel on 05.05.24.
//

#pragma once

#include "particleRepresentation/ParticleContainer.h"
#include "spdlog/spdlog.h"


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
     * Only parses files correctly which adhere to the cuboid format. If the file pretends to contain
     * valid cuboid data, but violates the input format for some reason, the behaviour of this
     * function is undefined. The program however will not crash. The key word Cuboid in the first non comment
     * line of a file indicates the cuboid file format. Look at 2D-cuboid-collision.txt for reference.
     */
    static void readCuboid(ParticleContainer &particles, std::ifstream &input_file);

    /**
     * @brief Helper method to read files containing particles.
     *
     * @param particles Particle Container to store the particles in.
     * @param input_file Input file stream being currently at the position of that line, in which the number of particles is specified.
     *
     * Only parses files correctly which adhere to the particle format. If the file pretends to contain
     * valid particle data, but violates the input format for some reason, the behaviour of this
     * function is undefined. The program however will not crash. The key word Particle in the first non comment
     * line of a file indicates the particle file format. Look at eingabe-sonne.txt for reference.
     */
    static void readParticle(ParticleContainer &particles, std::ifstream &input_file);

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
     */
    static void readFile(ParticleContainer &particles, const std::string &filename);
};
