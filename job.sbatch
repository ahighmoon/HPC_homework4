#!/bin/bash

#SBATCH --job-name=HPC_pingpong
#SBATCH --nodes=4
#SBATCH --cpus-per-task=1
#SBATCH --tasks-per-node=1
#SBATCH --mem=2GB 
#SBATCH --time=01:00:00 
#SBATCH --verbose

module purge 
module load  amber/openmpi/intel/20.06 

mpiexec ./pingpong 0 1
