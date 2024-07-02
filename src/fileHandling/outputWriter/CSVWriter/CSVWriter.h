//
// Created by daniel on 30.06.24.
//

#pragma once
#include <fstream>
#include <vector>

/**
 * Writing profiles to a csv file.
 */
class CSVWriter {
private:
     std::ofstream file;
     int numberOfBins;

public:
     /**
      * @brief Construct new CSVWriter. A csv file is opened ones and closed, when this class is destructed.
      *
      * @param numberOfBins Number of bins to use.
      */
     explicit CSVWriter(int numberOfBins);

     /**
      * @brief When this object is destructed, the csv file is closed.
      */
     ~CSVWriter();

     /**
      * @brief Write one data set of a velocity profile to a csv file.
      *
      * @param velocities Average velocities along y-axis of particles in each bin
      * @param time Current time in the simulation.
      * @return 0 on success, on error some value unequal 0.
      */
     int writeVelocityProfile(std::vector<double>& velocities, double time);

};
