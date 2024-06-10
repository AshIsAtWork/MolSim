#!/bin/bash
# This script loads the necessary modules for the cluster

# Load the cmake module
module load cmake/3.21.4

# Load the gcc module
module load gcc/11.2.0

# Load the boost module
module load boost/1.75.0-gcc11

# Load the xerces-c module
module load xerces-c/3.2.1

# Lists all the modules
module list