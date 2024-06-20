//
// Created by daniel on 05.05.24.
//

#include "TxtReader.h"

int FileReader::readCuboid(ParticleContainer &particles, std::ifstream &input_file) {
    std::array<double, 3> position{};
    unsigned N1;
    unsigned N2;
    unsigned N3;
    double h;
    double mass;
    int dimensions;
    double brownianMotionAverageVelocity;
    std::array<double, 3> velocity{};
    std::string tmpString;
    int numCuboids = 0;

    getline(input_file, tmpString);
    std::istringstream dimstream(tmpString);
    spdlog::debug("Read line: {}", tmpString);
    dimstream >> dimensions;
    spdlog::debug("Applying Brownian Motion to {} dimensions", dimensions);

    getline(input_file, tmpString);
    std::istringstream numstream(tmpString);
    spdlog::debug("Read line: {}", tmpString);
    numstream >> numCuboids;
    spdlog::debug("Reading {} cuboids", numCuboids);

    for (int i = 0; i < numCuboids; i++) {
        tmpString.clear();
        if (getline(input_file, tmpString).eof() && tmpString.empty()) {
            throw std::runtime_error(
                    "Error reading file: eof reached unexpectedly reading at line " + std::to_string(i));
        }
        spdlog::debug("Read line: {}", tmpString);
        std::istringstream dataStream(tmpString);

        for (auto &pos: position) {
            dataStream >> pos;
        }
        dataStream >> N1;
        dataStream >> N2;
        dataStream >> N3;
        dataStream >> h;
        dataStream >> mass;
        for (auto &v: velocity) {
            dataStream >> v;
        }

        if (dataStream.eof()) {
            throw std::runtime_error("Error reading file: eof reached unexpectedly reading at line " + std::to_string(i));
        }
        dataStream >> brownianMotionAverageVelocity;
        ParticleGenerator::generateCuboid(particles, position, N1, N2, N3, h, mass, velocity, dimensions,
                                          brownianMotionAverageVelocity);
    }
    return 0;
}

int FileReader::readParticle(ParticleContainer &particles, std::ifstream &input_file) {
    std::array<double, 3> x{};
    std::array<double, 3> v{};
    std::array<double, 3> f{};
    std::array<double, 3> old_f{};
    double m;
    int type;
    double epsilon;
    double sigma;
    std::string tmpString;
    int numParticles = 0;

    getline(input_file, tmpString);
    std::istringstream numStream(tmpString);
    spdlog::debug("Read line: {}", tmpString);
    numStream >> numParticles;
    spdlog::debug("Reading {} particles", numParticles);

    for (int i = 0; i < numParticles; i++) {
        tmpString.clear();
        getline(input_file, tmpString).eof();

        if (tmpString.empty()) {
            throw std::runtime_error("Error reading file: eof reached unexpectedly reading at line " + std::to_string(i));
        }
        spdlog::debug("Read line: {}", tmpString);
        std::istringstream dataStream(tmpString);

        for (auto &xj: x) {
            dataStream >> xj;
        }
        for (auto &vj: v) {
            dataStream >> vj;
        }
        for (auto &fj: f) {
            dataStream >> fj;
        }
        for (auto &old_fj: old_f) {
            dataStream >> old_fj;
        }
        if (dataStream.eof()) {
            throw std::runtime_error(
                    "Error reading file: eof reached unexpectedly reading at line " + std::to_string(i));
        }
        dataStream >> m;
        dataStream >> type;
        dataStream >> epsilon;
        dataStream >> sigma;
        Particle newP{x, v, m, type, epsilon, sigma};
        newP.setF(f);
        newP.setOldF(old_f);
        particles.add(newP);
    }
    return 0;
}

int FileReader::readFile(ParticleContainer &particles, const std::string &filename) {
    std::ifstream input_file(filename);
    std::string tmpString;

    if (input_file.is_open()) {
        spdlog::info("Opened file: {}", filename.find_first_of('/')
                                        != std::string::npos
                                        ? filename.substr(filename.find_last_of('/') + 1)
                                        : filename);
        if (getline(input_file, tmpString).eof()) {
            throw std::runtime_error("Error reading file: eof reached unexpectedly reading.");
        }
        spdlog::debug("Read line: {}", tmpString);

        while (tmpString.empty() or tmpString[0] == '#') {
            getline(input_file, tmpString);
            spdlog::debug("Read line: {}", tmpString);
        }

        if (tmpString == "Particle") {
            spdlog::info("File Format is Particle!");
            return readParticle(particles, input_file);
        }
        if (tmpString == "Cuboid") {
            spdlog::info("File Format is Cuboid!");
            return readCuboid(particles, input_file);
        }
        throw std::runtime_error("Unknown File Format");
    }
    throw std::runtime_error("Error opening file");
}


