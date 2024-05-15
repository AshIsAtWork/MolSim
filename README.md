# Molecular Dynamics - Group A

This repository is for the practical course **PSE: Molecular Dynamics** for group A in the SS 2024 semester.

# Repository Status

CMake Multi-Platform: (Currently only tested on Ubuntu)

[![CMAKE](https://github.com/AshIsAtWork/MolSim/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/AshIsAtWork/MolSim/actions/workflows/cmake-multi-platform.yml)

Tests:

[![CTest](https://github.com/AshIsAtWork/MolSim/actions/workflows/unit-tests.yml/badge.svg)](https://github.com/AshIsAtWork/MolSim/actions/workflows/unit-tests.yml)

Doxygen Documentation:

[![Doxygen](https://github.com/AshIsAtWork/MolSim/actions/workflows/doxygen.yml/badge.svg)](https://github.com/AshIsAtWork/MolSim/actions/workflows/doxygen.yml)

The hosted Documentation can be found [here](https://ashisatwork.github.io/MolSim/)


## All Contributors

**Group Members**

- [Ashutosh Solanki](https://github.com/AshIsAtWork)
- [Daniel Schade](https://github.com/D4ni3lSch4d3)
- [Robin Cleve](https://github.com/cleveee)

**Course Supervisors**

- [Manish Mishra](https://github.com/manishmishra6016)
- [Markus Mühlhäußer](https://github.com/thesamriel)
- [Fabio Gratl](https://github.com/FG-TUM)

## About The Project

This is the practical project repository for the course offered by the Scientific Computing Chair at TUM.

**Built Using:**

- Clang 16.0.6
- CMake 3.27.4
- make 4.3
- Boost Program Options 1.74.0.3

**Dependencies**

ou will need the following to compile the program:

- Doxygen 1.10.0 (Only needed for generating documentation)

```bash
sudo apt install doxygen
```

- Graphviz 2.42.2 (Only needed for generating documentation)

```bash
sudo apt install graphviz
```

- Libxerces 3.2.4

```bash
sudo apt install libxerces-c-dev
```

- Boost Program Options 1.74.0.3 

```bash
sudo apt-get install libboost-program-options-dev
```

## Build

The process of building the project is described here:

1. Create and change your working directory to the build folder inside the project.

```bash
mkdir build && cd build
```

2. Configure the Program using Cmake.
    - standard:
         ```bash
         cmake ..
         ```
    - With Doxygen Documentation:
         ```bash
         cmake .. -D BUILD_DOCS=ON
         ```
3. Building the Program
    - Compile Project:
   ```bash
   make 
   ```
    - Compile Docs using Doxygen
   ```bash
   make doc_doxygen
   ```
Note: Output present inside **build/docs/html/index.html**
4. Running the Program
   - Inside your **build/** folder run the following command:
   ```bash
   ./MolSim <FILENAME>
   ```
   Note: FILENAME includes the path and should look something like the following:
   - Task 1:
   ```bash
   ./MolSim -f ../input/eingabe-sonne.txt
   ```
   - Task 2:
   ```bash
   ./MolSim -f ../input/2D-cuboid-collision.txt -e 5 -d 0.0002
   ```
5. Running all Tests
   - First: Change into your **build/** folder:
     - Using ctest:
     ```bash
     ctest
     ```
     - Using executable:
     ```bash
     ./MolSimTests
     ```
   