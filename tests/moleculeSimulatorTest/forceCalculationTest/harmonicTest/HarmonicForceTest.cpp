//
// Created by daniel on 30.06.24.
//

#include <gtest/gtest.h>

#include "fileHandling/reader/XMLHandling/ConfigurationFile.h"
#include "moleculeSimulator/Simulator.h"
#include "particleRepresentation/particle/Particle.h"
#include "utils/enumsStructs.h"

TEST(HarmonicForceTest, Equilibration) {
    Particle lowerLeft{{5, 0, 5}, {0, 0, 0}, 1, 1, 1, 1};
    Particle lowerRight{{7.3, 0, 5}, {0, 0, 0}, 1, 1, 1, 1};
    Particle upperLeft{{5, 0, 7.3}, {0, 0, 0}, 1, 1, 1, 1};
    Particle upperRight{{7.3, 0, 7.3}, {0, 0, 0}, 1, 1, 1, 1};
    lowerRight.addDiagonalNeighbor(upperLeft.getId());
    lowerRight.addDirectNeighbor(lowerLeft.getId());
    upperLeft.addDirectNeighbor(lowerLeft.getId());
    upperRight.addDiagonalNeighbor(lowerLeft.getId());
    upperRight.addDirectNeighbor(upperLeft.getId());
    upperRight.addDirectNeighbor(lowerRight.getId());
    lowerRight.addDirectNeighbor(lowerLeft.getId());

    enumsStructs::SimulationSettings simulationSettings;
    simulationSettings.outputFileName = ("Test");
    simulationSettings.model = enumsStructs::TypeOfModel::linkedCells;
    enumsStructs::BoundarySet boundaries{
        enumsStructs::BoundaryCondition::outflow, enumsStructs::BoundaryCondition::outflow,
        enumsStructs::BoundaryCondition::outflow, enumsStructs::BoundaryCondition::outflow,
        enumsStructs::BoundaryCondition::outflow, enumsStructs::BoundaryCondition::outflow
    };
    enumsStructs::LinkedCellsSimulationParameters lcc{
        0.01, 40, enumsStructs::TypeOfForce::leonardJonesForce, 100, {14, 14, 14}, boundaries
    };
    simulationSettings.parametersLinkedCells = lcc;
    simulationSettings.thermostatParameters.useThermostat = false;
    simulationSettings.gravityOn = false;
    simulationSettings.outputFrequency = 1;
    simulationSettings.membraneParameters.membraneSetting = true;
    simulationSettings.membraneParameters.pull = false;
    simulationSettings.membraneParameters.N1 = 0;
    simulationSettings.membraneParameters.N2 = 0;
    simulationSettings.membraneParameters.k = 300;
    simulationSettings.membraneParameters.r0 = 2.2;

    Simulator simulator = {simulationSettings,FileHandler::outputFormat::vtk};
    simulator.getParticles().add(lowerLeft);
    simulator.getParticles().add(lowerRight);
    simulator.getParticles().add(upperLeft);
    simulator.getParticles().add(upperRight);
    simulator.run(false);
}
