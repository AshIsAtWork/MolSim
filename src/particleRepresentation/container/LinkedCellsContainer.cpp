//
// Created by daniel on 21.05.24.
//

#include "LinkedCellsContainer.h"

#include <iostream>

void LinkedCellsContainer::calculateHaloCellIndizes() {
    //For 2D
    if (twoD) {
        //bottom, top
        for (int x = 0; x < nX; x++) {
            haloCells.push_back(x);
            haloCells.push_back(x + nX * (nY - 1));
        }
        //left, right without bottom, top
        for (int y = 1; y < nY - 1; y++) {
            haloCells.push_back(nX * y);
            haloCells.push_back((nX - 1) + nX * y);
        }
    }
    //For 3D
    else {
        //side 1, 3
        for (int x = 0; x < nX; x++) {
            for (int z = 0; z < nZ; z++) {
                haloCells.push_back(x + nX * nY * z);
                haloCells.push_back(x + (nY - 1) * nX + nX * nY * z);
            }
        }
        //side 2, 4 without side 1, 3
        for (int y = 1; y < nY - 1; y++) {
            for (int z = 0; z < nZ; z++) {
                haloCells.push_back((nX - 1) + y * nX + nX * nY * z);
                haloCells.push_back(y * nX + nX * nY * z);
            }
        }
        //side 5,6 without side 1,2,3,4
        for (int x = 1; x < nX - 1; x++) {
            for (int y = 1; y < nY - 1; y++) {
                haloCells.push_back(x + y * nX);
                haloCells.push_back(x + y * nX + nX * nY * (nZ - 1));
            }
        }
    }
}

void LinkedCellsContainer::calculateBoundryCellIndizes() {
    //constants to handle 2D edge case
    int Z1 = !twoD;
    int Z2 = twoD ? 0 : nZ - 2;

    //boundry front
    for (int z = Z1; z <= Z2; z++) {
        for (int x = 1; x < nX - 1; x++) {
            boundries[0].push_back(x + nX + nX * nY * z);
        }
    }
    //boundry right
    for (int z = Z1; z <= Z2; z++) {
        for (int y = 1; y < nY - 1; y++) {
            boundries[1].push_back((nX - 2) + nX * y + nX * nY * z);
        }
    }
    //boundry back
    for (int z = Z1; z <= Z2; z++) {
        for (int x = 1; x < nX - 1; x++) {
            boundries[2].push_back(x + nX * (nY - 2) + nX * nY * z);
        }
    }
    //boundry left
    for (int z = Z1; z <= Z2; z++) {
        for (int y = 1; y < nY - 1; y++) {
            boundries[3].push_back(1 + nX * y + nX * nY * z);
        }
    }

    if (!twoD) {
        //boundry top (does only exist in three dimensional space)
        for (int x = 1; x < nX - 1; x++) {
            for (int y = 1; y < nY - 1; y++) {
                boundries[4].push_back(x + nX * y + nX * nY * (nZ - 2));
            }
        }
        //boundry bottom (does only exist in three dimensional space)
        for (int x = 1; x < nX - 1; x++) {
            for (int y = 1; y < nY - 1; y++) {
                boundries[5].push_back(x + nX * y + nX * nY);
            }
        }
    }
}

void LinkedCellsContainer::calculateDomainCellsIterationScheme() {
    //constants to handle 2D edge case
    int Z1 = !twoD;
    int Z2 = twoD ? 0 : nZ - 2;

    int cellNumber = (nX - 2) * (nY - 2) * (twoD ? 1 : nZ - 2);
    domainCellIterationScheme.reserve(cellNumber);
    for (int i = 0; i < cellNumber; i++) {
        domainCellIterationScheme.emplace_back();
    }

    int index = 0;
    for (int z = Z1; z <= Z2; z++) {
        for (int y = 1; y < nY - 1; y++) {
            for (int x = 1; x < nX - 1; x++) {
                //First insert the cell itself
                domainCellIterationScheme[index].push_back(threeDToOneD(x, y, z));

                //Then insert all relevant neighbours

                //(x-1, y-1, z)
                if (x >= 2 && y >= 2) {
                    domainCellIterationScheme[index].push_back(threeDToOneD(x - 1, y - 1, z));
                }
                //(x-1, y, z)
                if (x >= 2) {
                    domainCellIterationScheme[index].push_back(threeDToOneD(x - 1, y, z));
                }
                //(x, y-1, z)
                if (y >= 2) {
                    domainCellIterationScheme[index].push_back(threeDToOneD(x, y - 1, z));
                }
                //(x+1, y-1, z)
                if (x <= nX - 3 && y >= 2) {
                    domainCellIterationScheme[index].push_back(threeDToOneD(x + 1, y - 1, z));
                }
                //For 3D possibly up to 9 more cells do exist.
                if (z >= 2) {
                    //(x - 1, y - 1, z - 1)
                    if (x >= 2 && y >= 2) {
                        domainCellIterationScheme[index].push_back(threeDToOneD(x - 1, y - 1, z - 1));
                    }
                    //(x - 1, y, z - 1)
                    if (x >= 2) {
                        domainCellIterationScheme[index].push_back(threeDToOneD(x - 1, y, z - 1));
                    }
                    //(x, y - 1, z - 1)
                    if (y >= 2) {
                        domainCellIterationScheme[index].push_back(threeDToOneD(x, y - 1, z - 1));
                    }
                    //(x + 1, y - 1, z - 1)
                    if (x <= nX - 3 && y >= 2) {
                        domainCellIterationScheme[index].push_back(threeDToOneD(x, y - 1, z - 1));
                    }
                    //(x - 1, y + 1, z - 1)
                    if (x >= 2 && y <= nY - 3) {
                        domainCellIterationScheme[index].push_back(threeDToOneD(x - 1, y + 1, z - 1));
                    }
                    //(x, y + 1, z - 1)
                    if (y <= nY - 3) {
                        domainCellIterationScheme[index].push_back(threeDToOneD(x, y + 1, z - 1));
                    }

                    //(x + 1, y + 1, z - 1)
                    if (x <= nX - 3 && y <= nY - 3) {
                        domainCellIterationScheme[index].push_back(threeDToOneD(x + 1, y + 1, z - 1));
                    }

                    //(x + 1, y , z - 1)
                    if (x <= nX - 3) {
                        domainCellIterationScheme[index].push_back(threeDToOneD(x, y + 1, z - 1));
                    }

                    //(x, y, z - 1)
                    domainCellIterationScheme[index].push_back(threeDToOneD(x, y, z - 1));
                }
                index++;
            }
        }
    }
}

