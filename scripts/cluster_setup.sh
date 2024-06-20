#!/bin/bash
# This script loads the necessary modules for the cluster
# Params list:
# ${1} - NAME The name of the job
# ${2} - CLUSTER Possible values: serial or inter
# ${3} - PARTITION Possible values: serial_std, serial_long, teramem_inter or cm2_tiny
# ${4} - MAIL_TYPE Possible values: BEGIN, END, FAIL, REQUEUE, ALL or NONE
# ${5} - MAIL_USER The email address to send the notifications IMPORTANT: mail cannot be gmail, so use your university email
# ${6} - CPUS_PER_TASK The number of CPUs per task
# ${7} - TIME The time limit for the job in format "HH:MM:SS"
# ${8} - INPUT_FILE_PATH The path to the input file
# ${9} - INPUT_FILE_FORMAT The format of the input file
# ${10} - OUTPUT_FILE_FORMAT The format of the output file
# ${11} - OPTIONAL: -t for BENCHMARKING
# ${12} - OPTIONAL: -p for PROFILING
# ${13} - OPTIONAL: -O for optimisation

# Define color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m'
FLAG_T=""
FLAG_P=""
OPTIMISATION=""

# Function to display help message
function display_help() {
    echo -e "${YELLOW}Usage: $0 CLUSTER PARTITION MAIL_TYPE MAIL_USER CPUS_PER_TASK TIME INPUT_FILE_PATH INPUT_FILE_FORMAT OUTPUT_FILE_FORMAT BENCHMARKING PROFILING${NC}"
    echo
    echo "Parameters:"
    echo -e "${YELLOW}NAME${NC}              The name of the job"
    echo -e "${YELLOW}CLUSTER${NC}            Possible values: serial or inter"
    echo -e "${YELLOW}PARTITION${NC}          Possible values: serial_std, serial_long, teramem_inter or cm2_tiny"
    echo -e "${YELLOW}MAIL_TYPE${NC}          Possible values: BEGIN, END, FAIL, REQUEUE, ALL or NONE"
    echo -e "${YELLOW}MAIL_USER${NC}          The email address to send the notifications. IMPORTANT: mail cannot be gmail, so use your university email"
    echo -e "${YELLOW}CPUS_PER_TASK${NC}      The number of CPUs per task"
    echo -e "${YELLOW}TIME${NC}               The time limit for the job in format \"HH:MM:SS\""
    echo -e "${YELLOW}INPUT_FILE_PATH${NC}    The path to the input file"
    echo -e "${YELLOW}INPUT_FILE_FORMAT${NC}  The format of the input file"
    echo -e "${YELLOW}OUTPUT_FILE_FORMAT${NC} The format of the output file"
    echo -e "${YELLOW}BENCHMARKING${NC}       Optional flag -t for benchmarking"
    echo -e "${YELLOW}PROFILING${NC}          Optional flag -p for profiling"
    echo -e "${YELLOW}OPTIMISATION${NC}       Optional flag -O for optimisation"
    echo
    echo "Example:"
    echo -e "${YELLOW}  $0 MolSim_Group_A serial serial_std ALL your_university_email@example.com 4 02:00:00 ../input/assignment-3/2d-cuboid-collision.xml xml vtk -t -p -O${NC}"
    echo
}

# Check if the user requested help
if [[ "${1}" == "-h" || "${1}" == "--help" ]]; then
    display_help
    exit 0
fi

# Check if the number of parameters is correct
if [ "$#" -lt 10 ]; then
    echo -e "${RED}Error: You need to provide at least 10 parameters.${NC}"
    echo -e "${RED}Run the script with -h or --help to see the usage.${NC}"
    exit 1
fi

# Check if the second parameter is either "serial" or "inter"
if [ "${2}" != "serial" ] && [ "${2}" != "inter" ]; then
    echo -e "${RED}Error: The second parameter must be either 'serial' or 'inter'.${NC}"
    exit 1
fi

# Check if the third parameter is either "serial_std", "serial_long" or "teramem_inter"
if [ "${3}" != "serial_std" ] && [ "${3}" != "serial_long" ] && [ "${3}" != "teramem_inter" ] && [ "${3}" != "cm2_tiny" ]; then
    echo -e "${RED}Error: The third parameter must be either 'serial_std', 'serial_long' or 'teramem_inter'.${NC}"
    exit 1
fi

# Check if the fourth parameter is either "BEGIN", "END", "FAIL", "REQUEUE", "ALL" or "NONE"
if [ "${4}" != "BEGIN" ] && [ "${4}" != "END" ] && [ "${4}" != "FAIL" ] && [ "${4}" != "REQUEUE" ] && [ "${4}" != "ALL" ] && [ "${4}" != "NONE" ]; then
    echo -e "${RED}Error: The fourth parameter must be either 'BEGIN', 'END', 'FAIL', 'REQUEUE', 'ALL' or 'NONE'.${NC}"
    exit 1
fi

# Check if the sixth parameter is a number
if ! [[ "${6}" =~ ^[0-9]+$ ]]; then
    echo -e "${RED}Error: The sixth parameter must be a number.${NC}"
    exit 1
fi

# Check if the seventh parameter is in format "HH:MM:SS"
if ! [[ "${7}" =~ ^[0-9][0-9]:[0-9][0-9]:[0-9][0-9]$ ]]; then
    echo -e "${RED}Error: The seventh parameter must be in format 'HH:MM:SS'.${NC}"
    exit 1
