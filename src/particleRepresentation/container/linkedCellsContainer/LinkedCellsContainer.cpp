//
// Created by daniel on 21.05.24.
//

#include "LinkedCellsContainer.h"

using namespace enumsStructs;

void LinkedCellsContainer::calculateHaloCellIndizes() {
    for (int z = 0; z < nZ; z++) {
        //halo cells front
        for (int x = 0; x < nX; x++) {
            haloCells[0].push_back(threeDToOneD(x, 0, z));
        }
        //halo cells right
        for (int y = 0; y < nY; y++) {
            haloCells[1].push_back(threeDToOneD(nX - 1, y, z));
        }
        //halo cells back
        for (int x = 0; x < nX; x++) {
            haloCells[2].push_back(threeDToOneD(x, nY - 1, z));
        }
        //halo cells left
        for (int y = 0; y < nY; y++) {
            haloCells[3].push_back(threeDToOneD(0, y, z));
        }
    }

    if (!twoD) {
        for (int x = 0; x < nX; x++) {
            for (int y = 0; y < nY; y++) {
                //halo cells top (do only exist in three-dimensional space)
                haloCells[4].push_back(threeDToOneD(x, y, nZ - 1));
                //halo cells bottom (do only exist in three-dimensional space)
                haloCells[5].push_back(threeDToOneD(x, y, 0));
            }
        }
    }
}

void LinkedCellsContainer::calculateBoundaryCellIndizes() {
    //constants to handle 2D edge case
    int Z1 = !twoD;
    int Z2 = twoD ? 0 : nZ - 2;

    for (int z = Z1; z <= Z2; z++) {
        //boundary front
        for (int x = 1; x < nX - 1; x++) {
            boundaries[0].push_back(threeDToOneD(x, 1, z));
        }
        //boundary right
        for (int y = 1; y < nY - 1; y++) {
            boundaries[1].push_back(threeDToOneD(nX - 2, y, z));
        }
        //boundary back
        for (int x = 1; x < nX - 1; x++) {
            boundaries[2].push_back(threeDToOneD(x, nY - 2, z));
        }
        //boundary left
        for (int y = 1; y < nY - 1; y++) {
            boundaries[3].push_back(threeDToOneD(1, y, z));
        }
    }

    if (!twoD) {
        for (int x = 1; x < nX - 1; x++) {
            for (int y = 1; y < nY - 1; y++) {
                //boundary top (does only exist in three-dimensional space)
                boundaries[4].push_back(threeDToOneD(x, y, nZ - 2));
                //boundary bottom (does only exist in three-dimensional space)
                boundaries[5].push_back(threeDToOneD(x, y, 1));
            }
        }
    }
}

void LinkedCellsContainer::calculateDomainCellsIterationScheme() {
    //constants to handle 2D edge case
    int Z1 = !twoD;
    int Z2 = twoD ? 0 : nZ - 2;

    //initialize data structure
    int cellNumber = (nX - 2) * (nY - 2) * (twoD ? 1 : nZ - 2);
    domainCellIterationScheme.reserve(cellNumber);
    for (int i = 0; i < cellNumber; i++) {
        domainCellIterationScheme.emplace_back();
    }
    //calculate iteration route
    int index = 0;
    for (int z = Z1; z <= Z2; z++) {
        for (int y = 1; y < nY - 1; y++) {
            for (int x = 1; x < nX - 1; x++) {
                //First insert the cell itself
                domainCellIterationScheme[index].push_back(threeDToOneD(x, y, z));

                //Then insert all relevant neighbours
                //
                //         z-1         z           z+1
                //
                //       ^ ooo      ^ xxx        ^ xxx          c: current cell
                //     y | ooo    y | ocx     y  | xxx          x: ignored adjacent cells
                //       | ooo      | ooo        | xxx          o: considered adjacent cells
                //        ----->     ----->       ----->
                //         x          x            x

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
                        domainCellIterationScheme[index].push_back(threeDToOneD(x + 1, y - 1, z - 1));
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
                        domainCellIterationScheme[index].push_back(threeDToOneD(x + 1, y, z - 1));
                    }

                    //(x, y, z - 1)
                    domainCellIterationScheme[index].push_back(threeDToOneD(x, y, z - 1));
                }
                index++;
            }
        }
    }
}

double LinkedCellsContainer::calcDistanceFromBoundary(Particle &p, Side side) {
    switch (side) {
        case Side::front: {
            return p.getX()[1];
        }
        case Side::right: {
            return domainSize[0] - p.getX()[0];
        }
        case Side::back: {
            return domainSize[1] - p.getX()[1];
        }
        case Side::left: {
            return p.getX()[0];
        }
        case Side::top: {
            return domainSize[2] - p.getX()[2];
        }
        case Side::bottom: {
            return p.getX()[2];
        }
    }
    spdlog::error("A boundary was specified that does not exist!");
    return -1;
}

