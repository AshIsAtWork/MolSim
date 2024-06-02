// ////
// //// Created by daniel on 11.05.24.
// ////
// #include <gtest/gtest.h>
// #include <spdlog/spdlog.h>
// #include "moleculeSimulator/Simulator.h"
// #include "moleculeSimulator/forceCalculation/gravity/Gravity.h"
// #include "moleculeSimulator/forceCalculation/leonardJones/LeonardJonesForce.h"
//
//
// /**
//  * Set up test environment for each test.
//  */
// class SimulatorTest : public testing::Test {
// protected:
//     /**
//      * @brief This method is used to simply comparisons in the following tests
//      *
//      * @param res calculated particle data
//      * @param pos expected position
//      * @param vel expected velocity
//      * @param f expected force
//      * @param fOld  expected old force
//      */
//     void compareResults(Particle &res, std::array<double, 3> &pos,
//                         std::array<double, 3> &vel, std::array<double, 3> &f, std::array<double, 3> &fOld) {
//         //Because our data for comparison is in 3 digits after the comma accurate, we use 1e-03 as tolerance.
//
//         for (int i = 0; i < 3; i++) {
//             EXPECT_NEAR(res.getX()[i], pos[i], 1e-3);
//             EXPECT_NEAR(res.getV()[i], vel[i], 1e-3);
//             EXPECT_NEAR(res.getF()[i], f[i], 1e-3);
//             EXPECT_NEAR(res.getOldF()[i], fOld[i], 1e-3);
//         }
//     }
//
//     void SetUp() override {
//         //Deactivate all console output
//         spdlog::set_level(spdlog::level::off);
//     }
// };
//
// /**
//  * Test simulation of the solar system.
//  *
//  * This test checks, if the simulation calculates everything correctly. The expected values have been calculted
//  * with the implementation of week two which we validated through ParaView and some calculation by hand. The idea of this test is that
//  * we can be sure that everything works still as expected after we maybe adapt our code in the next weeks.
//  */
//
// TEST_F(SimulatorTest, SolarSystem) {
//     Gravity gravity;
//     std::string filepath = "../input/assignment-1/eingabe-sonne.txt";
//     std::string name = "test";
//     DirectSumSimulationParameters dS = {0.1, 1, 5,1,TypeOfForce::gravity};
//     Simulator simulator{dS,filepath,FileHandler::inputFormat::txt, FileHandler::outputFormat::vtk,10,name};
//     //Set timeMeasurement to true to deactivate all output
//     simulator.run(true);
//     auto result = dynamic_cast<DefaultParticleContainer&>(simulator.getParticles());
//     //Here we make use of ASSERT. If this tests fails for some reason, the next steps will lead to problems, because .at()
//     //will fail with an out of range exepction
//     ASSERT_EQ(result.size(), 4);
//     std::array<double, 3> p1_pos = {-1.16691e-06, 2.15477e-05, 0};
//     std::array<double, 3> p1_vel = {-3.20442e-06, 3.91587e-05, 0};
//     std::array<double, 3> p1_f = {-5.50661e-06, 3.44596e-05, 0};
//     std::array<double, 3> p1_fOld = {-5.10375e-06, 3.47349e-05, 0};
//
//     compareResults(result.at(0), p1_pos, p1_vel, p1_f, p1_fOld);
//
//     std::array<double, 3> p2_pos = {-0.899859, 0.461499, 0};
//     std::array<double, 3> p2_vel = {-0.463112, -0.879696, 0};
//     std::array<double, 3> p2_f = {2.61018e-06, -1.33846e-06, 0};
//     std::array<double, 3> p2_fOld = {2.47022e-06, -1.59336e-06, 0};
//
//     compareResults(result.at(1), p2_pos, p2_vel, p2_f, p2_fOld);
//
//     std::array<double, 3> p3_pos = {-0.466907, 5.33913, 0};
//     std::array<double, 3> p3_vel = {-0.423331, -0.0382404, 0};
//     std::array<double, 3> p3_f = {2.89642e-06, -3.31211e-05, 0};
//     std::array<double, 3> p3_fOld = {2.63353e-06, -3.31415e-05, 0};
//
//     compareResults(result.at(2), p3_pos, p3_vel, p3_f, p3_fOld);
//
//     std::array<double, 3> p4_pos = {34.7495, 0.0325599, 0};
//     std::array<double, 3> p4_vel = {-0.000911766, 0.0295997, 0};
//     std::array<double, 3> p4_f = {-8.28884e-18, -6.63223e-21, 0};
//     std::array<double, 3> p4_fOld = {-8.28882e-18, -5.92234e-21, 0};
//
//     compareResults(result.at(3), p4_pos, p4_vel, p4_f, p4_fOld);
// }
//
// /**
//  * Test simulation of two cuboids.
//  *
//  * This test checks, if the simulation calculates everything correctly. The expected values have been calculted
//  * with the implementation of week two which we validated through ParaView and some calculation by hand. The idea of this test is that
//  * we can be sure that everything works still as expected after we maybe adapt our code in the next weeks.
//  */
//
// TEST_F(SimulatorTest, TwoCuboidsWithTwoParticles) {
//     LeonardJonesForce lJF{5,1};
//     std::string name = "test";
//     std::string filepath = "../tests/testData/CuboidTest3.txt";
//     DirectSumSimulationParameters dS = {0.0001, 0.001, 5,1,TypeOfForce::gravity};
//     Simulator simulator{dS,filepath,FileHandler::inputFormat::txt, FileHandler::outputFormat::vtk,10,name};
//     //Reset initial velocities to the values we used as reference,
//     //because they are generated randomly (Brownian Motion) when creating the simulator, so otherwise
//     //the test would not make any sense.
//     auto set = dynamic_cast<DefaultParticleContainer&>(simulator.getParticles());
//     set.at(0).setV({-0.171411, 0.0178057, 0.00571789});
//     set.at(1).setV({0.0756284, -0.0582274, -0.160245});
//     set.at(2).setV({0.101672, -0.05844, -0.0104494});
//     set.at(3).setV({-0.0279367, -0.00286395, -0.0690031});
//
//     //Set timeMeasurement to true to deactivate all output
//     simulator.run(true);
//     auto result = dynamic_cast<DefaultParticleContainer&>(simulator.getParticles());
//     //Here we make use of ASSERT. If this tests fails for some reason, the next steps will lead to problems, because .at()
//     //will fail with an out of range exepction
//     ASSERT_EQ(result.size(), 4);
//     std::array<double, 3> p1_pos = {-0.000230701, 1.78071e-05, 5.72107e-06};
//     std::array<double, 3> p1_vel = {-0.291041, 0.0178102, 0.0057278};
//     std::array<double, 3> p1_f = {-119.169, 0.0090578, 0.0197711};
//     std::array<double, 3> p1_fOld = {-119.276, 0.00815971, 0.0178108};
//
//     compareResults(result.at(0), p1_pos, p1_vel, p1_f, p1_fOld);
//
//     std::array<double, 3> p2_pos = {1.00013, -5.82288e-05, -0.000160248};
//     std::array<double, 3> p2_vel = {0.195259, -0.0582319, -0.160255};
//     std::array<double, 3> p2_f = {119.169, -0.00905781, -0.0197711};
//     std::array<double, 3> p2_fOld = {119.276, -0.00815971, -0.0178108};
//
//     compareResults(result.at(1), p2_pos, p2_vel, p2_f, p2_fOld);
//
//     std::array<double, 3> p3_pos = {10.0001, -5.84405e-05, -1.04488e-05};
//     std::array<double, 3> p3_vel = {0.0416199, -0.0584417, -0.0104476};
//     std::array<double, 3> p3_f = {-120.16, -0.00667862, 0.00703644};
//     std::array<double, 3> p3_fOld = {-120.157, -0.00601055, 0.00633258};
//
//     compareResults(result.at(2), p3_pos, p3_vel, p3_f, p3_fOld);
//
//     std::array<double, 3> p4_pos = {11, -2.86341e-06, -6.90036e-05};
//     std::array<double, 3> p4_vel = {0.0321149, -0.00286228, -0.0690048};
//     std::array<double, 3> p4_f = {120.16, 0.00667862, -0.00703644};
//     std::array<double, 3> p4_fOld = {120.157, 0.00601055, -0.00633258};
//
//     compareResults(result.at(3), p4_pos, p4_vel, p4_f, p4_fOld);
// }
//
// /**
//  * Test some edge cases...
//  */
//
// /**
//  * The specified file is empty. In this case the program should terminate unsuccessfully.
//  */
//
// TEST_F(SimulatorTest, EmptyFile) {
//     std::string filename = "name";
//     DirectSumSimulationParameters dS = {0.0001, 0.001, 5,1,TypeOfForce::gravity};
//
//     std::string filepath = "../tests/testData/Empty.txt";
//     EXPECT_DEATH(Simulator simulator(dS,filepath,FileHandler::inputFormat::txt, FileHandler::outputFormat::vtk,10,filename), "");
// }
//
// /**
//  * The specified file does not exist. In this case the program should terminate unsuccessfully.
//  */
//
// TEST_F(SimulatorTest, FileDoesNotExist) {
//     std::string filename = "name";
//     DirectSumSimulationParameters dS = {0.0001, 0.001, 5,1,TypeOfForce::gravity};
//     std::string filepath = "IDoNotExist";
//     EXPECT_DEATH(Simulator simulator(dS,filepath,FileHandler::inputFormat::txt, FileHandler::outputFormat::vtk,10,filename), "");
// }
