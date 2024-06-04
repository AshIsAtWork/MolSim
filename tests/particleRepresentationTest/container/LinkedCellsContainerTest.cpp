//
// Created by daniel on 21.05.24.
//

#include <algorithm>
#include <gmock/internal/gmock-internal-utils.h>
#include <include/gtest/gtest.h>
#include <spdlog/spdlog.h>

#include "../../../src/fileHandling/FileHandler.h"
#include "../../../src/particleRepresentation/container/LinkedCellsContainer.h"



/**
 * Does threeDtoOneD calculate the correct index?
 * Results for reference are calculated by hand.
 */

TEST(LinkedCellsContainerTest, ThreeDtoOneD_BasicTest) {
    LinkedCellsContainer lcc{{3, 3, 3}, 1};
    EXPECT_EQ(lcc.threeDToOneD(0,0,0), 0);
    EXPECT_EQ(lcc.threeDToOneD(4,4,4), 124);
    EXPECT_EQ(lcc.threeDToOneD(1,2,3), 86);
}

/**
 * Does threeDtoOneD calculate the correct index if we have only 2D data?
 * Results for reference are calculated by hand.
 */

TEST(LinkedCellsContainerTest, ThreeDtoOneD_TwoDTest) {
    LinkedCellsContainer lcc{{3, 3, 0}, 1};
    EXPECT_EQ(lcc.threeDToOneD(0,0,0), 0);
    EXPECT_EQ(lcc.threeDToOneD(4,4,0), 24);
    EXPECT_EQ(lcc.threeDToOneD(2,2,0), 12);
}

/**
 * Does oneDtoThreeD calculate the correct coordinates?
 * Results for reference are calculated by hand.
 */

TEST(LinkedCellsContainerTest, OneDToThreeD_BasicTest) {
    LinkedCellsContainer lcc{{1, 2, 3}, 1};
    std::array<int, 3> res = {0, 0, 0};
    EXPECT_TRUE(lcc.oneDToThreeD(0) == res);
    res = {1, 1, 0};
    EXPECT_TRUE(lcc.oneDToThreeD(4) == res);
    std::cout << lcc.oneDToThreeD(44);
    res = {2, 2, 3};
    EXPECT_TRUE(lcc.oneDToThreeD(44) == res);
    res = {2, 3, 4};
    EXPECT_TRUE(lcc.oneDToThreeD(59) == res);
}

/**
 * Does oneDtoThreeD calculate the correct coordinates?
 * Results for reference are calculated by hand.
 */

TEST(LinkedCellsContainerTest, OneDToThreeD_TwoDTest) {
    LinkedCellsContainer lcc{{1, 2, 0}, 1.5};
    std::array<int, 3> res = {0, 0, 0};
    EXPECT_TRUE(lcc.oneDToThreeD(0) == res);
    res = {2, 2, 0};
    EXPECT_TRUE(lcc.oneDToThreeD(8) == res);
    res = {2, 1, 0};
    EXPECT_TRUE(lcc.oneDToThreeD(5) == res);
}

/**
 * Are particles assigned to their correct cells by their position?
 * Results for reference are calculated by hand.
 */

TEST(LinkedCellsContainerTest, CalcCellIndex_BasicTest) {
    LinkedCellsContainer lcc{{3, 3, 3}, 1};
    //Particles
    std::array<double, 3> p1 = {0, 0, 0};
    std::array<double, 3> p2 = {1.75, 0.66, 1.234};
    std::array<double, 3> p3 = {2.2, 1.2, 0};
    std::array<double, 3> p4 = {1.7, 3.1, 0};
    std::array<double, 3> p5 = {3, 3, 3};
    std::array<double, 3> p6 = {-10.3, 0, -1};

    EXPECT_EQ(lcc.calcCellIndex(p1), 31);
    EXPECT_EQ(lcc.calcCellIndex(p2), 57);
    EXPECT_EQ(lcc.calcCellIndex(p3), 38);
    EXPECT_EQ(lcc.calcCellIndex(p4), 47);
    EXPECT_EQ(lcc.calcCellIndex(p5), 124);
    EXPECT_EQ(lcc.calcCellIndex(p6), 5);
}