std::array<double, 3> LinkedCellsContainer::calcGhostParticle(Particle &p, Side side) {
    std::array<double, 3> position = p.getX();
    switch (side) {
        case Side::front: {
            position[1] -= 2 * p.getX()[1];
        }
        break;
        case Side::right: {
            position[0] += 2 * (domainSize[0] - p.getX()[0]);
        }
        break;
        case Side::back: {
            position[1] += 2 * (domainSize[1] - p.getX()[1]);
        }
        break;
        case Side::left: {
            position[0] -= 2 * p.getX()[0];
        }
        break;
        case Side::top: {
            position[2] += 2 * (domainSize[2] - p.getX()[2]);
        }
        break;
        case Side::bottom: {
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

    //Calculate number of cells in each dimension. We add here plus two, because each dimension contains two extra halo cells.
    //If the cutOff radius is smaller than the domain size, we set the cell number to 1 because we need at least one cell.
    nX = static_cast<int>(floor(domainSize[0] / rCutOff)) + 2 + (domainSize[0] < rCutOff ? 1 : 0);
    nY = static_cast<int>(floor(domainSize[1] / rCutOff)) + 2 + (domainSize[1] < rCutOff ? 1 : 0);
    //If we have only 2 dimensions, we define nZ := 1
    nZ = twoD ? 1 : static_cast<int>(floor(domainSize[2] / rCutOff)) + 2 + (domainSize[2] < rCutOff ? 1 : 0);

    //Calculate cell sizes
    cellSizeX = domainSize[0] / (nX - 2);
    cellSizeY = domainSize[1] / (nY - 2);
    cellSizeZ = twoD ? 0 : domainSize[2] / (nZ - 2);

    //Calculate y and z basis for faster index calculation and better code understanding
    baseY = nX;
    baseZ = nX * nY;

    //Initialize data structure
    int numberCells = nX * nY * nZ;
    cells.reserve(numberCells);
    for (int n = 0; n < numberCells; n++) {
        cells.emplace_back();
    }

    //Precalculate indizes for fast access in the future
    calculateHaloCellIndizes();
    calculateBoundaryCellIndizes();
    calculateDomainCellsIterationScheme();
}

int LinkedCellsContainer::calcCellIndex(const std::array<double, 3> &position) {
    int x, y, z;

    //Particles outside the domain are administered to their corresponding halo cell

    if (__isnan(position[0]) || __isnan(position[1]) || __isnan(position[2])) {
        spdlog::error(
            "Cannot calculate cell index of particle with position {}, because some value is none! Program will be terminated!",
            ArrayUtils::to_string(position));
        exit(-1);
    }

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
        return x + baseY * y;
    }

    if (position[2] < 0) {
        z = 0;
    } else if (position[2] >= domainSize[2]) {
        z = nY - 1;
    } else {
        z = static_cast<int>(floor(position[2] / cellSizeZ)) + 1;
    }

    return x + baseY * y + baseZ * z;
}

void LinkedCellsContainer::add(Particle &p) {
    //If the Linked Cell container is set to 2D it is not possible to add particles living in 3D space.
    if (twoD && (__fpclassify(p.getX()[2]) != FP_ZERO || __fpclassify(p.getV()[2]) != FP_ZERO ||
                 __fpclassify(p.getF()[2]) != FP_ZERO || __fpclassify(p.getOldF()[2]) != FP_ZERO)) {
        spdlog::error("Adding particles living in 3D space to a 2D Linked Cell container is not possible!");
        exit(-1);
    }
    int index = calcCellIndex(p.getX());
    cells[index].push_back(std::move(p));
    currentSize++;
}

int LinkedCellsContainer::threeDToOneD(int x, int y, int z) const {
    return x + baseY * y + baseZ * z;
}

std::array<int, 3> LinkedCellsContainer::oneDToThreeD(int index) const {
    int z = index / baseZ;
    index -= z * baseZ;
    int y = index / baseY;
    index -= y * baseY;
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

void LinkedCellsContainer::clearHaloCells(Side side) {
    for (auto cell: haloCells[static_cast<int>(side)]) {
        currentSize -= cells[cell].size();
        cells[cell].clear();
    }
}

void LinkedCellsContainer::applyToEachParticle(const std::function<void(Particle &)> &function) {
    for (auto &cell: cells) {
        for (Particle &p: cell) {
            function(p);
        }
    }
}

void LinkedCellsContainer::applyToEachParticleInDomain(const std::function<void(Particle &)> &function) {
    for (auto &cellGroup: domainCellIterationScheme) {
        for (auto &p: cells[cellGroup[0]]) {
            function(p);
        }
    }
}

void LinkedCellsContainer::applyToAllUniquePairsInDomain(const std::function<void(Particle &, Particle &)> &function) {
    for (auto &cellGroup: domainCellIterationScheme) {
        //First, consider all pairs within the cell that distance is smaller or equal then the cutoff radius

        for (auto p_i = cells[cellGroup[0]].begin(); p_i != cells[cellGroup[0]].end(); std::advance(p_i, 1)) {
            for (auto p_j = std::next(p_i); p_j != cells[cellGroup[0]].end(); std::advance(p_j, 1)) {
                if (ArrayUtils::L2Norm(p_i->getX() - p_j->getX()) <= rCutOff) {
                    function(*p_i, *p_j);
                }
            }
        }
        //Then, consider all relevant neighbour cells

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

void LinkedCellsContainer::applyToAllBoundryParticles(
    const std::function<void(Particle &, std::array<double, 3>&)> &function, Side boundary,
    double threshold) {
    for (auto cell: boundaries[static_cast<int>(boundary)]) {
        for (Particle &p: cells[cell]) {
            double distanceFromBoundry = calcDistanceFromBoundary(p, boundary);
            if (0 < distanceFromBoundry && distanceFromBoundry <= threshold) {
                auto ghostPosition = calcGhostParticle(p, boundary);
                function(p, ghostPosition);
            }
        }
    }
}