double LinkedCellsContainer::calcDistanceFromBoundry(Particle &p, Boundry boundry) {
    switch(boundry) {
        case Boundry::front : {
            return p.getX()[1];
        }
        case Boundry::right : {
            return domainSize[0] - p.getX()[0];
        }
        case Boundry::back : {
            return domainSize[1] - p.getX()[1];
        }
        case Boundry::left : {
            return p.getX()[0];
        }
        case Boundry::top : {
            return domainSize[2] - p.getX()[2];
        }
        case Boundry::bottom : {
            return p.getX()[2];
        }
    }
    spdlog::error("A boundry was specified that does not exist!");
    return -1;
}

std::array<double, 3> LinkedCellsContainer::calcGhostParticle(Particle &p, Boundry boundry) {
    std::array<double, 3> position = p.getX();
    switch(boundry) {
        case Boundry::front : {
            position[1] -= 2 * p.getX()[1];
        }break;
        case Boundry::right : {
            position[0] += 2 * (domainSize[0] - p.getX()[0]);
        }break;
        case Boundry::back : {
            position[1] += 2 * (domainSize[1] - p.getX()[1]);
        }break;
        case Boundry::left : {
            position[0] -= 2 * p.getX()[0];
        }break;
        case Boundry::top : {

            position[2] += 2 * (domainSize[2] - p.getX()[2]);
        }break;
        case Boundry::bottom : {
            position[2] -= 2 * p.getX()[2];
        }
    }
    return position;
}

LinkedCellsContainer::LinkedCellsContainer(std::array<double, 3> domainSize, double rCutOff) : currentSize{0},
                                                                                               rCutOff{rCutOff},
                                                                                               domainSize{domainSize} {
    if (domainSize[0] <= 0 || domainSize[1] <= 0 || domainSize[2] < 0) {
        spdlog::error("Domain size is invalid!");
        exit(-1);
    }

    //Determine if we are in 2D or 3D
    twoD = __fpclassify(domainSize[2]) == FP_ZERO;

    spdlog::info("Creating Linked Cell Container in {}D", twoD ? 2 : 3);

    //Calculate number of cells in each dimension. We add here plus two, because each dimension contains two exta halo cells.
    //If the cutOff radius is smaller than the domain size, we set the cell number to 1 because we need at least one cell.
    nX = static_cast<int>(floor(domainSize[0] / rCutOff)) + 2 + (domainSize[0] < rCutOff ? 1 : 0);
    nY = static_cast<int>(floor(domainSize[1] / rCutOff)) + 2 + (domainSize[1] < rCutOff ? 1 : 0);
    //If we have only 2 dimensions, we define nZ := 1
    nZ = twoD ? 1 : static_cast<int>(floor(domainSize[2] / rCutOff)) + 2 + (domainSize[2] < rCutOff ? 1 : 0);

    //Calculate cell sizes
    cellSizeX = domainSize[0] / (nX - 2);
    cellSizeY = domainSize[1] / (nY - 2);
    cellSizeZ = twoD ? 0 : domainSize[2] / (nZ - 2);

    //Initialize data structure
    cells.reserve(nX * nY * nZ);
    for (int n = 0; n < nX * nY * nZ; n++) {
        cells.emplace_back();
    }

    //Precalculate indizes for fast access in the future
    calculateHaloCellIndizes();
    calculateBoundryCellIndizes();
    calculateDomainCellsIterationScheme();
}

