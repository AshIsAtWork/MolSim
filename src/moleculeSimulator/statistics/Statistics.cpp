//
// Created by daniel on 29.06.24.
//

#include "Statistics.h"

Statistics::Statistics(int numberOfBins) : csvWriter{numberOfBins}, numberOfBins{numberOfBins}{}

void Statistics::calculateVelocityAndDensityProfile(ParticleContainer &particles, double lengthX, double time) {
    //Store velocities along the y-axis, densities per bin in a vector and number of particles per bin
    std::vector<double> avgVelocities;
    std::vector<double> avgDensities;
    std::vector<int> numbers;
    //Initialize vectors
    avgVelocities.assign(numberOfBins,0.0);
    avgDensities.assign(numberOfBins,0.0);
    numbers.assign(numberOfBins,0);

    //Iterate over all particles and sum up velocities along the y-axis and densities
    particles.applyToEachParticleInDomain([lengthX,this,&avgVelocities, &avgDensities, &numbers](Particle& p) {
       //Compute bin
       int bin = static_cast<int>(std::floor((numberOfBins / lengthX) * p.getX()[0]));
       if(bin < 0 or bin >= numberOfBins) {
          throw std::runtime_error("Could not assign particle to a bin!");
       }
       else {
           //Velocity along y-axis
           avgVelocities[bin] += p.getX()[1];
           numbers[bin]++;
       }
    });
    //Compute averages if possible
    for(int i = 0; i < numberOfBins; i++) {
        if(numbers[i] != 0) {
            avgVelocities[i] /= numbers[i];
        }else {
            avgVelocities[i] = NAN;
        }
    }
    //Write to csv file
    csvWriter.writeVelocityProfile(avgVelocities, time);
}
