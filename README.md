# Molecular Dynamics - Group A

This repository is for the practical course **PSE: Molecular Dynamics** for group A in the SS 2024 semester.

## All Contributors

**Group Members**

- Ashutosh Solanki
- Daniel Schade
- Robin Cleve

**Course Supervisors**

- Manish Mishra
- Markus Mühlhäußer
- Fabio Gratl

## About The Project

LOREM IPSUM

**Built Using:**

- Clang 16.0.6
- CMake 3.27.4
- make 4.3

**Dependencies**

ou will need the following to compile the program:

- Doxygen 1.10.0

```bash
sudo apt install doxygen
```

- Graphviz 2.42.2

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
   ```bash
   ./MolSim ../input/eingabe-sonne.txt
   ```
     