int LinkedCellsContainer::calcCellIndex(const std::array<double, 3> &position) {

    int x, y, z;
    //Particles outside the domain are administered to their corresponding halo cell
    if (position[0] < 0) {
        x = 0;
    } else if (position[0] >= domainSize[0]) {
        x = nX - 1;
    } else {
        x = static_cast<int>(floor(position[0] / cellSizeX)) + 1;
    }

    if (position[1] < 0) {
        y = 0;
    } else if (position[1] >= domainSize[1]) {
        y = nY - 1;
    } else {
        y = static_cast<int>(floor(position[1] / cellSizeY)) + 1;
    }

    if (twoD) {
        return x + nX * y;
    }

    if (position[2] < 0) {
        z = 0;
    } else if (position[2] >= domainSize[2]) {
        z = nY - 1;
    } else {
        z = static_cast<int>(floor(position[2] / cellSizeZ)) + 1;
    }

    return x + nX * y + nX * nY * z;
}

void LinkedCellsContainer::add(Particle &p) {
    //If the Linked Cell container is set to 2D it is not possible to add particles living in 3D space.
    if (twoD && (__fpclassify(p.getX()[2]) != FP_ZERO || __fpclassify(p.getV()[2]) != FP_ZERO ||
                 __fpclassify(p.getF()[2]) != FP_ZERO || __fpclassify(p.getOldF()[2]) != FP_ZERO)) {
        spdlog::error("Adding particles living in 3D space to a 2D Linked Cell container is not possible!");
    }
    int index = calcCellIndex(p.getX());
    cells[index].push_back(std::move(p));
    currentSize++;
}

int LinkedCellsContainer::threeDToOneD(int x, int y, int z) const {
    return x + nX * y + nX * nY * z;
}

std::array<int, 3> LinkedCellsContainer::oneDToThreeD(int index) const {
    int z = index / (nX * nY);
    index -= z * (nX * nY);
    int y = index / nX;
    index -= y * nX;
    return {index, y, z};
}

void LinkedCellsContainer::updateCells() {
    for (auto &index: domainCellIterationScheme) {
        for (auto p = cells[index[0]].begin(); p != cells[index[0]].end();) {
            int newIndex = calcCellIndex(p->getX());
            if (newIndex != index[0]) {
                cells[newIndex].push_back(*p);
                p = cells[index[0]].erase(p);
            } else {
                ++p;
            }
        }
    }
}

size_t LinkedCellsContainer::size() {
    return currentSize;
}

void LinkedCellsContainer::clearHaloCells() {
    for (auto cell: haloCells) {
        currentSize -= static_cast<int>(cells[cell].size());
        cells[cell].clear();
    }
}

void LinkedCellsContainer::applyToEachParticle(const std::function<void(Particle &)> &function) {
    for (auto cell: cells) {
        for (Particle &p: cell) {
            function(p);
        }
    }
}

void LinkedCellsContainer::applyToEachParticleInDomain(const std::function<void(Particle &)> &function) {
    for (auto cellGroup: domainCellIterationScheme) {
        for (auto &p: cells[cellGroup[0]]) {
            function(p);
        }
    }
}

void LinkedCellsContainer::applyToAllUniquePairsInDomain(const std::function<void(Particle &, Particle &)> &function) {
    for (auto cellGroup: domainCellIterationScheme) {
        //First, consider all pairs within the cell that distance is smaller or equal then the cutoff radius
        for (auto p_i = cells[cellGroup[0]].begin(); p_i != cells[cellGroup[0]].end(); std::advance(p_i, 1)) {
            for (auto p_j = std::next(p_i); p_j != cells[cellGroup[0]].end(); std::advance(p_j, 1)) {
                if (ArrayUtils::L2Norm(p_i->getX() - p_j->getX()) <= rCutOff) {
                    function(*p_i, *p_j);
                }
            }
        }
        //Then, consider all neighbour cells

        for (auto neighbour = cellGroup.begin() + 1; neighbour != cellGroup.end(); std::advance(neighbour, 1)) {
            for (auto &p_i: cells[cellGroup[0]]) {
                for (auto &p_j: cells[*neighbour]) {
                    if (ArrayUtils::L2Norm(p_i.getX() - p_j.getX()) <= rCutOff) {
                        function(p_i, p_j);
                    }
                }
            }
        }
    }
}

void LinkedCellsContainer::
applyToAllBoundryParticles(const std::function<void(Particle &, std::array<double, 3>)> &function, Boundry boundry, double threshhold) {
    for (auto cell: boundries[static_cast<int>(boundry)]) {
        for (Particle &p: cells[cell]) {
            double distanceFromBoundry = calcDistanceFromBoundry(p, boundry);
            if(0 < distanceFromBoundry && distanceFromBoundry < threshhold)
            function(p, calcGhostParticle(p, boundry));
        }
    }
}
