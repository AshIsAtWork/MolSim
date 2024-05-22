//
// Created by daniel on 21.05.24.
//

#pragma once
#include <vector>

#include "fileHandling/outputWriter/VTKWriter.h"
#include "moleculeSimulator/forceCalculation/Force.h"
#include "particle/Particle.h"

class LinkedCellsContainer {
private:
    //Data structure
    std::vector<std::vector<Particle>> cells;
    int size;

    //Precomputed indizes
    std::vector<int> haloCells;
    std::array<std::vector<int>, 6> boundries;
    std::vector<std::vector<int>> domainCellIterationScheme;

    //Parameters
    int nX;
    int nY;
    int nZ;
    double cellSizeX;
    double cellSizeY;
    double cellSizeZ;
    double rCutOff;
    bool twoD;
    std::array<double, 3> domainSize;

    outputWriter::VTKWriter vtk_writer;

    //Helper methods

    void calculateHaloCellIndizes();
    void calculateBoundryCellIndizes();
    void calculateDomainCellsIterationScheme();

public:

    LinkedCellsContainer(std::array<double, 3> domainSize, double rCutOff);
    int calcCellIndex(const std::array<double, 3>& position);
    void add(Particle& p);
    void updateForces(Force& force);

    void updateVelocities(double deltaT);

    void updatePositions(double deltaT);

    int threeDToOneD(int x, int y, int z) const;

    std::array<int, 3> oneDToThreeD(int index) const;

    void plot(int iteration);

    void updateCells();

    //getter for tests

    std::vector<std::vector<Particle>>& getCells(){
        return cells;
    }

    std::vector<int>& getHaloCells(){
        return haloCells;
    }

   std::array<std::vector<int>,6>& getBoundries(){
        return boundries;
    }


    std::vector<std::vector<int>>& getDomainCellIterationScheme(){
        return domainCellIterationScheme;
    }

    int getNX() const {
        return nX;
    }

    int getNY() const {
        return nY;
    }

    int getNZ() const {
        return nZ;
    }

    double getCellSizeX() const {
        return cellSizeX;
    }

    double getCellSizeY() const {
        return cellSizeY;
    }

    double getCellSizeZ() const {
        return cellSizeZ;
    }

    double getRCutOff() const {
        return rCutOff;
    }

    bool isTwoD() const {
        return twoD;
    }

    std::array<double, 3> getDomainSize() const {
        return domainSize;
    }
};

