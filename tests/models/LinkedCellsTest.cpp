//
// Created by daniel on 28.05.24.
//

#include <gtest/gtest.h>

#include "../../src/models/linkedCells/LinkedCells.h"
#include "moleculeSimulator/forceCalculation/leonardJones/LeonardJonesForce.h"

using namespace enumsStructs;

bool isInDomain(Particle& p) {
 std::array<double, 3> lowerBound = {0,0,0};
 std::array<double, 3> upperBound = {3,3,3};
 return lowerBound <= p.getX() && p.getX() < upperBound;
}

bool noExplosion(Particle& p) {
 //The upper bound is set here arbitrarily to 1000. Is it not so clear what to choose here. For sure, the velocity of the particle should
 //not get to high which is an indicator for an explosion.
 return abs(p.getV()[0]) <= 10e3 && abs(p.getV()[1]) <= 10e3 && abs(p.getV()[2]) <= 10e3;
}

/**
 * Here we test, if the implementation of the reflective boundries does work.
 * Testing this thoroughly is difficult, because there are only qualitative requirements.
 * The key requirements are:
 *                          1.) No particle should leave the domain
 *                          2.) There should be no explosions
 *
 * To test this, we put one particle into a small domain, let it fly around and let the simulation run for some time.
 * After each step we check, if the particle is still within the domain and that velocities and forces do not exceed
 * some threshold
 *
 * --> One have to keep in mind to choose deltaT small enough to ensue that the method is stable. Otherwise, it could happen
 *     that the particle escapes the simulation domain, because the force applied by the virtual ghost particle has not been yet
 *     big enough to divert the particle from the border.
 */

TEST(LinkedCellsTest, ReflectiveBoundries) {
 LeonardJonesForce lJF(5, 1);
 std::pair<Side, enumsStructs::BoundaryCondition> front = {Side::front, enumsStructs::BoundaryCondition::reflective};
 std::pair<Side, enumsStructs::BoundaryCondition> right = {Side::right, enumsStructs::BoundaryCondition::reflective};
 std::pair<Side, enumsStructs::BoundaryCondition> back = {Side::back, enumsStructs::BoundaryCondition::reflective};
 std::pair<Side, enumsStructs::BoundaryCondition> left = {Side::left, enumsStructs::BoundaryCondition::reflective};
 std::pair<Side, enumsStructs::BoundaryCondition> top = {Side::top, enumsStructs::BoundaryCondition::reflective};
 std::pair<Side, enumsStructs::BoundaryCondition> bottom = {Side::bottom, enumsStructs::BoundaryCondition::reflective};


  std::array<std::pair<Side, enumsStructs::BoundaryCondition>, 6> boundrySettings= {front, right, back, left, top, bottom};

 LinkedCells linkedCellModel = {lJF,0.00005,{3,3,3},3, 1, FileHandler::inputFormat::txt, FileHandler::outputFormat::vtk, boundrySettings};
 double current_time = 0;

 int iteration = 0;

 linkedCellModel.updateForces();

 //We use a velocity vector here that will guarantee that all boundries are being hit.

 Particle p = {{1.5,1.5,1.5},{10,20,30},1};
 linkedCellModel.addParticle(p);

 while (current_time < 5) {
  linkedCellModel.step();
  iteration++;
  current_time += 0.00005;
  //Check conditions
  linkedCellModel.getParticles().applyToEachParticle([](Particle& p) {
   //We want to stop the simualtion immediately, if the particle is outside the domain or an explosion was detected
   ASSERT_TRUE(isInDomain(p));
   ASSERT_TRUE(noExplosion(p));
  });

 }
}

