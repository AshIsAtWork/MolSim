# Group A #
Members:
- [Daniel Schade](https://github.com/D4ni3lSch4d3)
- [Ashutosh Solanki](https://github.com/AshIsAtWork)
- [Robin Cleve](https://github.com/cleveee)

# Code #
* Link to Pull Request: [Pull Request for Assignment 2](https://github.com/AshIsAtWork/MolSim/pull/4)
* Compiler: gcc 11.4.0

# Report #

---

## Task 1: Unit Tests ##

**1. Setting up the googletest framework**  
We started this task with informing ourselves about ways to integrate the googletest framework into our project via CMake without relying on system-wide installations as it is required. There are two main approaches for this:

* Install libraries and headers: Import GoogleTest by using `find_package` (or `pkg_check_modules`).  
* Build GoogleTest as part of your project directly. This is done by making the GoogleTest source code available to the main build which can be done in several ways. The most flexible way is to use the `FetchContent_Declare` function. This function automatically pulls the latest stable version of googletest from GitHub. One big advantage of this approach is that our project is always kept up to date without requiring any additional work.
We created a new test executable called MolSimTest for unit testing which comprises all source files necessary for testing, so all files in the `src` directory and the `tests` directory. We excluded the file `MolSim.cpp` containing the main function of our program, because its inclusion caused problems like multiple definition errors. Fortunately, we do not need this file for our unit tests and therefore this is no problem at all.

We put all code for integrating googletest in a [separate cmake module](../../cmake/modules/google-test.cmake) to improve the legibility and structure of our CMake file. Otherwise, our `CMakeLists.txt` file would become confusing when adding more and more dependency. 

Last but not least we used `gtest_discover_tests`, a really handy command ensuring all tests being automatically discovered and configured.

**2. Writing and Organising Unit Tests**  
First of all, we created a new folder called `tests` in our repository being the place where all test files are stored. The `tests` folder mimics the folder structure of our `src` folder so that all tests belonging to some class can be found quickly. Before writing any tests we informed ourselves about best practices of using the googletest framework. A good overview provides the following source: [Google Test Primer](http://google.github.io/googletest/primer.html). With this knowledge in mind we started to write our first tests, whereby we paid particular attention to the following points:
* We tested each class individually in its own .cpp file and its own testsuite. In following this rule, we ensured that our tests exhibit a meaningful structure which gains in importance when the project grows larger and the number of tests increases.
* We used test fixtures to set up a test environment for each test suite that avoids code duplication, because otherwise we have to set up the environment in each test case separately.
* We thought about where to use the `ASSERT` macro instead of the `EXPECT` macro. In the Google Test Primer was suggested to use `EXPECT` whenever it is possible in order to get more information about the test results. The main difference between `ASSERT` and `EXPECT` is that the test is aborted immediately, if an `ASSERT` statement is violated whereas the test continuous independently of the result of an `EXPECT` statement. Therefore, `ASSERT` should only be used, when the program is in an invalid state after the `ASSERT` statement failed. We left a comment everywhere, where we used `ASSERT` instead of `EXPECT` to explain, why it is necessary in this case.  

We wrote a comment above each test to describe the idea behind it. This may help in the future, when a test fails, and we cannot remember exactly what this test was about. 

Finally, we also created two larger tests that test multiple iterations in the Simulator class ([see here](../../tests/moleculeSimulatorTest/SimulatorTest.cpp)). We have calculated the data that we use as a reference for these tests using our own implementation which might seem counterintuitive at the first glance. We have some strong reasons to think that our implementation is currently calculating correct results, which we have checked in more detail using ParaView and other unit tests. In the future, however, we may change our implementation or make optimizations. These tests ensure that our program still delivers correct results.

**3. Running all Unit Tests**  
To run all unit tests, first build the whole project as described in our [README](../../README.md) file. Then change into the build folder and run `ctest` or `./MolSimTests` over the command line.

---

## Task 2: Continuous Integration ##
For continuous integration, we use GitHub Action workflows. A workflow is a configurable, automated process that will run one or more jobs. Workflows are defined by a YAML file checked in to your repository and will run when triggered by an event in your repository, or they can be triggered manually, or at a defined schedule.  
We took the developer liberty to extend these checks for every push on every branch and every pull request. This ensures that the code is always in a working state and that no bugs are introduced.

Workflows are defined in the `.github/workflows` directory in a repository.
A repository can have multiple workflows, each of which can perform a different set of tasks.
For our project, we created three workflows:
* The [first workflow](../../.github/workflows/cmake-multi-platform.yml) builds and tests pull requests and checks, if the code compiles. Code that does not even compile should be rejected.
    For this we set up an Ubuntu image, with GCC and Clang. We then use the GitHub checkout workflow to check out the branch we are currently working on. Furthermore, we install the dependencies we need to build our project. We then build the project using CMake and make. We have also added the `fsanitize` flag to our build to check for memory leaks. If the build fails, the workflow will fail and the pull request will not be merged by us.
* The [second workflow](../../.github/workflows/unit-tests.yml) runs our unit tests whenever new code is pushed, so that bugs in the code may be spotted immediately. We use the same setup as in the first workflow, but we also run the tests using ctest. If the tests fail, the workflow will fail and the pull request will not be merged by us.
* The [third workflow](../../.github/workflows/doxygen.yml) is for building and deploying the Doxygen documentation of this project to GitHub Pages that ensures that there is always up-to-date project documentation available on the web. If you want to visit the documentation, click [here](https://ashisatwork.github.io/MolSim/).

Additionally, to enforce this, we added branch protected setting to our repository.
This means that no code can be merged into the main branch without passing the tests.
This ensures that the main branch is always in a working state.
It also requires at least one reviewer to approve the pull request before it can be merged.
This ensures that the code is reviewed before it is merged into the main branch.

---

## Task 3: Logging ##

**1. Integrating spdlog into our Project**  
Like in task one, we first informed ourselves about the different possibilities of adding spdlog to our project. Because we cannot rely on any system libraries we had to add all required dependencies to this repository. Again we used the `FetchContent_Declare` function of CMake pulling automatically the latest stable version of spdlog from GitHub. Because the setup is done implicitly when building the project, this procedure ensures a good user experience for other people that will work with our project.  
Like already done with googletest we extracted all CMake code corresponding to the integration of spdlog into a [separate CMake module](../../cmake/modules/spdlog.cmake) for the reason as stated above. 

**2. Using spdlog for efficient Logging**  
As feedback from the last week, we have been told that our program would spam the console quite heavily with debug output which of course compromises the running time of our program. Additionally, the user could not adjust the console output to his or her needs. If you just want to execute the simulation as fast as possible, you might not be interested in any kind of debug output. In the case that you unfortunately have to debug your program, however, you want to receive as much information about the current state of your program as possible. With the logging feature that spdlog provides this can now be realised. The following things are worth mentioning:
* We transformed all console output being part of the simulation into a log statement and set the log level accordingly. To inform the user about errors and other things that should not happen in the normal execution flow, we used the log level error. One example of this can be found in the [FileReader](../../src/fileHandling/reader/FileReader.cpp) class. To provide the user with additional debug information, we used the log levels trace and debug. For all information which is useful for the user when using the program, for example, which file format was provided as input and so on, we used the log level info being the default log level.
* We informed ourselves about the difference of log functions and log macros. In general, log macros are more efficient than log functions because they are processed by the predecessor. If you want no logging info, the predecessor will ignore all logging statements and your program will be compiled without them. Log functions, however, are still part of your compiled code even if you set the log level to off which may affect the running time of your program negatively. Nevertheless, we have decided in favor of log functions. The main reason for this is that we do not want you to have to recompile your program if you want to change the log level. Furthermore, log functions are very optimized and will not lead to significantly longer execution times. Last but not least, the output format of log functions can be adapted to your individual needs, which might be useful in the future.  

**3. Setting the log level**  
You can set the log level directly over the command line using the option `-l <log level>`. All valid log levels are `trace`, `debug`, `info`, `warn`, `error`, `critical` and `off`. If the log level is not specified the log level `info` will be used. Here is a simple example, how to execute our program with the log level `debug`:

```bash
    ./MolSim -f ../input/2D-cuboid-collision.txt -l debug
```

---

## Task 4: Collision of two Bodies ##

While the previous tasks were more concerned with the set-up of our repository, in this task, we have extended the functionality of our program with some exiting new features. 

**1. Generation of Cuboids of Particles**   
The first new feature is the implementation of the new class
called [Particle Generator](../../src/moleculeSimulator/particleGeneration/ParticleGenerator.h).
With this class we are now able to generate cuboid-like particle structures.
In the first week we only were able
to read single particles from a file
which might become really cumbersome if you want to run a simulation containing hundreds of particles.
Using this new class, you only have to specify some parameters and everything else is handled automatically.
The definition of one cuboid requires seven parameters
whereas some parameters like the position consist of three numbers.
Handling the input over the command line seems to us, therefore, to be not the right approach.
Instead, we introduced a new file format, specially for cuboid data.
Additional information about this file format can be found [here](../../input/2D-cuboid-collision.txt).
Additionally, we implemented a new parser being part of the class `FileReader`.
For our implementation we took the already existing parser (the one that parses the file `eingabe-sonne.txt`),
which was already provided, as example and adapted the function to our needs.
Building on already existing functionality is often the simplest solution and therefore maybe better
than coming up with something completely new.
To be able to distinguish between the two file formats,
the first line being not a comment of each input file must specify which data it contains,
either `Particle` data (the old file format) or `Cuboid` data
(the new file format).
Both file formats can be processed by our program using the option `-f <path to file>`. 

**2. Brownian Motion**  
Another interesting feature is that each particle is assigned a randomised velocity at the start of the simulation in addition to its initial velocity being proportional to the temperature of the object of interest. This models the Brownian Motion that can be observed in reality. There are no particles that do not move at all. Each particle does some movement, however small it may be. We created a little video which shall highlight this effect. Click [here](Brownian-Motion.mp4) to watch.  

**2. Leonard Jones Force**  
As announced in last week's task sheet, we had to add this week a new type of force to our project.
With the help of our strategy pattern this was quickly done.
We just created a new class [LeonardJonesForce](../../src/moleculeSimulator/forceCalculation/leonardJones/LeonardJonesForce.h) as additional specialisation of our [Force](../../src/moleculeSimulator/forceCalculation/Force.h) interface.
Therefore, we could easily substitute the gravitational force with the new Leonard-Jones force.
To increase the efficiency of our calculation,
we simplified the formula
which was given on the worksheet in such a way that no high exponents like 12 occur whose computation might be costly. 

**3. Application of Newton's third Law of Motion**  
As you recommended in last week's feedback and being part of this assignment sheet, we use now Newton's third law of motion to speed up the force calculation. By applying this rule it is only necessary to iterate over all unique pairs of particles and compute the force between them instead of iterating over all conceivable possibilities. According to the binomial coefficient, there are `n over 2`, that is `n*(n-1)/2` unique pairs when we consider `n` particles which is half as much as `n² - n` possible pairs that we processed in our old version. With this optimization we may not improve the asymptotically running time of our force calculation that is still in `O²`, but nevertheless may speed up our program up to a constant factor. In the next section we did some benchmarks to compare the running times of the old naive implementation with the new one.

**4. Benchmarks**  
As required we measured the execution time of our program calculating the collision of two 2D bodies as stated in the worksheet.
We ran the program on the following hardware:
* **Operating System**: `Ubuntu 22.04.4 LTS`
* **Processor**: `Intel(R) Core(TM) i5-6500 CPU @ 3.20GHz`
* **Memory**: `16 GiB`

|                                        |              |
|----------------------------------------|-------------:|
| Using Newton's third law of motion     | 2 min 27 sec |
| Not Using Newton's third law of motion | 4 min 26 sec |
| Speed up                               |         1.81 |

As our benchmark shows, the new version is significantly faster than the old one. 

If you want to benchmark our program on your own machine using the same data and parameters as we did, change into your build folder
and run the following command:

```bash
./MolSim -f ../input/2D-cuboid-collision.txt -e 5 -d 0.0002 -t
```
*Note: The program will use the optimised version of the force calculation that is based on Newton's third law of motion.*

We also created a video showing the collision of the two bodies in ParaView. This can be found [here](Animation-Collision-of-two-Bodies.mp4).

---

## Misc ##

* As you proposed in the feedback of our last submission,
now our program calculates the force between all particles before entering the simulation loop.
This ensures that our first position and velocity updates do not use an old force of `0` in the first iteration.

* Because Robin and Daniel had problems building the project on their macs, we thought about a simple solution to fix this. We decided to use Docker, a free available software using OS-level virtualization to run software in lightweight containers. A container is an isolated environment in which you can build and run your software without doing any environment configuration on your native operating system. We created a suitable [Dockerfile](../../Dockerfile) that specifies all dependencies being necessary to use our program. [Here](../../Docker/readme.md) you can find step-by-step instructions on how to install Docker, create an image and run your program in a Docker container having macOS as a native operating system. 


