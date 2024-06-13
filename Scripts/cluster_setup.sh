#!/bin/bash
# This script loads the necessary modules for the cluster
# Params list:
# $1 - CLUSTER Possible values: serial or inter
# $2 - PARTITION Possible values: serial_std, serial_long or teramem_inter
# $3 - MAIL_TYPE Possible values: BEGIN, END, FAIL, REQUEUE, ALL or NONE
# $4 - MAIL_USER The email address to send the notifications IMPORTANT: mail cannot be gmail, so use your university email
# $5 - CPUS_PER_TASK The number of CPUs per task
# $6 - TIME The time limit for the job in format "HH:MM:SS"

# Define color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

# Function to display help message
function display_help() {
    echo "Usage: $0 CLUSTER PARTITION MAIL_TYPE MAIL_USER CPUS_PER_TASK TIME"
    echo
    echo "Parameters:"
    echo "  CLUSTER        Possible values: serial or inter"
    echo "  PARTITION      Possible values: serial_std, serial_long or teramem_inter"
    echo "  MAIL_TYPE      Possible values: BEGIN, END, FAIL, REQUEUE, ALL or NONE"
    echo "  MAIL_USER      The email address to send the notifications. IMPORTANT: mail cannot be gmail, so use your university email"
    echo "  CPUS_PER_TASK  The number of CPUs per task"
    echo "  TIME           The time limit for the job in format \"HH:MM:SS\""
    echo
    echo "Example:"
    echo "  $0 serial serial_std ALL your_university_email@example.com 4 02:00:00"
    echo
}

# Check if the user requested help
if [[ "$1" == "-h" || "$1" == "--help" ]]; then
    display_help
    exit 0
fi

# Check if the number of parameters is correct
if [ "$#" -ne 6 ]; then
    echo -e "${RED}Error: You need to provide exactly 6 parameters.${NC}"
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

# Load the necessary modules
module load slurm_setup
module load cmake/3.21.4
module load gcc/11.2.0
module load boost/1.75.0-gcc11
module load xerces-c/3.2.1

# List all the loaded modules
module list

# Set up the build directory
cd .. && rm -rf build/ && mkdir build/

# Print the parameters
echo -e "${GREEN}Made Cluster_start.cmd file with the following parameters:${NC}"
echo -e "${GREEN}CLUSTER: $1${NC}"
echo -e "${GREEN}PARTITION: $2${NC}"
echo -e "${GREEN}MAIL_TYPE: $3${NC}"
echo -e "${GREEN}MAIL_USER: $4${NC}"
echo -e "${GREEN}CPUS_PER_TASK: $5${NC}"
echo -e "${GREEN}TIME: $6${NC}"

# Remove any existing cluster_start.cmd file
rm -f cluster_start.cmd

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

./MolSim -f ../input/assignment-3/disc-against-wall.xml -i xml -o vtk
EOL

echo -e "${GREEN}cluster_start.cmd has been created with the provided parameters. To submit the job, run the following command:${NC}"
echo -e "${GREEN}sbatch cluster_start.cmd${NC}"