fi

# Check if the ninth parameter is a valid file format
if [ "${9}" != "xml" ] && [ "${8}" != "txt" ]; then
    echo -e "${RED}Error: The ninth parameter must be either 'xml' or 'txt'.${NC}"
    exit 1
fi

# Check if the tenth parameter is a valid file format
if [ "${10}" != "xyz" ] && [ "${10}" != "vtk" ]; then
    echo -e "${RED}Error: The tenth parameter must be either 'xyz' or 'vtk'.${NC}"
    exit 1
fi

# Check if the user provided the optional flags for benchmarking
if [ "${11}" == "-t" ] || [ "${12}" == "-t" ] || [ "${13}" == "-t" ]; then
    echo -e "${YELLOW}BENCHMARKING flag is set${NC}"
    FLAG_T="-t"
else
    echo -e "${YELLOW}BENCHMARKING flag is not set${NC}"
    FLAG_T=""
fi

# Check if the user provided the optional flags for profiling
if [ "${11}" == "-p" ] || [ "${12}" == "-p" ] || [ "${13}" == "-p" ]; then
    echo -e "${YELLOW}PROFILING flag is set${NC}"
    FLAG_P="-DPROFILING=ON"
else
    echo -e "${YELLOW}PROFILING flag is not set${NC}"
    FLAG_P=""
fi

# Check if the user provided the optional flags for optimisation
if [ "${11}" == "-O" ] || [ "${12}" == "-O" ] || [ "${13}" == "-O" ]; then
    echo -e "${YELLOW}OPTIMISATION flag is set${NC}"
    OPTIMISATION="-DOPTIMISATION=ON"
else
    echo -e "${YELLOW}OPTIMISATION flag is not set${NC}"
    OPTIMISATION=""
fi

# Load the necessary modules
module load slurm_setup
module load cmake/3.21.4
module load gcc/11.2.0
module load boost/1.75.0-gcc11
module load xerces-c/3.2.1

# List all the loaded modules
module list

# Set up the build directory
cd .. && rm -rf build/ && mkdir build/ && cd build && cmake -DCMAKE_BUILD_TYPE=Release ${FLAG_P} ${OPTIMISATION} .. && cmake --build .

# Print the parameters
echo -e
echo -e "${GREEN}Making Cluster_start.cmd with the following parameters:${NC}"
echo -e "${GREEN}NAME: ${YELLOW}${1}${NC}"
echo -e "${GREEN}CLUSTER: ${YELLOW}${2}${NC}"
echo -e "${GREEN}PARTITION: ${YELLOW}${3}${NC}"
echo -e "${GREEN}MAIL_TYPE: ${YELLOW}${4}${NC}"
echo -e "${GREEN}MAIL_USER: ${YELLOW}${5}${NC}"
echo -e "${GREEN}CPUS_PER_TASK: ${YELLOW}${6}${NC}"
echo -e "${GREEN}TIME: ${YELLOW}${7}${NC}"
echo -e "${GREEN}INPUT_FILE_PATH: ${YELLOW}${8}${NC}"
echo -e "${GREEN}INPUT_FILE_FORMAT: ${YELLOW}${9}${NC}"
echo -e "${GREEN}OUTPUT_FILE_FORMAT: ${YELLOW}${10}${NC}"
echo -e "${GREEN}BENCHMARKING: ${YELLOW}${FLAG_T}${NC}"
echo -e "${GREEN}PROFILING: ${YELLOW}${FLAG_P}${NC}"
echo -e "${GREEN}OPTIMISATION: ${YELLOW}${OPTIMISATION}${NC}"

# Remove any existing cluster_start.cmd file
cd .. && rm -f cluster_start.cmd

# Write the content to the cluster_start.cmd file
cat <<EOL > cluster_start.cmd
#!/bin/sh
#SBATCH -J ${1}
#SBATCH -o ./%x.%j.%N.out
#SBATCH -D ../build/
#SBATCH --get-user-env
#SBATCH --clusters=${2}
#SBATCH --partition=${3}
#SBATCH --mail-type=${4}
#SBATCH --mail-user=${5}
#SBATCH --mem=1000mb
#SBATCH --cpus-per-task=${6}
#SBATCH --export=NONE
#SBATCH --time=${7}

./MolSim -f ${8} -i ${9} -o ${10} ${FLAG_T}
EOL

# Add the benchmarking flag if it was provided
if [ "${FLAG_P}" == "-p" ]; then
    echo "gprof molsim gmon.out > analysis.txt" >> cluster_start.cmd
fi

echo -e "${GREEN}Success: ${NC}cluster_start.cmd has been created with the provided parameters.${NC}"
echo -e

echo -e "${GREEN}The following commands do not need to be adapted, you can copy and paste them as they are.${NC}"
echo -e "${GREEN}----------------------------------------${NC}"
echo -e "${GREEN}To submit the job, run the following command:${NC}"
echo -e "${YELLOW}sbatch ../cluster_start.cmd${NC}"
echo -e "${GREEN}----------------------------------------${NC}"
echo -e "${GREEN}To see the status of the job, run the following command:${NC}"
echo -e "${YELLOW}squeue --cluster ${2} --me${NC}"
echo -e "${GREEN}----------------------------------------${NC}"
echo -e "${GREEN}To cancel the job, run the following command:${NC}"
echo -e "${YELLOW}scancel -M ${2} -n ${1}${NC}"
echo -e "${GREEN}----------------------------------------${NC}"