/**
 * Are particles assigned to their correct cells by their position in 2D?
 * Results for reference are calculated by hand.
 */

TEST(LinkedCellsContainerTest, CalcCellIndex_Test2D) {
    LinkedCellsContainer lcc{{3, 3, 0}, 0.9};
    //Particles
    std::array<double, 3> p1 = {0, 0, 0};
    std::array<double, 3> p2 = {1.75, 1.66, 0};
    std::array<double, 3> p3 = {3.1, 3.1, 0};

    //Are particles assigned to their correct cells by their position?

    EXPECT_EQ(lcc.calcCellIndex(p1), 6);
    EXPECT_EQ(lcc.calcCellIndex(p2), 12);
    EXPECT_EQ(lcc.calcCellIndex(p3), 24);
}

/**
 * Are the correct halo cell indizes calculated?
 */

TEST(LinkedCellsContainerTest,calculateHaloCellIndizes_BasicFunctionality) {
    LinkedCellsContainer lcc{{3,2,1},1};
    auto haloCells = lcc.getHaloCells();

    std::set<int> front = {0,1,2,3,4,20,21,22,23,24,40,41,42,43,44};
    std::set<int> right = {4,9,14,19,24,29,34,39,44,49,54,59};
    std::set<int> back = {15,16,17,18,19,35,36,37,38,39,55,56,57,58,59};
    std::set<int> left = {0,5,10,15,20,25,30,35,40,45,50,55};
    std::set<int> top = {40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59};
    std::set<int> bottom = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};

    std::vector<std::set<int>> expectedHaloCells = {front, right, back, left, top, bottom};

    for(int i = 0; i < 6; i++) {
        ASSERT_EQ(haloCells[i].size(), expectedHaloCells[i].size());
        for(auto cell : haloCells[i]) {
            EXPECT_NE(expectedHaloCells[i].find(cell), expectedHaloCells[i].end());
        }
    }
}

/**
 * Are the correct halo cell indizes calculated in a 2D simulation environment?
 */

TEST(LinkedCellsContainerTest,calculateHaloCellIndizes_Test2D) {
    LinkedCellsContainer lcc{{1,2,0},1};
    auto haloCells = lcc.getHaloCells();

    std::set<int> front = {0,1,2};
    std::set<int> right = {2,5,8,11};
    std::set<int> back = {9,10,11};
    std::set<int> left = {0,3,6,9};
    std::set<int> top = {};
    std::set<int> bottom = {};

    std::vector<std::set<int>> expectedHaloCells = {front, right, back, left, top, bottom};

    for(int i = 0; i < 6; i++) {
        ASSERT_EQ(haloCells[i].size(), expectedHaloCells[i].size());
        for(auto cell : haloCells[i]) {
            EXPECT_NE(expectedHaloCells[i].find(cell), expectedHaloCells[i].end());
        }
    }
}

/**
 * Are the correct boundry cell indizes calculated in a 2D simulation environment?
 */

TEST(LinkedCellsContainerTest,calculateBoundryCellIndizes_BasicFunctionality) {
    LinkedCellsContainer lcc{{2,1,3},1};
    auto boundryCells = lcc.getBoundries();

    std::set<int> front = {17,18,29,30,41,42};
    std::set<int> right = {18,30,42};
    std::set<int> back = {17,18,29,30,41,42};
    std::set<int> left = {17,29,41};
    std::set<int> top = {41,42};
    std::set<int> bottom = {17,18};

    std::vector<std::set<int>> expectedBoundryCells = {front, right, back, left, top, bottom};

    for(int i = 0; i < 6; i++) {
        ASSERT_EQ(boundryCells[i].size(), expectedBoundryCells[i].size());
        for(auto cell : boundryCells[i]) {
            EXPECT_NE(expectedBoundryCells[i].find(cell), expectedBoundryCells[i].end());
        }
    }
}

/**
 * Are the correct boundry cell indizes calculated?
 */

