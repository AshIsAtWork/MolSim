//
// Created by daniel on 05.05.24.
//

#include "FileReader.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
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
    std::cout << "Read line: " << tmpString << std::endl;
    numstream >> numCuboids;
    std::cout << "Reading " << numCuboids << "." << std::endl;

    getline(input_file, tmpString);
    std::cout << "Read line: " << tmpString << std::endl;

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
            std::cout
                    << "Error reading file: eof reached unexpectedly reading from line "
                    << i << std::endl;
            exit(-1);
        }
        datastream >> brownianMotionAverageVelocity;
        ParticleGenerator::generateCuboid(particles, position, N1, N2, N3, h, mass, velocity);

        getline(input_file, tmpString);
        std::cout << "Read line: " << tmpString << std::endl;
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
    std::cout << "Read line: " << tmpString << std::endl;
    numstream >> numParticles;
    std::cout << "Reading " << numParticles << "." << std::endl;

    particles.reserve(numParticles);

    getline(input_file, tmpString);
    std::cout << "Read line: " << tmpString << std::endl;

    for (int i = 0; i < numParticles; i++) {
        std::istringstream datastream(tmpString);

        for (auto &xj: x) {
            datastream >> xj;
        }
        for (auto &vj: v) {
            datastream >> vj;
        }
        if (datastream.eof()) {
            std::cout
                    << "Error reading file: eof reached unexpectedly reading from line "
                    << i << std::endl;
            exit(-1);
        }
        datastream >> m;
        Particle newP{x, v, m, i};
        particles.add(newP);

        getline(input_file, tmpString);
        std::cout << "Read line: " << tmpString << std::endl;
    }
}

void FileReader::readFile(ParticleContainer &particles, std::string &filename) {

    std::ifstream input_file(filename);
    std::string tmpString;

    if (input_file.is_open()) {
        getline(input_file, tmpString);
        std::cout << "Read line: " << tmpString << std::endl;

        while (tmpString.empty() or tmpString[0] == '#') {
            getline(input_file, tmpString);
            std::cout << "Read line: " << tmpString << std::endl;
        }

        if (tmpString == "Particle") {
            std::cout << "File Format is Particle!\n";
            readParticle(particles, input_file);
        } else if (tmpString == "Cuboid") {
            std::cout << "File Format is Cuboid!\n";
            readCuboid(particles,input_file);
        } else {
            std::cout << "Error: Unknown file format!" << filename << std::endl;
            exit(-1);
        }

    } else {
        std::cout << "Error: could not open file " << filename << std::endl;
        exit(-1);
    }
}
