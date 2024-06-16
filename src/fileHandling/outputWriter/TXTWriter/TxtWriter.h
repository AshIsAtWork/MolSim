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
     * @brief Writes particles form given particle container into a file called checkpoint.txt.
     * @param pc Particle Container which contains the particles that should be written to the .txt file.
     * @return 0, if the operation was successful, 1 otherwise
     */
    static int writeToFile(ParticleContainer &pc);
};
