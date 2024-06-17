//
// Created by daniel on 24.04.24.
//

#include "FileHandler.h"

#include "outputWriter/TXTWriter/TxtWriter.h"

void FileHandler::readFile(ParticleContainer &particles, std::string &filePath, inputFormat format) {

    switch (format) {
        case inputFormat::txt: {
            if (FileReader::readFile(particles, filePath) != 0) {
                throw std::invalid_argument("Exception during reading the txt occurred");
            }
        }break;
        //TODO: Move XML from MolSim.cpp to here
        default: {
            throw std::invalid_argument("Invalid Input Format Selected");
        }
    }
}

void FileHandler::writeToFile(ParticleContainer &particles, int iteration, outputFormat format, std::string &baseName) {
    switch (format) {
        case outputFormat::xyz: {
            xyzWriter.plotParticles(particles, baseName, iteration);
        }
        break;
        case outputFormat::vtk: {
            vtkWriter.initializeOutput(static_cast<int>(particles.size()));
            particles.applyToEachParticle([this](Particle &p) {
                vtkWriter.plotParticle(p);
            });
            vtkWriter.writeFile(baseName, iteration);
        }
        break;
        case outputFormat::txt: {
            TxtWriter::writeToFile(particles,baseName);
            break;
        default:
            throw std::invalid_argument("Invalid Output Format Selected");
        }
    }
}
