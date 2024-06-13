#!/bin/bash
# This script loads the necessary modules for the cluster
#Params list:
# $1 - CLUSTER Possible values: serial or inter
# $2 - PARTITION Possible values: serial_std, serial_long or teramem_inter
# $3 - MAIL_TYPE Possible values: BEGIN, END, FAIL, REQUEUE, ALL or NONE
# $4 - MAIL_USER The email address to send the notifications
# $5 - CPUS_PER_TASK The number of CPUs per task
# $6 - TIME The time limit for the job in format "HH:MM:SS"


# Check if the number of parameters is correct
if [ "$#" -ne 6 ]; then
    echo "Error: You need to provide exactly 6 parameters."
    exit 1
fi

# Check if the first parameter is either "serial" or "inter"
if [ "$1" != "serial" ] && [ "$1" != "inter" ]; then
    echo "Error: The first parameter must be either 'serial' or 'inter'."
    exit 1
fi

# Check if the second parameter is either "serial_std", "serial_long" or "teramem_inter"
if [ "$2" != "serial_std" ] && [ "$2" != "serial_long" ] && [ "$2" != "teramem_inter" ]; then
    echo "Error: The second parameter must be either 'serial_std', 'serial_long' or 'teramem_inter'."
    exit 1
fi

# Check if the third parameter is either "BEGIN", "END", "FAIL", "REQUEUE", "ALL" or "NONE"
if [ "$3" != "BEGIN" ] && [ "$3" != "END" ] && [ "$3" != "FAIL" ] && [ "$3" != "REQUEUE" ] && [ "$3" != "ALL" ] && [ "$3" != "NONE" ]; then
    echo "Error: The third parameter must be either 'BEGIN', 'END', 'FAIL', 'REQUEUE', 'ALL' or 'NONE'."
    exit 1
fi

# Check if the fifth parameter is a number
if ! [[ "$5" =~ ^[0-9]+$ ]]; then
    echo "Error: The fifth parameter must be a number."
    exit 1
fi

# Check if the sixth parameter is in format "HH:MM:SS"
if ! [[ "$6" =~ ^[0-9][0-9]:[0-9][0-9]:[0-9][0-9]$ ]]; then
    echo "Error: The sixth parameter must be in format 'HH:MM:SS'."
    exit 1
fi

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


#Set up the build directory
cd .. && rm -rf build/ && mkdir build/

# Print the parameters
echo "Made Cluster_start.cmd file with the following parameters:"
echo "CLUSTER: $1"
echo "PARTITION: $2"
echo "MAIL_TYPE: $3"
echo "MAIL_USER: $4"
echo "CPUS_PER_TASK: $5"
echo "TIME: $6"

rm -f cluster_start.cmd

# Reference: https://doku.lrz.de/example-serial-job-scripts-on-the-linux-cluster-10746634.html

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

echo "cluster_start.cmd has been created with the provided parameters. To submit the job, run the following command:"
echo "sbatch cluster_start.cmd"