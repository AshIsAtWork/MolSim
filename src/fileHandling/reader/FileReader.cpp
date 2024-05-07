//
// Created by daniel on 05.05.24.
//

#include "FileReader.h"

#include <cstdlib>
#include <fstream>
#include <sstream>

#include "moleculeSimulator/particleGeneration/ParticleGenerator.h"

void FileReader::readCuboid(ParticleContainer &particles, std::ifstream &input_file) {
    std::array<double, 3> position{};
    unsigned N1;
    unsigned N2;
    unsigned N3;
    double h;
    double mass;
    double brownianMotionAverageVelocity;
    std::array<double, 3> velocity{};
    std::string tmpString;
    int numCuboids = 0;

    getline(input_file, tmpString);
    std::istringstream numstream(tmpString);
    spdlog::debug("Read line: {}", tmpString);
    numstream >> numCuboids;
    spdlog::debug("Reading {} cuboids", numCuboids);

    getline(input_file, tmpString);
    spdlog::debug("Read line: {}", tmpString);

    for (int i = 0; i < numCuboids; i++) {
        std::istringstream datastream(tmpString);

        for (auto &pos: position) {
            datastream >> pos;
        }
        datastream >> N1;
        datastream >> N2;
        datastream >> N3;
        datastream >> h;
        datastream >> mass;
        for (auto &v: velocity) {
            datastream >> v;
        }

        if (datastream.eof()) {
            spdlog::error("Error reading file: eof reached unexpectedly reading from line {}", i);
        }
        datastream >> brownianMotionAverageVelocity;
        ParticleGenerator::generateCuboid(particles, position, N1, N2, N3, h, mass, velocity);

        getline(input_file, tmpString);
        spdlog::debug("Read line: {}", tmpString);
    }
}

void FileReader::readParticle(ParticleContainer &particles, std::ifstream &input_file) {
    std::array<double, 3> x{};
    std::array<double, 3> v{};
    double m;
    std::string tmpString;
    int numParticles = 0;

    getline(input_file, tmpString);
    std::istringstream numstream(tmpString);
    spdlog::debug("Read line: {}", tmpString);
    numstream >> numParticles;
    spdlog::debug("Reading {} particles", numParticles);

    particles.reserve(numParticles);

    getline(input_file, tmpString);
    spdlog::debug("Read line: {}", tmpString);

    for (int i = 0; i < numParticles; i++) {
        std::istringstream datastream(tmpString);

        for (auto &xj: x) {
            datastream >> xj;
        }
        for (auto &vj: v) {
            datastream >> vj;
        }
        if (datastream.eof()) {
            spdlog::error("Error reading file: eof reached unexpectedly reading from line {}", i);
            exit(-1);
        }
        datastream >> m;
        Particle newP{x, v, m, i};
        particles.add(newP);

        getline(input_file, tmpString);
        spdlog::debug("Read line: {}", tmpString);
    }
}

void FileReader::readFile(ParticleContainer &particles, const std::string &filename) {
    std::ifstream input_file(filename);
    std::string tmpString;

    if (input_file.is_open()) {
        spdlog::info("Opened file: {}", filename.find_first_of('/')
                                        != std::string::npos
                                            ? filename.substr(filename.find_last_of('/') + 1)
                                            : filename);
        getline(input_file, tmpString);
        spdlog::debug("Read line: {}", tmpString);

        while (tmpString.empty() or tmpString[0] == '#') {
            getline(input_file, tmpString);
            spdlog::debug("Read line: {}", tmpString);
        }

        if (tmpString == "Particle") {
            spdlog::info("File Format is Particle!");
            readParticle(particles, input_file);
        } else if (tmpString == "Cuboid") {
            spdlog::info("File Format is Cuboid!");
            readCuboid(particles, input_file);
        } else {
            spdlog::error("Unknown file format!");
            exit(-1);
        }
    } else {
        spdlog::error("Could not open file!");
        exit(-1);
    }
}