TEST(LinkedCellsContainerTest, calculateBoundryCellIndizes_Test2D) {
    LinkedCellsContainer lcc{{2,2,0},1};
    auto boundryCells = lcc.getBoundries();

    std::set<int> front = {5,6};
    std::set<int> right = {6,10};
    std::set<int> back = {9,10};
    std::set<int> left = {5,9};
    std::set<int> top = {};
    std::set<int> bottom = {};

    std::vector<std::set<int>> expectedBoundryCells = {front, right, back, left, top, bottom};

    for(int i = 0; i < 6; i++) {
        ASSERT_EQ(boundryCells[i].size(), expectedBoundryCells[i].size());
        for(auto cell : boundryCells[i]) {
            EXPECT_NE(expectedBoundryCells[i].find(cell), expectedBoundryCells[i].end());
        }
    }
}

/**
 * Is the domain cell iteration scheme correctly calculated?
 */

TEST(LinkedCellsContainerTest, calculateDomainCellsIterationScheme_BasicFunctionality) {
    LinkedCellsContainer lcc{{3,3,3},1};
    auto domainCellsIterationScheme = lcc.getDomainCellIterationScheme();

    std::set<int> cell0 = {31};
    std::set<int> cell1 = {32,31};
    std::set<int> cell2 = {33,32};
    std::set<int> cell3 = {36,31,32};
    std::set<int> cell4 = {37,36,31,32,33};
    std::set<int> cell5 = {38,37,32,33};
    std::set<int> cell6 = {41,36,37};
    std::set<int> cell7 = {42,41,36,37,38};
    std::set<int> cell8 = {43,37,38,42};
    std::set<int> cell9 = {56,31,32,36,37};
    std::set<int> cell10 = {57,56,31,32,33,36,37,38};
    std::set<int> cell11 = {58,57,32,33,37,38};
    std::set<int> cell12 = {61,56,57,31,32,36,37,41,42};
    std::set<int> cell13 = {62,61,56,57,58,31,32,33,36,37,38,41,42,43};
    std::set<int> cell14 = {63,62,57,58,32,33,37,38,42,43};
    std::set<int> cell15 = {66,61,62,36,37,41,42};
    std::set<int> cell16 = {67,66,61,62,63,36,37,38,41,42,43};
    std::set<int> cell17 = {68,67,62,63,37,38,42,43};
    std::set<int> cell18 = {81,56,57,61,62};
    std::set<int> cell19 = {82,81,56,57,58,61,62,63};
    std::set<int> cell20 = {83,82,57,58,62,63};
    std::set<int> cell21 = {86,81,82,56,57,61,62,66,67};
    std::set<int> cell22 = {87,86,81,82,83,56,57,58,61,62,63,66,67,68};
    std::set<int> cell23 = {88,87,82,83,57,58,62,63,67,68};
    std::set<int> cell24 = {91,86,87,61,62,66,67};
    std::set<int> cell25 = {92,91,86,87,88,61,62,63,66,67,68};
    std::set<int> cell26 = {93,92,87,88,62,63,67,68};

    std::vector<std::set<int>> expectedDomainCellIterationScheme =
        {cell0,cell1,cell2,cell3,cell4,
         cell5, cell6, cell7, cell8, cell9, cell10, cell11, cell12, cell13, cell14, cell15, cell16, cell17, cell18,
         cell19, cell20, cell21, cell22,cell23, cell24, cell25, cell26
        };

    ASSERT_EQ(domainCellsIterationScheme.size(), expectedDomainCellIterationScheme.size());

    for(int i = 0; i < 27; i++) {
        ASSERT_EQ(domainCellsIterationScheme[i].size(), expectedDomainCellIterationScheme[i].size());
        for(auto cell : domainCellsIterationScheme[i]) {
            EXPECT_NE(expectedDomainCellIterationScheme[i].find(cell), expectedDomainCellIterationScheme[i].end());
        }
    }
}

/*
 * Does the function applyToEachParticle apply the given function exactly one time to each particle in the linked cell container?
 */

TEST(LinkedCellsContainerTest,Iterators_ApplyToEachParticle) {
    LinkedCellsContainer lcc = {{3,3,3},1};
    auto& cells = lcc.getCells();
    for(auto& cell: cells) {
        cell.emplace_back();
    }
    std::array<double, 3> toAdd = {1,1,1};
    lcc.applyToEachParticle([toAdd](Particle& p) {
       p.setX(p.getX() + toAdd);
    });

    for(auto& cell : cells) {
        for(auto& particle : cell) {
            EXPECT_TRUE(particle.getX() == toAdd);
        }
    }
}

