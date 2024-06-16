//
// Created by cle on 6/16/24.
//

#include "TxtWriter.h"

int TxtWriter::writeToFile(ParticleContainer &pc, std::string filename) {

    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        spdlog::error("Unable to open file");
        return 1;
    }

    outFile << "# xyz-coord, velocity, Force, Old force, mass, type, epsilon, sigma\n"
            << "Particle\n"
            << pc.size()
            << "\n";

    auto writeParticle = [&outFile](Particle &particle) {

        for (const auto &xj: particle.getX()) {
            outFile << xj << " ";
        }
        outFile << "   ";
        for (const auto &vj: particle.getV()) {
            outFile << vj << " ";
        }
        outFile << "   ";
        for (const auto &fj: particle.getF()) {
            outFile << fj << " ";
        }
        outFile << "   ";
        for (const auto &ofj: particle.getOldF()) {
            outFile << ofj << " ";
        }

        outFile << "   "
                << particle.getM() << "   "
                << particle.getType() << "   "
                << particle.getEpsilon() << "   "
                << particle.getSigma() << "\n";
    };

    pc.applyToEachParticle(writeParticle);
    outFile.close();

    spdlog::info("Particles written to {}", filename);
    return 0;
}
