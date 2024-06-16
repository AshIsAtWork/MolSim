//
// Created by cle on 6/16/24.
//

#pragma once

#include <iostream>
#include <fstream>
#include "particleRepresentation/container/ParticleContainer.h"

class TxtWriter {
public:

    /**
     * @brief Writes particles form given particle container to a .txt file
     * @param pc Particle Container which contains the particles that should be written to the .txt file.
     * @param filename name of the file the contents of the particle container are written to.
     *          Important: do not call the file checkpointTest14562547.txt since it could somehow
     *          interfere with the remove command in the TxtWriterTest.cpp
     * @return 0, if the operation was successful, 1 otherwise
     */
    static int writeToFile(ParticleContainer &pc, std::string filename);
};
