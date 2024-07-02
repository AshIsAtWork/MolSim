//
// Created by daniel on 30.06.24.
//

#include "CSVWriter.h"

#include <spdlog/spdlog.h>
#include <sys/stat.h>

CSVWriter::CSVWriter(int numberOfBins) : numberOfBins{numberOfBins} {
    struct stat info;
    if( stat( "statistics", &info ) != 0 ) {
        spdlog::info("Directory statistics does not exist. Creating directory...");
        if(mkdir("statistics",0777) != 0) {
            spdlog::error("Unable to create directory statistics");
        }else {
            spdlog::info("Directory statistics successfully created");
        }
    }
    spdlog::info("Open csv file...");
    file = std::ofstream{"statistics/Profile.csv"};
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open csv file");
    }else {
        spdlog::info("CSV file opened successfully...");
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
    for(int i = 0; i < numberOfBins; i++) {
        file << velocities[i];
        if(i < numberOfBins - 1) {
            file << ",";
        }
        else {
            file << "\n";
        }
    }
    return 0;
}
