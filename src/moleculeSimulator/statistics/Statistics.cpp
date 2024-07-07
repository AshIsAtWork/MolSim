//
// Created by daniel on 29.06.24.
//

#include "Statistics.h"

Statistics::Statistics(int numberOfBins) : csvVelocityWriter{numberOfBins, "VelocityProfile"},
                                           csvDensityWriter{numberOfBins, "DensityProfile"},
                                           numberOfBins{numberOfBins} {
}

void Statistics::calculateVelocityAndDensityProfile(ParticleContainer &particles, std::array<double, 3> dimensions,
                                                    double time) {
    //Store velocities along the y-axis, densities per bin in a vector and number of particles per bin
    std::vector<double> avgVelocities;
    std::vector<double> densities;
    std::vector<int> numbers;
    //Initialize vectors
    avgVelocities.assign(numberOfBins, 0.0);
    densities.assign(numberOfBins, 0.0);
    numbers.assign(numberOfBins, 0);

    //Iterate over all particles and sum up velocities along the y-axis and densities
    particles.applyToEachParticleInDomain([dimensions,this,&avgVelocities, &densities, &numbers](Particle &p) {
        //Compute bin
        int bin = static_cast<int>(std::floor((numberOfBins / dimensions[0]) * p.getX()[0]));
        if (bin < 0 or bin >= numberOfBins) {
            throw std::runtime_error("Could not assign particle to a bin!");
        } else {
            //Skip fixed particles
            if (!p.isFixed()) {
                //Velocity along y-axis
                avgVelocities[bin] += p.getV()[1];
                //Mass
                densities[bin] += p.getM();
                //Increase count
                numbers[bin]++;
            }
        }
    });
    //Compute statisitcs if possible
    for (int i = 0; i < numberOfBins; i++) {
        if (numbers[i] != 0) {
            avgVelocities[i] /= numbers[i];
            //Divide mass by volume.
            densities[i] /= (dimensions[0] / numberOfBins) * dimensions[1] * dimensions[2];
        } else {
            //When not possible, we set the value to zero to achieve a good looking graph
            avgVelocities[i] = 0;
        }
    }
    //Write profiles to csv file
    csvVelocityWriter.writeProfile(avgVelocities, time);
    csvDensityWriter.writeProfile(densities, time);
}