/*
 * Does the function applyToEachParticleInDomain apply the given function exactly one time to each particle in the domain of the linked cell container?
 */

TEST(LinkedCellsContainerTest,Iterators_ApplyToEachParticleInDomain) {
    LinkedCellsContainer lcc = {{3,3,3},1};
    auto& cells = lcc.getCells();
    for(auto& cell: cells) {
        cell.emplace_back();
    }
    std::array<double, 3> toAdd = {1,1,1};
    lcc.applyToEachParticleInDomain([toAdd](Particle& p) {
       p.setX(p.getX() + toAdd);
    });

    auto& domainIterationScheme = lcc.getDomainCellIterationScheme();

    for(auto& cell : domainIterationScheme) {
        for(auto& particle : cells[cell[0]]) {
            EXPECT_TRUE(particle.getX() == toAdd);
        }
    }

    std::array<double, 3> zero = {0,0,0};

    auto& haloCells = lcc.getHaloCells();
    for(auto& side : haloCells) {
        for(auto& cell : side) {
            for(auto& particle : cells[cell]) {
                EXPECT_TRUE(particle.getX() == zero);
            }
        }
    }
}

/**
 * Does the function applyToAllUniquePairsInDomain apply the given function exactly one time
 * to each unique pair in the domain of the linked cell container and are all unique pairs covered?
 *
 * --> This test is really important, because the correct application of Newton's third law of motion depends on this method.
 *
 * Testing this is not that easy. We proceed as follows:
 *
 *      1.) We add 2 particels to each cell in the domain. All having the positon (0,0,0). This enforces that the distance
 *          between all particles is 0 and therefore smaller than the cut off radius of 1. So all particles in the adjacent cells of each cell
 *          will be considered
 *
 *      2.) We use the type of each particle for storing the number of pairs, each particle is part of.
 *
 *      3.) We compare the results with those computed by hand:
 *          Example: The cell in the middle of the cuboid has 26 adjacent cells being its neighbours. Therefore, each particle in this cell should
 *                   be part of 52 + 1 unique pairs, because in each cell are 2 particles and there is one additional particle in its own cell.
 *
 */

TEST(LinkedCellsContainerTest,Iterators_ApplyToAllUniquePairsInDomain) {
    LinkedCellsContainer lcc = {{3,3,3},1};
    auto& cells = lcc.getCells();
    for(auto& cell: cells) {
        cell.emplace_back();
        cell.emplace_back();
    }

    lcc.applyToAllUniquePairsInDomain([](Particle& p1, Particle &p2) {
       p1.setType(p1.getType() + 1);
       p2.setType(p2.getType() + 1);
    });

    std::vector<int> expected = {
        //Slice Z=1
        15,23,15,23,35,23,15,23,15,
        //Slice Z=2
        23,35,23,35,53,35,23,35,23,
        //Slice Z=3
        15,23,15,23,35,23,15,23,15
    };
    int i = 0;
    auto& domainIterationScheme = lcc.getDomainCellIterationScheme();
    for(auto& cellIndizes : domainIterationScheme) {
        for(auto& particle : cells[cellIndizes[0]]) {
            EXPECT_EQ(particle.getType(), expected[i]);
        }
        i++;
    }
}

/**
 * Does the function applyToAllBoundryParticles:
 *         -apply the given function exactly one time to each particle in the boundry cells of a specific side?
 *         -calculate the position of the corresponding ghost particle correctly?
 *         -if the distance of the particle to one side is greater than threshhold: is the particle ignored?
 * Values used as referenced are calculated by hand.
 */

