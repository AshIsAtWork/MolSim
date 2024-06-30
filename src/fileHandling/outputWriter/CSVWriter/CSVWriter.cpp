//
// Created by daniel on 30.06.24.
//

#include "CSVWriter.h"

CSVWriter::CSVWriter(int numberOfBins) : file{"statistics/Profile.csv"} {
    //Open file
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file");
    }

    //Create description
    file << "time,";

    for(int i = 1; i <= numberOfBins; i++) {
        file << i;
        if(i!=numberOfBins) {
            file << ",";
        }
        else {
            file << "\n";
        }
    }
}

CSVWriter::~CSVWriter() {
    file.close();
}

int CSVWriter::writeVelocityProfile(std::vector<double> &velocities, double time) {
    file << time << ",";
    for(int i = 0; i < velocities.size(); i++) {
        file << velocities[i];
        if(i!=velocities.size()) {
            file << ",";
        }
        else {
            file << "\n";
        }
    }
    return 0;
}
