#!/bin/bash
# This script loads the necessary modules for the cluster
# Params list:
# $1 - CLUSTER Possible values: serial or inter
# $2 - PARTITION Possible values: serial_std, serial_long or teramem_inter
# $3 - MAIL_TYPE Possible values: BEGIN, END, FAIL, REQUEUE, ALL or NONE
# $4 - MAIL_USER The email address to send the notifications IMPORTANT: mail cannot be gmail, so use your university email
# $5 - CPUS_PER_TASK The number of CPUs per task
# $6 - TIME The time limit for the job in format "HH:MM:SS"
# $7 - INPUT_FILE_PATH The path to the input file
# $8 - INPUT_FILE_FORMAT The format of the input file
# $9 - OUTPUT_FILE_FORMAT The format of the output file


# Define color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m'

# Function to display help message
function display_help() {
    echo -e "${YELLOW}Usage: $0 CLUSTER PARTITION MAIL_TYPE MAIL_USER CPUS_PER_TASK TIME${NC}"
    echo
    echo "Parameters:"
    echo -e "${YELLOW}CLUSTER${NC}            Possible values: serial or inter"
    echo -e "${YELLOW}PARTITION${NC}          Possible values: serial_std, serial_long or teramem_inter"
    echo -e "${YELLOW}MAIL_TYPE${NC}          Possible values: BEGIN, END, FAIL, REQUEUE, ALL or NONE"
    echo -e "${YELLOW}MAIL_USER${NC}          The email address to send the notifications. IMPORTANT: mail cannot be gmail, so use your university email"
    echo -e "${YELLOW}CPUS_PER_TASK${NC}      The number of CPUs per task"
    echo -e "${YELLOW}TIME${NC}               The time limit for the job in format \"HH:MM:SS\""
    echo -e "${YELLOW}INPUT_FILE_PATH${NC}    The path to the input file"
    echo -e "${YELLOW}INPUT_FILE_FORMAT${NC}  The format of the input file"
    echo -e "${YELLOW}OUTPUT_FILE_FORMAT${NC} The format of the output file"
    echo
    echo "Example:"
    echo -e "${YELLOW}  $0 serial serial_std ALL your_university_email@example.com 4 02:00:00 ../input/assignment-3/2d-cuboid-collision.xml xml vtk${NC}"
    echo
}

# Check if the user requested help
if [[ "$1" == "-h" || "$1" == "--help" ]]; then
    display_help
    exit 0
fi

# Check if the number of parameters is correct
if [ "$#" -ne 9 ]; then
    echo -e "${RED}Error: You need to provide exactly 9 parameters.${NC}"
    echo -e "${RED}Run the script with -h or --help to see the usage.${NC}"
    exit 1
fi

# Check if the first parameter is either "serial" or "inter"
if [ "$1" != "serial" ] && [ "$1" != "inter" ]; then
    echo -e "${RED}Error: The first parameter must be either 'serial' or 'inter'.${NC}"
    exit 1
fi

# Check if the second parameter is either "serial_std", "serial_long" or "teramem_inter"
if [ "$2" != "serial_std" ] && [ "$2" != "serial_long" ] && [ "$2" != "teramem_inter" ]; then
    echo -e "${RED}Error: The second parameter must be either 'serial_std', 'serial_long' or 'teramem_inter'.${NC}"
    exit 1
fi

# Check if the third parameter is either "BEGIN", "END", "FAIL", "REQUEUE", "ALL" or "NONE"
if [ "$3" != "BEGIN" ] && [ "$3" != "END" ] && [ "$3" != "FAIL" ] && [ "$3" != "REQUEUE" ] && [ "$3" != "ALL" ] && [ "$3" != "NONE" ]; then
    echo -e "${RED}Error: The third parameter must be either 'BEGIN', 'END', 'FAIL', 'REQUEUE', 'ALL' or 'NONE'.${NC}"
    exit 1
fi

# Check if the fifth parameter is a number
if ! [[ "$5" =~ ^[0-9]+$ ]]; then
    echo -e "${RED}Error: The fifth parameter must be a number.${NC}"
    exit 1
fi

# Check if the sixth parameter is in format "HH:MM:SS"
if ! [[ "$6" =~ ^[0-9][0-9]:[0-9][0-9]:[0-9][0-9]$ ]]; then
    echo -e "${RED}Error: The sixth parameter must be in format 'HH:MM:SS'.${NC}"
    exit 1
fi

# Check if the eighth parameter is a valid file format
if [ "$8" != "xml" ] && [ "$8" != "txt" ]; then
    echo -e "${RED}Error: The eighth parameter must be either 'xml' or 'txt'.${NC}"
    exit 1
fi

# Check if the ninth parameter is a valid file format
if [ "$9" != "xml" ] && [ "$9" != "vtk" ]; then
    echo -e "${RED}Error: The ninth parameter must be either 'xml' or 'vtk'.${NC}"
    exit 1
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
cd .. && rm -rf build/ && mkdir build/ && cd build && cmake .. && make

# Print the parameters
echo -e
echo -e "${GREEN}Making Cluster_start.cmd with the following parameters:${NC}"
echo -e "${GREEN}CLUSTER: ${YELLOW}$1${NC}"
echo -e "${GREEN}PARTITION: ${YELLOW}$2${NC}"
echo -e "${GREEN}MAIL_TYPE: ${YELLOW}$3${NC}"
echo -e "${GREEN}MAIL_USER: ${YELLOW}$4${NC}"
echo -e "${GREEN}CPUS_PER_TASK: ${YELLOW}$5${NC}"
echo -e "${GREEN}TIME: ${YELLOW}$6${NC}"
echo -e "${GREEN}INPUT_FILE_PATH: ${YELLOW}$7${NC}"
echo -e "${GREEN}INPUT_FILE_FORMAT: ${YELLOW}$8${NC}"
echo -e "${GREEN}OUTPUT_FILE_FORMAT: ${YELLOW}$9${NC}"

# Remove any existing cluster_start.cmd file
cd .. && rm -f cluster_start.cmd

# Write the content to the cluster_start.cmd file
cat <<EOL > cluster_start.cmd
#!/bin/sh
#SBATCH -J MolSimGroupA
#SBATCH -o ./%x.%j.%N.out
#SBATCH -D build/
#SBATCH --get-user-env
#SBATCH --clusters=$1
#SBATCH --partition=$2
#SBATCH --mail-type=$3
#SBATCH --mail-user=$4
#SBATCH --mem=1000mb
#SBATCH --cpus-per-task=$5
#SBATCH --export=NONE
#SBATCH --time=$6

./MolSim -f $7 -i $8 -o $9
EOL

echo -e "${GREEN}Success: ${NC}cluster_start.cmd has been created with the provided parameters.${NC}"
echo -e

echo -e "${GREEN}The following commands do not need to be adapted, you can copy and paste them as they are.${NC}"
echo -e "${GREEN}----------------------------------------${NC}"
echo -e "${GREEN}To submit the job, run the following command:${NC}"
echo -e "${YELLOW}sbatch ../cluster_start.cmd${NC}"
echo -e "${GREEN}----------------------------------------${NC}"
echo -e "${GREEN}To see the status of the job, run the following command:${NC}"
echo -e "${YELLOW}squeue --cluster $1 --me${NC}"
echo -e "${GREEN}----------------------------------------${NC}"
echo -e "${GREEN}To cancel the job, run the following command:${NC}"
echo -e "${YELLOW}scancel -M $1 --me${NC}"
echo -e "${GREEN}----------------------------------------${NC}"