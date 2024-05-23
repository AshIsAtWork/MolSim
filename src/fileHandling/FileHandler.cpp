//
// Created by daniel on 24.04.24.
//

#include "FileHandler.h"

void FileHandler::readFile(ParticleContainer &particles, std::string &filePath, inputFormat format) {

    switch (format) {
        case inputFormat::txt: {
            if (FileReader::readFile(particles, filePath) != 0) {
                exit(1);
            }
        }
            break;
        case inputFormat::xml: {
            if (XMLReader::readFile(particles, filePath) != 0) {
                exit(1);
            }
        }
            break;
        case inputFormat::invalid: {
            spdlog::error("Invalid input format selected. Please select a valid input format.");
            spdlog::error("Code should not reach this point. Exiting program.");
            exit(1);
        }
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
            break;
        case outputFormat::invalid:
            spdlog::error("Invalid output format selected. Please select a valid output format.");
            spdlog::error("Code should not reach this point. Exiting program.");
            exit(1);
    }
}
