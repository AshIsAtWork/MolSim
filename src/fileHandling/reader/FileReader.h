//
// Created by daniel on 05.05.24.
//

#pragma once

#include "particleRepresentation/ParticleContainer.h"


class FileReader {
private:
    static void readCuboid(ParticleContainer &particles, std::ifstream& input_file);

    static void readParticle(ParticleContainer &particles, std::ifstream& input_file);


public:
    FileReader();

    virtual ~FileReader();

    static void readFile(ParticleContainer &particles, std::string &filename);
};