TEST(LinkedCellsContainerTest,Iterators_ApplyToAllBoundryParticles) {
    LinkedCellsContainer lcc = {{3,3,3},1};
    //Add some particles to the container with different positions covering most parts of the domain
    std::vector<Particle> toAdd = {
        Particle{{1.5,1.5,0.4},{0,0,0},1},
        Particle{{2.5,1.5,0.5},{0,0,0},1},
        Particle{{0.1,2.1,1.5},{0,0,0},1},
        Particle{{1.5,2.7,1.5},{0,0,0},1},
        Particle{{1.5,0.5,1.5},{0,0,0},1},
        Particle{{2.9,1.2,1.5},{0,0,0},1},
        Particle{{0.1,0.1,2.5},{0,0,0},1},
        Particle{{1.6,2.5,2.6},{0,0,0},1}
    };
    for(Particle& p : toAdd) {
        lcc.add(p);
    }

    //Test front
    lcc.applyToAllBoundryParticles([](Particle& p, std::array<double, 3> ghostPosition) {
        std::array<double, 3> expectedGhostPosition = {0.1,-0.1,2.5};
        EXPECT_TRUE(ghostPosition == expectedGhostPosition);
    }, Side::front , 0.1);

    //Test right
    lcc.applyToAllBoundryParticles([](Particle& p, std::array<double, 3> ghostPosition) {
        std::array<double, 3> expectedGhostPosition = {3.1,1.2,1.5};
        EXPECT_TRUE(ghostPosition == expectedGhostPosition);
    }, Side::right , 0.2);

    //Test back
    lcc.applyToAllBoundryParticles([](Particle& p, std::array<double, 3> ghostPosition) {
        std::array<double, 3> expectedGhostPosition = {1.5,3.3,1.5};
        EXPECT_TRUE(ghostPosition == expectedGhostPosition);
    }, Side::back , 0.3);

    //Test left, here we choose the threshhold small enough that no particle fulfills the requirements.
    //Therefore the function should be called 0 times.
    int i = 0;
    lcc.applyToAllBoundryParticles([&i](Particle& p, std::array<double, 3> ghostPosition) {
        i++;
    }, Side::left , 0.05);
    EXPECT_EQ(i, 0);

    //Test top
    lcc.applyToAllBoundryParticles([](Particle& p, std::array<double, 3> ghostPosition) {
        std::array<double, 3> expectedGhostPosition = {1.6,2.5,3.4};
        EXPECT_TRUE(ghostPosition == expectedGhostPosition);
    }, Side::top , 0.4);

    //Test bottom
    lcc.applyToAllBoundryParticles([](Particle& p, std::array<double, 3> ghostPosition) {
        std::array<double, 3> expectedGhostPosition = {1.5,1.5,-0.4};
        EXPECT_TRUE(ghostPosition == expectedGhostPosition);
    }, Side::bottom , 0.4);
}

/**
 * Does the function clearHaloCells() remove all particles from all halo cells of a specific side?
 */

TEST(LinkedCellsContainerTest, ClearHaloCells) {
    LinkedCellsContainer lcc = {{1,1,1},1};

    //Add one particle to each side of halo cells
    std::vector<Particle> toAdd = {
        Particle{{0.5,0.5,-0.5},{0,0,0},1},
        Particle{{-0.5,0.5,0.5},{0,0,0},1},
        Particle{{0.5,1.5,0.5},{0,0,0},1},
        Particle{{1.5,0.5,0.5},{0,0,0},1},
        Particle{{0.5,-0.5,0.5},{0,0,0},1},
        Particle{{0.5,0.5,1.5},{0,0,0},1},
    };

    for(Particle& p : toAdd) {
        lcc.add(p);
    }

    size_t numberParticlesFrontBefore = 0;
    for(auto cellIndex : lcc.getHaloCells()[0]) {
        numberParticlesFrontBefore += lcc.getCells()[cellIndex].size();
    }

    for(int side = 0; side < 6; side++) {
        //Ensure that side contains one particle. Otherwise, testing if clearing was successful would not be meaningful
        ASSERT_EQ(numberParticlesFrontBefore, 1);
        lcc.clearHaloCells(static_cast<Side>(side));
        size_t numberParticlesFrontAfter = 0;
        for(auto cellIndex : lcc.getHaloCells()[side]) {
            numberParticlesFrontAfter += lcc.getCells()[cellIndex].size();
        }
        EXPECT_EQ(numberParticlesFrontAfter,0);
    }
}

