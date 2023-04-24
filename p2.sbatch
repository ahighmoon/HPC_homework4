#!/bin/bash

#SBATCH --job-name=inner-prod
#SBATCH --nodes=4 
#SBATCH --cpus-per-task=1
#SBATCH --tasks-per-node=1
#SBATCH --mem=2GB 
#SBATCH --time=00:10:00 
#SBATCH --verbose

module purge 
module load  amber/openmpi/intel/20.06 

mpirun -np 4 ./int_ring 1000



