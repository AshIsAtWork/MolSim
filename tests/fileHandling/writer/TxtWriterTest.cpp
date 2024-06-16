//
// Created by cle on 6/16/24.
//

#include <gtest/gtest.h>
#include <fstream>
#include "fileHandling/outputWriter/TXTWriter/TxtWriter.h"
#include "particleRepresentation/container/defaultParticleContainer/DefaultParticleContainer.h"

class TxtWriterTest : public testing::Test {
protected:
    DefaultParticleContainer dpc;

    void SetUp() override {
        //Deactivate all console output
        spdlog::set_level(spdlog::level::off);
    }

    // Helper function to read a file into a string after writing
    static std::string readFileToString(const std::string& filename) {
        std::ifstream file(filename);
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
};

TEST_F(TxtWriterTest, Basic_TxtWriter_test) {

    std::array<double, 3> x = {1.1, 2.2, 3.3};
    std::array<double, 3> v = {4.4, 5.5, 6.6};
    std::array<double, 3> f = {7.7, 8.8, 9.9};
    std::array<double, 3> old_f = {10.1, 12.2, 13.3};

    for (int i = 0; i < 11; i++) {
        Particle pNew = Particle{x, v ,1,2};
        pNew.setF(f);
        pNew.setOldF(old_f);
        dpc.add(pNew);
    }

    ASSERT_EQ(TxtWriter::writeToFile(dpc), 0);

    std::string filename = "checkpoint.txt";
    std::string fileContent = readFileToString(filename);

    // Expected file content
    std::string expectedContent =
            "# xyz-coord, velocity, Force, Old force, mass, type, epsilon, sigma\n"
            "Particle\n"
            "11\n"
            "1.1 2.2 3.3    4.4 5.5 6.6    7.7 8.8 9.9    10.1 12.2 13.3    1   2   5   1\n"
            "1.1 2.2 3.3    4.4 5.5 6.6    7.7 8.8 9.9    10.1 12.2 13.3    1   2   5   1\n"
            "1.1 2.2 3.3    4.4 5.5 6.6    7.7 8.8 9.9    10.1 12.2 13.3    1   2   5   1\n"
            "1.1 2.2 3.3    4.4 5.5 6.6    7.7 8.8 9.9    10.1 12.2 13.3    1   2   5   1\n"
            "1.1 2.2 3.3    4.4 5.5 6.6    7.7 8.8 9.9    10.1 12.2 13.3    1   2   5   1\n"
            "1.1 2.2 3.3    4.4 5.5 6.6    7.7 8.8 9.9    10.1 12.2 13.3    1   2   5   1\n"
            "1.1 2.2 3.3    4.4 5.5 6.6    7.7 8.8 9.9    10.1 12.2 13.3    1   2   5   1\n"
            "1.1 2.2 3.3    4.4 5.5 6.6    7.7 8.8 9.9    10.1 12.2 13.3    1   2   5   1\n"
            "1.1 2.2 3.3    4.4 5.5 6.6    7.7 8.8 9.9    10.1 12.2 13.3    1   2   5   1\n"
            "1.1 2.2 3.3    4.4 5.5 6.6    7.7 8.8 9.9    10.1 12.2 13.3    1   2   5   1\n"
            "1.1 2.2 3.3    4.4 5.5 6.6    7.7 8.8 9.9    10.1 12.2 13.3    1   2   5   1\n";

    // Compare the file content with the expected content
    EXPECT_EQ(expectedContent, fileContent);

    // Clean up the test file
    //std::remove(filename.c_str());
}