/**
 * Are all particles assigned to their correct cell after their positions have been changed?
 */

TEST(LinkedCellContainerTest, UpdateCells) {
    LinkedCellsContainer lcc = {{3,3,3},1};

    //Add 9 particles, one particle in each cell for the slice with Z=1
    std::vector<Particle> toAdd = {
        Particle{{0.5,0.5,0.5},{0,0,0},1,1},
        Particle{{1.5,0.5,0.5},{0,0,0},1,2},
        Particle{{2.5,0.5,0.5},{0,0,0},1,3},
        Particle{{0.5,1.5,0.5},{0,0,0},1,4},
        Particle{{1.5,1.5,0.5},{0,0,0},1,5},
        Particle{{2.5,1.5,0.5},{0,0,0},1,6},
        Particle{{0.5,2.5,0.5},{0,0,0},1,7},
        Particle{{1.5,2.5,0.5},{0,0,0},1,8},
        Particle{{2.5,2.5,0.5},{0,0,0},1,9}
    };

    for(Particle& p : toAdd) {
        lcc.add(p);
    }

    //Shift all particles up by one
    std::array<double, 3> shift = {0,0,1};
    lcc.applyToEachParticle([shift](Particle& p) {
       p.setX(p.getX() + shift);
    });

    lcc.updateCells();

    //All particles should be now one cell higher than before
    int type = 1;
    for(int y = 1; y <= 3; y++) {
        for(int x = 1; x <= 3; x++) {
            EXPECT_EQ(lcc.getCells()[lcc.threeDToOneD(x, y, 2)][0].getType(), type++);

        }
    }
}

/**
 * Is the size of the linked cell container tracked properly?
 */

TEST(LinkedCellContainerTest, Size) {
    LinkedCellsContainer lcc = {{3,3,3},1};
    //Size at the begining should be 0
    ASSERT_EQ(lcc.size(), 0);


    //Add a bunch of particles...
    for(int i = 0; i < 100; i++) {
        Particle p;
        lcc.add(p);
    }
    EXPECT_EQ(lcc.size(), 100);

    //Add some more particles, that will be assigned to halo cells at the front
    for(int i = 0; i < 10; i++) {
        Particle p{{1,-1,1},{0,0,0},1};
        lcc.add(p);
    }
    EXPECT_EQ(lcc.size(), 110);

    //Clear halo cells at the front and remove the 10 particles we have just added
    lcc.clearHaloCells(Side::front);
    EXPECT_EQ(lcc.size(), 100);
}

/**
 * Does adding particles to the container work properly?
 * Are particles assigend to their correct cell?
 */

TEST(LinkedCellContainerTest, Add_BasicFunctionality){
    LinkedCellsContainer lcc = {{3,3,3},1};
    //Size at the begining should be 0
    ASSERT_EQ(lcc.size(), 0);

    //add one particle and check, if it is assigend to its correct cell
    Particle p = {{0,0,0},{0,0,0},1, 10};
    lcc.add(p);
    ASSERT_EQ(lcc.size(), 1);
    ASSERT_EQ(lcc.getCells()[lcc.threeDToOneD(1,1,1)].size(), 1);
    EXPECT_EQ(lcc.getCells()[lcc.threeDToOneD(1,1,1)][0].getType(),10);
}

/**
 * Test 2D edge case: Particles that live not in 2D space should not be addable to a 2D simulation environment
 */

TEST(LinkedCellContainerTest, Add_EdgeCase_2D){
    LinkedCellsContainer lcc = {{3,3,0},1};
    //Size at the begining should be 0
    ASSERT_EQ(lcc.size(), 0);
    //We want to test an 2D simulation environment
    ASSERT_TRUE(lcc.isTwoD());

    //add one particle living in 2D space. This should be possible
    Particle p2D = {{0,0,0},{0,0,0},1, 10};
    lcc.add(p2D);
    ASSERT_EQ(lcc.size(), 1);
    //try adding a particle living in 3D space. Should terminate the program with an error.

    Particle p3D = {{1,1,1},{1,1,1},1};
    EXPECT_DEATH(lcc.add(p3D),"");
}













