/*
 * FileReader.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include "particleRepresentation/ParticleContainer.h"

class FileReader {

public:
    FileReader();

    virtual ~FileReader();

    void readFile(ParticleContainer &particles, std::string &filename);
};
