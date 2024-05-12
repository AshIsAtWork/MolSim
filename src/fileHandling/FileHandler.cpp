//
// Created by daniel on 24.04.24.
//

#include "FileHandler.h"

void FileHandler::readFile(ParticleContainer &particles, std::string &filePath) {
    if (FileReader::readFile(particles, filePath) != 0) {
        exit(1);
    }
}

void FileHandler::writeToFile(ParticleContainer &particles, int iteration, outputFormat format) {
    switch (format) {
        case outputFormat::xyz: {
            xyzWriter.plotParticles(particles, fileName, iteration);
        }
        break;
        case outputFormat::vtk: {
            vtkWriter.initializeOutput(static_cast<int>(particles.size()));
            for (Particle &p: particles) {
                vtkWriter.plotParticle(p);
            }
            vtkWriter.writeFile(fileName, iteration);
        }
    }
}
