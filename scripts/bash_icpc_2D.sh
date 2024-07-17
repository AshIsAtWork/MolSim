#!/bin/sh
#SBATCH -J MolSim_A
#SBATCH -o ./%x.%j.%N.out
#SBATCH -D ../build/
#SBATCH --get-user-env
#SBATCH --clusters=inter
#SBATCH --partition=cm2_inter
#SBATCH --mail-type=ALL
#SBATCH --mail-user=d.schade@tum.de
#SBATCH --nodes=1
#SBATCH --mem=1000mb
#SBATCH --cpus-per-task=7
#SBATCH --export=NONE
#SBATCH --time=00:10:00

export OMP_NUM_THREADS=7

./MolSim -f ../input/assignment-5/benchmark-contest-2D.xml -i xml -o vtk -t
